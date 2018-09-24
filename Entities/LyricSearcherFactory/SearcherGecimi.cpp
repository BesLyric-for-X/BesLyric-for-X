#include "SearcherGecimi.h"
#include "Utility/NetAccess.h"

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>

#include <QTextCodec>

#include "LrcProcessor.h"

//搜索歌词
bool SearcherGecimi::SearchLyric(QString strSong, QString strArtist, QVector<LyricInfo>& vecLyricInfo)
{
    QString strSearchUrl = "http://gecimi.com/api/lyric";

    strSearchUrl += strSong.size() == 0 ? "" : ("/" + strSong);
    strSearchUrl += strArtist.size() == 0 ? "" : ("/"+  strArtist);

    QString strSaveBuffer;

    if(!NetworkAccess::SyncDownloadString( strSearchUrl, strSaveBuffer))
	{
        m_strLastResult = tr("网络连接失败，无法获取数据");
		return false;
	}

    //从 歌词迷获取的json数据获取歌词链接
    QVector<QString>	vecLyricLink;
    if(!GetLyricLinkFromJson(strSaveBuffer, vecLyricLink))
    {
        m_strLastResult = tr("歌词迷数据格式异常，无法解析数据");
        return false;
    }

    //从 去链接中下载歌词文件，并获得具体信息置于 vecLyricInfo 中
    vecLyricInfo.clear();
    GetLyricInfoFromLinks(vecLyricLink, vecLyricInfo);

    if(vecLyricLink.size() > vecLyricInfo.size())
    {
        m_strLastResult.sprintf("共查询到%d条数据，成功下载%d条数据",vecLyricLink.size(),vecLyricInfo.size());
    }
    else
    {
        m_strLastResult.sprintf("成功下载询到的%d条数据",vecLyricInfo.size());
    }

	return true;
}

//从 去链接中下载歌词文件，并获得具体信息置于 vecLyricInfo 中
void SearcherGecimi::GetLyricInfoFromLinks(QVector<QString>& vecLyricLink, QVector<LyricInfo>& vecLyricInfo)
{
    for(auto iter = vecLyricLink.begin(); iter != vecLyricLink.end(); iter++)
    {
        QString strLyricBuffer;

        if(!NetworkAccess::SyncDownloadString( *iter,strLyricBuffer))
            continue;
		
        //从歌词文本缓存获取 歌词 信息
        LyricInfo lyricInfo;
        GetOneLyricInfoFromLyricBuffer(strLyricBuffer, lyricInfo);
		
        vecLyricInfo.push_back(lyricInfo);
    }
}


//从歌词文本缓存获取 歌词 信息
void SearcherGecimi::GetOneLyricInfoFromLyricBuffer(QString strLyricBuffer,LyricInfo& lyricInfo)
{
    LrcProcessor processor;

    processor.LoadFromRawText(strLyricBuffer);

    QVector<QPair<int, QString>> vecTimeLineInfo = processor.GetLrcLyric();

    lyricInfo.strSong = processor.m_strTitle;
    lyricInfo.strArtist = processor.m_strArtist;
    lyricInfo.strLyricFrom = tr("歌词迷");
	
    QString strPlaneText = "";
    QString strLabelText = "";;

    for(auto iter = vecTimeLineInfo.begin(); iter != vecTimeLineInfo.end(); iter++)
    {
        strPlaneText += iter->second + "\n";
        strLabelText += LrcProcessor::ToLrcLine(*iter)+ "\n";
    }
	
    lyricInfo.strPlaneText = strPlaneText;
    lyricInfo.strLabelText = strLabelText;
}

//从 歌词迷获取的json数据获取歌词链接
bool SearcherGecimi::GetLyricLinkFromJson(QString strLyricJson, QVector<QString>& vecLyricLink)
{
    vecLyricLink.clear();

    QByteArray byt = strLyricJson.toUtf8();
    QJsonDocument doc=QJsonDocument::fromJson(byt);

    if(doc.isObject())
    {
        //从 歌词迷获取的json数据获取歌词链接置于 vecLyricLink
        /*
            歌词迷获取的歌词的结构为：
            {
               "code" : 0,
               "count" : 6,
               "result" : [
                  {
                     "aid" : 3179315,
                     "artist_id" : 11807,
                     "lrc" : "http://s.gecimi.com/lrc/388/38847/3884774.lrc",
                     "sid" : 3884774,
                     "song" : "我们的歌"
                  },
                  {
                  ...
                  }
                ]
            }
         */

        QJsonObject jObject= doc.object();
        QJsonArray arrayTemp=jObject.value("result").toArray();
        for(int i=0;i<arrayTemp.count();i++)
        {
            QJsonObject objTemp=arrayTemp.at(i).toObject();
            QString lrc=objTemp.value("lrc").toString();
            vecLyricLink.push_back(lrc);
        }
    }
    else
    {
        return false;
    }

	return true;
}

