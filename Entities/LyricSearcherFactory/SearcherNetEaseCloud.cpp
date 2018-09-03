#include "SearcherNetEaseCloud.h"
#include "Define/Define.h"
#include "NetAccess.h"
#include "LrcProcessor.h"
#include <QUrlQuery>
#include <QTextCodec>

#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>

//搜索歌词
bool SearcherNetEaseCloud::SearchLyric(QString strSong, QString strArtist, QVector<LyricInfo>& vecLyricInfo)
{
	//获取id列表
    QVector< SONGINFO > vecSongList;
	if(!GetSongListWithNameAndArtist(strSong, strArtist,vecSongList, m_strLastResult))
		return false;

    for(auto iter = vecSongList.begin(); iter != vecSongList.end(); iter++)
    {
        if(!iter->strSong.contains(strSong))//歌名不包含于结果歌词名则跳过
            continue;

        QString strLyricJson;

        QUrlQuery query;
        query.addQueryItem("os", "osx");
        query.addQueryItem("id", QString::number(iter->nID));
        query.addQueryItem("lv", "-1");
        query.addQueryItem("kv", "-1");
        query.addQueryItem("tv", "-1");

        //获取id对应的歌词
        if(!NetworkAccess::SyncDownloadString("http://music.163.com/api/song/lyric", strLyricJson,query))
        {
            m_strLastResult = tr("网络连接失败，无法获取歌词内容数据");
            return false;
        }

        //从json 数据获取一个歌词添加到
        LyricInfo oneLyricInfo;

        if(!GetOneLyricFromJson(strLyricJson, oneLyricInfo))
            continue;
		
        oneLyricInfo.strSong =  iter->strSong;
        oneLyricInfo.strArtist =  iter->strArtists;
        vecLyricInfo.push_back(oneLyricInfo);
    }

	return true;
}

//通过使用歌名 和 艺术家名获得 歌曲信息列表
bool SearcherNetEaseCloud::GetSongListWithNameAndArtist(QString strSong, QString strArtist, QVector< SONGINFO >& vecSongList,QString& strLastResult)
{
    QTextCodec *utf8 = QTextCodec::codecForName("utf-8");
    QByteArray encodedSong = utf8->fromUnicode(strSong).toPercentEncoding();
    QByteArray encodedArtist = utf8->fromUnicode(strArtist).toPercentEncoding();
    QByteArray queryData;
    queryData.append("csrf_token=&s="+encodedArtist+"+"+ encodedSong +"&type=1&offset=0&total=True&limit=20");

    QString strRes;
    if(!NetworkAccess::SyncDownloadStringPost( "https://music.163.com/api/search/get/web",strRes,queryData))
    {
        strLastResult = tr("网络连接失败，无法获取歌词索引数据");
        return false;
    }
	
    //strRes = strRes.substr( strRes.find_first_of('{'), strRes.find_last_of('}') - strRes.find_first_of('{')+1);

    //获取id列表
    if(!GetSongListFromJson(strRes, vecSongList))
    {
        strLastResult = tr("网易云歌词数据格式异常，无法解析数据");
        return false;
    }

    return true;
}

//获得结果中的歌曲信息列表
bool SearcherNetEaseCloud::GetSongListFromJson(QString strJsonRes, QVector< SONGINFO >& vecSongList)
{
    vecSongList.clear();
    QByteArray byte = strJsonRes.toUtf8();
    QJsonDocument doc=QJsonDocument::fromJson(byte);

    if(doc.isObject())
    {
        /*
            获取的信息的结构为：
            {
               "code" : 200,
               "result" : {
                  "songCount" : 3,
                  "songs" : [
                     {
                        "album" : {...},
                        "alias" : [],
                        "artists" : [{... ,"name" : "王力宏", ...},{...}]
                        ...
                        "id" : 25642952,
                        "mvid" : 5293365,
                        "name" : "我们的歌",
                        ...
                     }
                     ,
                     {...}
                    ]
                }
            }
        */

        QJsonObject jObject= doc.object();
        QJsonObject resultObject= jObject.value("result").toObject();
        QJsonArray songsArray = resultObject.value("songs").toArray();
        for(int i = 0; i < songsArray.size(); i++)
        {
            QJsonObject songObject = songsArray.at(i).toObject();

            int nID = songObject.value("id").toInt();
            QString strSong = songObject.value("name").toString();
            QString strArtists;

            QJsonArray artistsArray = songObject.value("artists").toArray();
            for(int j = 0; j < artistsArray.size(); j++)
            {
                QJsonObject artistObject = artistsArray.at(j).toObject();
                QString artistName = artistObject.value("name").toString();

                strArtists += j > 0 ? "、":"";
                strArtists += artistName;
            }

            SONGINFO songInfo;
            songInfo.nID = nID;
            songInfo.strArtists = strArtists;
            songInfo.strSong = strSong;
            vecSongList.push_back(songInfo);  //将从一首歌收集的信息储存
        }
    }

    return true;
}

