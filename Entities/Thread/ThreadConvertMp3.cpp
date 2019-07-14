#include "ThreadConvertMp3.h"
#include "Entities/MP3Editor/mp3Editor.h"

ThreadConvertMp3::ThreadConvertMp3()
{
    mp3Editor = new Mp3Editor(this);
    connect(mp3Editor, &Mp3Editor::sig_getEditResult, this, &ThreadConvertMp3::OnGetEditResultd);


}

void ThreadConvertMp3::doTask(ConvertTask task)
{
    taskQueue.enqueue(task);

    if(!isRunning())
        start(NormalPriority);
}


void ThreadConvertMp3::OnGetEditResultd(bool success, QString path, QString errorTip)
{
    if(!success)
    {
        BesMessageBox::information(tr("提示"),
            tr("转换 mp3 时发生错误")+ "\n\n" + tr("出错细节:")+ errorTip + "("+ path +")");
    }

    oneConvertDone = true;
}

void ThreadConvertMp3::run()
{
    while(!taskQueue.empty())
    {
        ConvertTask task = taskQueue.dequeue();
        //QString localImagePath;
        QString pathImage;
        CustomMp3Data mp3Data;
        int index,index1,index2;

        //根据 songNcmId 下载歌曲的信息（包括专辑图片url）
        NcmDetailInfo info;
        QString errorTip;

        if(!downloadNcmInfo(task.songNcmId, info,errorTip ))
            goto directCopy;

        //下载专辑图片
        info.albumPicUrl;
        index1 = task.sourceMp3FilePath.lastIndexOf('/');
        index2 = task.sourceMp3FilePath.lastIndexOf('\\');
        index = qMax(index1, index2);
        if(index == -1)
            goto directCopy;

        pathImage = task.sourceMp3FilePath.left(index)+ (index1==-1?'\\':'/')
                     + QString("%1.jpg").arg(task.songNcmId);

        if(!NetworkAccess::SyncDownloadString(info.albumPicUrl, pathImage,QUrlQuery(),true))
            goto directCopy;


        //将图片和相关信息加入到源mp3得到最终的 mp3
        mp3Data.imagePath = pathImage;
        mp3Data.artist = info.firstArtist;
        mp3Data.album = info.album;
        mp3Data.title = info.title;
        mp3Data.mp3OutputPath = task.targetMp3FilePath;

        oneConvertDone = false;

        if(!mp3Editor->CustomizeMp3(task.sourceMp3FilePath,mp3Data))
            goto directCopy;

        while(!oneConvertDone)
            msleep(500);

        goto convertEnd;

directCopy:
        QFile::copy(task.sourceMp3FilePath, task.targetMp3FilePath);
        oneConvertDone = true;

convertEnd:
        //删除临时图片和临时mp3
        QFile::remove(task.sourceMp3FilePath);
        QFile::remove(mp3Data.imagePath);

        emit sig_finishConvert(task.songNcmId);
    }
}

bool ThreadConvertMp3::downloadNcmInfo(int ncmId, NcmDetailInfo& info, QString& strLastResult)
{
    //QTextCodec *utf8 = QTextCodec::codecForName("utf-8");
    QByteArray strNcmId = QString("%1").arg(ncmId).toLocal8Bit();
    QByteArray queryData;
    queryData.append("id="+strNcmId+"&ids=%5B" + strNcmId+"%5D");
    QString strRes;
    if(!NetworkAccess::SyncDownloadStringPost( "https://music.163.com/api/song/detail/",strRes,queryData))
    {
        strLastResult = tr("网络连接失败，无法获取索引数据");
        return false;
    }

    //获取id列表
    if(!GetSongInfoFromJson(strRes, info))
    {
        strLastResult = tr("网易云歌词数据格式异常，无法解析数据");
        return false;
    }

    return true;
}


//获得结果中的歌曲信息列表
bool ThreadConvertMp3::GetSongInfoFromJson(QString strJsonRes, NcmDetailInfo& info)
{
    QByteArray byte = strJsonRes.toUtf8();
    QJsonDocument doc=QJsonDocument::fromJson(byte);

    if(doc.isObject())
    {
        /*
            获取的信息的结构为：
            {
                "songs": [
                    {
                        "name": "锦里",
                        "id": 441617611,
                        ...
                        "artists": [
                            {
                                "name": "HOPE-T",
                                "id": 1198071,
                                ...
                            },
                            {
                                "name": "接个吻，开一枪",
                                "id": 12085016,
                                ...
                            }
                        ],
                        "album": {
                        "name": "锦里（Original Mix）",
                        "id": 35008198,
                        ...
                        "picUrl": "https://p2.music.126.net/m7pdSoU19s4txS1v_w6p-A==/109951162810952889.jpg",
                        ...
                    }
                ],
                "equalizers": {},
                "code": 200
            }
        */

        QJsonObject jObject= doc.object();

        int code = jObject.value("code").toInt();
        if(code != 200)
            return false;

        QJsonArray songsArray = jObject.value("songs").toArray();
        for(int i = 0; i < songsArray.size(); i++)
        {
            QJsonObject songObject = songsArray.at(i).toObject();

            info.songId = songObject.value("id").toInt();
            info.title = songObject.value("name").toString();

            QJsonArray artistsArray = songObject.value("artists").toArray();
            for(int j = 0; j < artistsArray.size(); j++)
            {
                QJsonObject artistObject = artistsArray.at(j).toObject();
                QString artistName = artistObject.value("name").toString();

                info.artists += j > 0 ? "、":"";
                info.artists += artistName;

                if(j == 0)
                    info.firstArtist = artistName;
            }

            QJsonObject AlbumObj = songObject.value("album").toObject();
            if(!AlbumObj.isEmpty())
            {
                info.albumId = AlbumObj.value("id").toInt();
                info.album = AlbumObj.value("name").toString();
                info.albumPicUrl = AlbumObj.value("picUrl").toString();
            }

            break; //只需要第一个结点的信息即可
        }
    }
    else
        return false;

    return true;
}

