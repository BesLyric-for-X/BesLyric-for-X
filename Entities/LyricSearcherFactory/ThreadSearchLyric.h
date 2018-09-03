#ifndef THREADSEARCHLYRIC_H
#define THREADSEARCHLYRIC_H

#include <QObject>
#include <QSharedPointer>
#include <QThread>
#include <QMutexLocker>
#include <QMutex>

#include "LyricSearcherFactory.h"
#include "BesMessageBox.h"
#include "ISearcher.h"

class ThreadSearchLyric :public QThread
{
    Q_OBJECT

public:
    ThreadSearchLyric():m_bIsSearching(false){}

    void StartSearchLyric(QString artist, QString song)
    {
        QMutexLocker locker(&mutex);

        if(m_bIsSearching)
        {
            BesMessageBox::information(tr("等待提示"),tr("搜索模块正被占用，请等待......"));
            return;
        }

        m_strArtist = artist;
        m_strSong = song;

        start(HighestPriority);
    }


signals:
    void lyricResultChanged(LyricSearchResult);  //搜索消息结果改变


protected:
    virtual void run()
    {
        QString strArtist;
        QString strSong;

        {
           QMutexLocker locker(&mutex);
           m_bIsSearching = true; //进入搜索状态

           strArtist = m_strArtist;
           strSong = m_strSong;
        }

        QVector<LyricInfo> vecLyricInfo;			//暂时储存每一次获得的
        QSharedPointer<ISearcher> pSearcher;

        bool bShowUnexpectedResultTip = true; //显示意外结果提示
        QString strResultTip;

        int nTotalCount = 0;
        for( int i= 0; i < (int)SF_UNDEFINED; i++) //遍历每一种定义的歌词获取方式
        {
            pSearcher = CLyricSearcherFactory::CreateSearcher((SEARCH_FROM)i);

            vecLyricInfo.clear();
            if(!pSearcher->SearchLyric(strSong, strArtist, vecLyricInfo))
            {
                strResultTip = pSearcher->GetLastResult();  //保留最后一次的异常信息
            }
            else
            {
                bShowUnexpectedResultTip = false;  //只要出现过一次搜索正常的结果，就不显示异常信息
            }

            if(!vecLyricInfo.empty())
            {
                LyricSearchResult SearchResult;
                SearchResult.vecLyricInfoTotal = vecLyricInfo;
                //SearchResult.strUnexpectedResultTip = strResultTip;
                SearchResult.bShowUnexpectedResultTip = false;			//遍历每一个搜索器的过程都不显示异常信息
                SearchResult.bCurrentSearchDone = false;					//搜索还没结束
                SearchResult.bAppendToList = (nTotalCount==0 ? false: true); //之前还没发过数据则需要清空列表将bAppendToList 设为false
                nTotalCount += vecLyricInfo.size();

                emit lyricResultChanged(SearchResult);
            }
        }

        //发送最后一次信号，结束搜索
        LyricSearchResult SearchResult;
        SearchResult.bCurrentSearchDone = true;							//标记搜索结束
        SearchResult.bAppendToList = nTotalCount == 0 ? false:true;//这里显示异常信息说明没有获取到任何数据，清空列表；否则，不清空列表
        SearchResult.vecLyricInfoTotal = QVector<LyricInfo>();				//发送空的数据回去
        SearchResult.bShowUnexpectedResultTip = bShowUnexpectedResultTip;	//是否显示异常信息
        SearchResult.strUnexpectedResultTip = strResultTip;				//异常信息
        emit lyricResultChanged(SearchResult);

        {
           QMutexLocker locker(&mutex);
           m_bIsSearching = false; //退出搜索状态
        }

    }

    QMutex mutex;
    bool m_bIsSearching;
    QString m_strArtist;
    QString m_strSong;
};


#endif // THREADSEARCHLYRIC_H