//从单个歌词json数据获得歌词
bool SearcherNetEaseCloud::GetOneLyricFromJson(QString strLyricJson,  LyricInfo& oneLyricInfo)
{
    QByteArray byte = strLyricJson.toUtf8();
    QJsonDocument doc=QJsonDocument::fromJson(byte);

    if(doc.isObject())
    {
        /*
            获取的信息的结构为：
            {
                 "code" : 200,
                   "klyric" : {
                      "lyric" : "..."
                      "version" : 6
                   },
                   "lrc" : {
                      "lyric" : "[00:00.00] 作曲 : 王力宏\n[00:01.00] 作词 : 王力宏/陳信延\n[00:15.750]已经听了一百遍 怎么听都不会倦\n[00:23.140]从白天唱到黑夜 你一直在身边（一直在身边）\n[00:29.450]如果世界太危险 只有音乐最安全  带着我进梦里面 让歌词都实现\n[00:42.519]无论是开心还是难过我的爱一直不变\n[00:46.389]（不必担心时间流逝带走一切）\n[00:50.789]无论是HIP-HOP还是摇滚我的爱一直不变\n[00:54.389]（所有美好回忆记录在里面）\n[00:56.149]这种Forever Love 那么深\n[00:58.649]我们的歌 那么真\n[01:00.279]无国界 跨时代\n[01:02.179]再不会叫我KISS GOODBYE\n[01:03.669]要每一句能够动人心弦  YE~~\n[01:10.570]情人总分分合合\n[01:13.509]可是我们却越爱越深\n[01:17.389]认识你让我的幸福如此  悦耳\n[01:24.240]能不能不要切歌  继续唱我们的歌\n[01:31.139]让感动一辈子都记得\n[01:37.580]en heng~~~\n[01:40.699]wo~  o~\n[01:48.309]已经听了一百遍 怎么听都不会倦\n[01:55.158]从白天唱到黑夜 你一直在身边（一直在身边）\n[02:02.800]如果世界太危险 只有音乐最安全  带着我进梦里面 让歌词都实现\n[02:15.150]无论是开心还是难过我的爱一直不变\n[02:19.150]（不必担心时间流逝带走一切）\n[02:22.000]无论是HIP-HOP还是摇滚我的爱一直不变\n[02:25.670]（所有美好回忆记录在里面）\n[02:28.720]这种Forever Love那么深 我们的歌那么真\n[02:32.850]无国界 跨时代\n[02:34.750]再不会叫我KISS GOODBYE\n[02:36.228]要每一句能够动人心弦 YE~~\n[02:43.130]情人总分分合合\n[02:46.140]可是我们却越爱越深\n[02:50.300]认识你让我的幸福如此 悦耳\n[02:56.880]能不能不要切歌 继续唱我们的歌\n[03:03.660]让感动一辈子都记得\n[03:08.370]\n[03:18.440]（一辈子都记得）\n[03:36.240]情人总分分合合\n[03:39.280]可是我们却越爱越深\n[03:43.860]认识你让我的幸福如此 悦耳\n[03:51.400]能不能不要切歌 继续唱我们的歌\n[03:57.400]让感动一辈子都记得\n",
                      "version" : 18
                   },
                   "qfy" : false,
                   "sfy" : false,
                   "sgc" : true,
                   "tlyric" : {
                      "lyric" : null,
                      "version" : 0
                   }
            }
        */

        QJsonObject jObject= doc.object();

        QJsonObject lrcObject= jObject.value("lrc").toObject();
        QString strLyric= lrcObject.value("lyric").toString();

        //QJsonObject tlyricObject= jObject.value("tlyric").toObject();
        //QString strLyricTranslate= tlyricObject.value("lyric").toString();

        GetOneLyricInfoFromLyricBuffer(strLyric, oneLyricInfo);
    }

    return true;
}

//从歌词文本缓存获取 歌词 信息
void SearcherNetEaseCloud::GetOneLyricInfoFromLyricBuffer(QString strLyricBuffer,LyricInfo& lyricInfo)
{
    LrcProcessor processor;

    processor.LoadFromRawText(strLyricBuffer);
    QVector<QPair<int, QString>> vecTimeLineInfo = processor.GetLrcLyric();

    lyricInfo.strSong = processor.m_strTitle;
    lyricInfo.strArtist = processor.m_strArtist;
    lyricInfo.strLyricFrom = tr("网易云音乐");

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
