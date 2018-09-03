#pragma once
#include "Struct.h"
#include <QString>
#include <QVector>

//储存获取的单个歌词信息
struct LyricInfo
{
    QString strPlaneText;
    QString strLabelText;
    QString strSong;
    QString strArtist;
    QString strLyricFrom;
};

struct LyricSearchResult
{
	bool bCurrentSearchDone;						//标记本次搜索是否结束
	bool bAppendToList;								//是否为追加到列表，false 则先清空
    QVector<LyricInfo> vecLyricInfoTotal;			//所有获得的歌词
	bool bShowUnexpectedResultTip;					//是否显示意外结果的提示
    QString strUnexpectedResultTip;                 //意外结果的提示
};


//网易云歌曲信息结构（可用于存储从网易云搜索到的数据）
typedef struct _SONG_INFO{
    int nID;
    QString strArtists;
    QString strSong;
} SONGINFO;


struct SongInfoGuessResult
{
    int nResultType;               //1 表示获得到了歌曲名和歌手名  2 表示只获取歌曲名  3表示没有获取到任何猜测结果
    QString strSongName;		//歌曲名
    QString strArtist;			//歌手信息
};
