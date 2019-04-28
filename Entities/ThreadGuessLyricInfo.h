#ifndef ThreadGuessLyricInfo_H
#define ThreadGuessLyricInfo_H

#include <QObject>
#include <QSharedPointer>
#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include <QFile>

#include "assert.h"
#include "BesMessageBox.h"
#include "NetAccess.h"
#include "LyricSearcherFactory/SearcherNetEaseCloud.h"
#include "UnicodeReader.h"

class ThreadGuessLyricInfo :public QThread
{
    Q_OBJECT

public:
    ThreadGuessLyricInfo():m_bIsGuessing(false){}

    bool StartGuess(QString musicPath, bool bGuessNcmFileName = false)
    {
        QMutexLocker locker(&mutex);

        if(m_bIsGuessing)
        {
            BesMessageBox::information(tr("等待提示"),tr("模块正在使用中，请稍后操作 :)"));
            return false;
        }

        m_bGuessNcmFileName = bGuessNcmFileName;
        m_strMusicPath = musicPath;

        start(HighestPriority);
        return true;
    }


signals:
    //发送出哪个信号由 m_bGuessNcmFileName 决定
    void sig_loadLyricInfoGuessResult(QString songName, QString artist);  //搜索结果发送
    void sig_loadNcmInfoGuessResult(QString songName, QString artist);    //

protected:
    virtual void run()
    {
        QString musicPath;

        {
           QMutexLocker locker(&mutex);
           m_bIsGuessing = true; //进入搜索状态

           musicPath = m_strMusicPath;
        }

        //获得文件名的名称 和 后缀
        QString strFullName;
        QString strExt;

        int nLastDot = -1;
        int nLastSlash = -1;
        for(int i = musicPath.size()-1; i >=0 ; i--)
        {
            if(musicPath.at(i) == '.')
            {
                nLastDot = i;
            }
            else if(musicPath.at(i) == '/')
            {
                nLastSlash = i;
                break;
            }
        }

        //assert( nLastDot != -1 && nLastSlash != -1 && "音乐路径出错");
        if(nLastDot == -1 || nLastSlash == -1)
        {
            //没有结果，直接发送原路径出去
            if(m_bGuessNcmFileName)
                emit sig_loadNcmInfoGuessResult(musicPath,"");
            else
                emit sig_loadLyricInfoGuessResult(musicPath,"");

            return;
        }

        strExt = musicPath.right(musicPath.size() - nLastDot -1).trimmed();
        strFullName = musicPath.right(musicPath.size() - nLastSlash -1).left(nLastDot-nLastSlash-1);

        //准备获取歌词名和歌手
        QString strArtist;
        QString strSongName;

        //如果是mp3，读取读取并分析文件
        bool bHasGuessed = false;
        if(strExt == "mp3")
        {
            QString artist;
            QString songName;

            if(GetGuessResultFromMp3(artist, songName))
            {
                bHasGuessed = true;

                strArtist = artist;
                strSongName = songName;
            }
        }

        //如果没有猜测出结果，根据文件名猜测结果
        if(!bHasGuessed)
        {
            //从文件得到"-" 2个字段
            int nIndex = strFullName.indexOf('-');
            if(nIndex == -1)
            {
                //如果只有1个字段，直接作为歌曲名返回
                strSongName = strFullName;
            }
            else
            {
                QString s1 = strFullName.left( nIndex ).trimmed();
                QString s2 = strFullName.right(strFullName.size() - nIndex -1 ).trimmed();

                //如果得到2个字段，则通过网易云接口搜索得到的结果，确定哪个是文件名，哪个是歌手名
                if(GuessWhetherFirstArtistFromNetease(s1, s2))
                {
                    strArtist = s1;
                    strSongName = s2;
                }
                else{
                    strArtist = s2;
                    strSongName = s1;
                }
            }
        }

        if(m_bGuessNcmFileName)
            emit sig_loadNcmInfoGuessResult(strSongName, strArtist);
        else
            emit sig_loadLyricInfoGuessResult(strSongName, strArtist);

        {
           QMutexLocker locker(&mutex);
           m_bIsGuessing = false; //退出搜索状态
        }
    }


