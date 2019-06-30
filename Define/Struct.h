#pragma once
#include "Struct.h"
#include <QString>
#include <QVector>

//网易云歌曲信息结构（可用于存储从网易云搜索到的数据）
typedef struct _SONG_INFO{
    int nID;
    QString strArtists;
    QString strSong;
    QString strAlbum;
    int nTime;          //毫秒时间

    //用于显示歌曲列表时附加信息（下载状态）
    int nPercentage;             //当前下载百分比(-1 用于表示从未下载，-2 表示网络文件无效, -3 表示网络连接错误，-4 表示本地存储失败）
} SONGINFO;


struct SongInfoGuessResult
{
    int nResultType;               //1 表示获得到了歌曲名和歌手名  2 表示只获取歌曲名  3表示没有获取到任何猜测结果
    QString strSongName;		//歌曲名
    QString strArtist;			//歌手信息
};


//储存获取的单个歌词信息
struct LyricInfo
{
    QString strPlaneText;
    QString strLabelText;
    QString strSong;
    QString strArtist;
    QString strLyricFrom;
};

//用于储存传送搜索歌词的结果
//(原先用于传输歌词结果，为了复用，直接添加 Vector<SONGINFO> vecSongInfo 成员用于传输歌曲信息)
struct LyricSearchResult
{
    bool bCurrentSearchDone;						//标记本次搜索是否结束
    bool bAppendToList;								//是否为追加到列表，false 则先清空
    QVector<LyricInfo> vecLyricInfoTotal;			//所有获得的歌词
    QVector<SONGINFO> vecSongInfo;                  //获取的所有歌曲信息
    bool bShowUnexpectedResultTip;					//是否显示意外结果的提示
    QString strUnexpectedResultTip;                 //意外结果的提示
};

