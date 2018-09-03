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
* @Describe   工厂类，产生不同的 歌词搜索器 用于搜索歌词； 
*/

#pragma once
#include "Define/Define.h"
#include "ISearcher.h"
#include "SearcherGecimi.h"
#include "SearcherNetEaseCloud.h"
#include <QSharedPointer>
#include <assert.h>

class CLyricSearcherFactory
{
public:
    static QSharedPointer<ISearcher> CreateSearcher(SEARCH_FROM search_from)
	{
        switch(search_from)
        {
        case SF_GECIMI:
           return QSharedPointer<ISearcher>(new SearcherGecimi());
        case SF_NETEASE:
           return QSharedPointer<ISearcher>(new SearcherNetEaseCloud());
        default:
            assert(false && "未知的歌词搜索类型");
            break;
        }
        return QSharedPointer<ISearcher>(nullptr);
	}

};