    //从mp3文件中获取猜测结果
    bool GetGuessResultFromMp3(QString& strArtist, QString& songName)
    {
        strArtist.clear();
        songName.clear();

        //读取文件
        QFile file(m_strMusicPath);

        if(!file.open(QIODevice::ReadOnly))
            return false;

        QByteArray byteArray = file.read(file.bytesAvailable());

        file.close();

         //读取文件内容 到 m_pFileData 指向的内存中
        char* pFileData = byteArray.data();

        //分析标题和歌手
        int nGetCount = 0;//获得数据数
        for(int i=0;i < byteArray.size(); i++)
        {
            if(nGetCount >=2 )
                break;
            //标签结构
            //char FrameID[4];
            //char Size[4];
            //char Flags[2];

            union _USize{char bit[4]; int nSize;} USize;

            if( pFileData[i] == 'T' && pFileData[i+1] == 'I' && pFileData[i+2] == 'T' && pFileData[i+3] == '2') //标题
            {
                USize.bit[0] = pFileData[i+4+3];
                USize.bit[1] = pFileData[i+4+2];
                USize.bit[2] = pFileData[i+4+1];
                USize.bit[3] = pFileData[i+4+0];

                int nSize = USize.nSize;

                if(nSize <= 3 || (nSize-3)%2!=0) //这里标签大小大于3时才有内容，且只处理去除bom头后还剩下偶数个
                    continue;

//                char szBom[3];
//                char* pszTitle = new char[nSize-3];

//                memcpy(szBom, pFileData+i+4+6, 3);
//                memcpy(pszTitle, pFileData+i+4+9, nSize-3);

                //if(szBom[0] == 0x01 && szBom[1] == 0xff && szBom[2] == 0xfe){}

                //包括 bom头构建 QByteArray
                QByteArray byteArray;
                byteArray.setRawData(pFileData+i+4+6+1,nSize-1);
                songName = UnicodeReader::ReadUnicodeString(byteArray);

//                delete pszTitle[];
                nGetCount++;
            }
            else if(pFileData[i] == 'T' && pFileData[i+1] == 'P' && pFileData[i+2] == 'E' && pFileData[i+3] == '1')//作者
            {
                USize.bit[0] = pFileData[i+4+3];
                USize.bit[1] = pFileData[i+4+2];
                USize.bit[2] = pFileData[i+4+1];
                USize.bit[3] = pFileData[i+4+0];

                int nSize = USize.nSize;

                if(nSize <= 3 || (nSize-3)%2!=0) //这里标签大小大于3时才有内容，且只处理去除bom头后还剩下偶数个
                    continue;

//                char szBom[3];
//                char* pszArtist = new char[nSize-3];

//                memcpy(szBom, pFileData+i+4+6, 3);
//                memcpy(pszArtist, pFileData+i+4+9, nSize-3);

                //if(szBom[0] == 0x01 && szBom[1] == 0xff && szBom[2] == 0xfe){}

                //包括 bom头构建 QByteArray
                QByteArray byteArray;
                byteArray.setRawData(pFileData+i+4+6+1,nSize-1);
                strArtist = UnicodeReader::ReadUnicodeString(byteArray);

//                delete pszArtist[];
                nGetCount++;
            }
        }

        if(strArtist.size() == 0 && songName.size() == 0)
            return false;

        return true;
    }


    //从网易云接口分析前个字符串是否为歌手
    bool GuessWhetherFirstArtistFromNetease(QString& strFirst, QString& strSecond)
    {
        QTextCodec *utf8 = QTextCodec::codecForName("utf-8");
        QByteArray encodedFirst = utf8->fromUnicode(strFirst).toPercentEncoding();
        QByteArray encodedSecond = utf8->fromUnicode(strSecond).toPercentEncoding();
        QByteArray queryData;
        queryData.append("csrf_token=&s="+encodedFirst+"+"+ encodedSecond +"&type=1&offset=0&total=True&limit=20");

        QString strRes;
        if(!NetworkAccess::SyncDownloadStringPost( "https://music.163.com/api/search/get/web",strRes,queryData))
        {
            //strLastResult = tr("网络连接失败，无法获取索引数据");
            return false;
        }

        //获取id列表
        QVector< SONGINFO > vecSongList;
        if(!SearcherNetEaseCloud::GetSongListFromJson(strRes, vecSongList))
        {
            //"网易云歌词数据格式异常，无法解析数据";,默认返回true，猜测第一个字符串歌手名
            return true;
        }

        int nFirstArtistCount;
        int nFirstSongCount;
        int nSecondArtistCount;
        int nSecondSongCount;

        nFirstArtistCount = nFirstSongCount = nSecondArtistCount = nSecondSongCount = 0;

        for(auto iter = vecSongList.begin(); iter != vecSongList.end(); iter++)
        {
			if (iter->strArtists.contains(strFirst))//第一个串包含于结果歌词名
				nFirstArtistCount++;
			if (iter->strArtists.contains(strSecond))//第二个串包含于结果歌词名
				nSecondArtistCount++;

			QString lowerCaseSong = iter->strSong.toLower();
			if (lowerCaseSong.contains("cover"))				//含有cover 的歌名一般都同时有歌名和歌词
				continue;										//	再此无筛选意义，而且由于搜索结果 “其他歌曲 cover 歌手”也会被查出来
																//	具有误导性，因此跳过
            if( iter->strSong.contains(strFirst))//第一个串包含于结果歌手名
                nFirstSongCount++;
            if( iter->strSong.contains(strSecond))//第二个串包含于结果歌手名
                nSecondSongCount++;
        }

        //评估2个字符串的得分
        int nPointFirstGot = 0;
        int nPointSecondGot = 0;

        if( nFirstArtistCount > nFirstSongCount)
            nPointFirstGot++;
        if( nSecondArtistCount > nSecondSongCount)
            nPointSecondGot++;

        if(nPointFirstGot >= nPointSecondGot)
            return true;
        else
            return false;
    }

    QMutex mutex;
    bool m_bIsGuessing;
    bool m_bGuessNcmFileName;       //本线程用于猜测歌曲 歌词的歌名和歌手，也用来猜测 ncm 歌曲的歌名和歌手
    QString m_strMusicPath;
};


#endif // ThreadGuessLyricInfo_H
