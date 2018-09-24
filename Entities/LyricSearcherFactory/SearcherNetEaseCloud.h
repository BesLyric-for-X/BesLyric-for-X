/*
	BesLyric  一款 操作简单、功能实用的 专门用于制作网易云音乐滚动歌词的 歌词制作软件。
    Copyright (C) 2018

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
* @file       SearcherNetEaseCloud.h
* 
* @Describe   定义网易云音乐搜索歌词类，实现搜索歌词接口； 
*/

#pragma once
#include "ISearcher.h"
#include <QObject>
#include <QVector>
#include <QString>

class SearcherNetEaseCloud: public ISearcher,public QObject
{
public:
    virtual ~SearcherNetEaseCloud(){}

	//搜索歌词
    virtual bool SearchLyric(QString strSong, QString strArtist, QVector<LyricInfo>& vecLyricInfo);

    //通过使用歌名 和 艺术家名获得 歌曲信息列表
    static bool GetSongListWithNameAndArtist(QString strSong, QString strArtist, QVector< SONGINFO >& vecSongList, QString& strLastResult);
    
    //获得结果中的歌曲信息列表
    static bool GetSongListFromJson(QString strJsonRes, QVector< SONGINFO >& vecSongList);
private:

    //从单个歌词json数据获得歌词
    bool GetOneLyricFromJson(QString strLyricJson,  LyricInfo& oneLyricInfo);

    //从歌词文本缓存获取 歌词 信息
    void GetOneLyricInfoFromLyricBuffer(QString strLyricBuffer,LyricInfo& lyricInfo);

};




