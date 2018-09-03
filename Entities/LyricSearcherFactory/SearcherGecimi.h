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
* @file       SearcherGecimi.h
*
* @Describe   定义歌词迷搜索歌词类，实现搜索歌词接口； 
*/

#pragma once
#include "ISearcher.h"
#include <QObject>
#include <QString>

class SearcherGecimi: public ISearcher, public QObject
{
public:
	//搜索歌词
    virtual bool SearchLyric(QString strSong, QString strArtist, QVector<LyricInfo>& vecLyricInfo);
    
private:
	//从 去链接中下载歌词文件，并获得具体信息置于 vecLyricInfo 中
    void GetLyricInfoFromLinks(QVector<QString>& vecLyricLink, QVector<LyricInfo>& vecLyricInfo);

	//从歌词文本缓存获取 歌词 信息
    void GetOneLyricInfoFromLyricBuffer(QString strLyricBuffer,LyricInfo& lyricInfo);

	//从 歌词迷获取的json数据获取歌词链接
    bool GetLyricLinkFromJson(QString strLyricJson, QVector<QString>& vecLyricLink);

};
