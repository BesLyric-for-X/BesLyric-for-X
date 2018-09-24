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
*   @Describe   定义搜索歌词需要的抽象接口；
*/

#pragma once
#include "Define/Define.h"
#include <QVector>
#include <QString>

class ISearcher
{
public:
	/*
		@brief	搜索歌词
		@param	strSong			搜索的歌曲名
		@param	strArtist		搜索的歌手
		@param	vecLyricInfo	获得的歌词数据
		@return	false —— 网络连接异常 或 数据格式出错
		@note	注意该函数需要将操作的结果写入 m_strLastResult，
	*/
    virtual bool SearchLyric(QString strSong, QString strArtist, QVector<LyricInfo>& vecLyricInfo) = 0;
    
    virtual QString GetLastResult()
	{
        return  m_strLastResult;
	}

    virtual ~ISearcher(){}
protected:
    QString m_strLastResult;
};
