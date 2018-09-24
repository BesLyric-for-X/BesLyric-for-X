#ifndef ThreadLogin_H
#define ThreadLogin_H

#include "Define/Static.h"
#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QRegExp>
#include <QStringList>
#include "NetAccess.h"
#include "SettingManager.h"

class ThreadLogin :public QThread
{
    Q_OBJECT

public:
    ThreadLogin(){}

    bool SendLogin()
    {
       m_bAnonymity = SettingManager::GetInstance().data().loginAnonymously;

       start(NormalPriority);
       return true;
    }


protected:
    virtual void run()
    {
        SendLoginInfo(m_bAnonymity);
    }


    //发送登录信息（ip地址）
    void SendLoginInfo(bool bAnonymity)
    {
        //获得ip地址的网页源，2个备用
        const int SrcCount = 2;
        QString ipSrc[SrcCount]={
            "http://ip.qq.com/",
            "https://whatismyipaddress.com/",
        };

        //单个源最大检测ip的次数
        int nMaxSingleCheckCount = 5;

        QString strIP= "";
        QString tempBuffer;

        if(bAnonymity)
        {
            strIP = "Anonymity";
        }
        else
        {
            int nLeftCheckCount = nMaxSingleCheckCount * SrcCount; // 计算总共检测次数
            while(nLeftCheckCount-1)
            {
                bool bRet = NetworkAccess::SyncDownloadString(
                            ipSrc[(nLeftCheckCount-1) / nMaxSingleCheckCount],tempBuffer);

                if(bRet == false)
                {
                    //可能没网络，或网络异常，也可能读取文件失败
                    //等待5秒再检测
                    nLeftCheckCount--;
                    sleep(5);
                    continue;
                }

                // https://whatismyipaddress.com/ 和 http://ip.qq.com/ 的网页中IP都是 >ddd.ddd.ddd.ddd< 特征，查找该特征的字符串

                CatchIPStr(tempBuffer, strIP);

                if(strIP.size() != 0)
                    break;//已经获得ip，退出循环

                nLeftCheckCount--;
            }
        }

        if(strIP.size() == 0)
            strIP = "unknown";

        //访问链接，服务端负责记录登录信息
        QString strSendLink = LINK_SEND_LOGIN + "?ip=" + strIP + "&version=" + VERSION_NAME + "&vernum=" +VERSION_NUMBER;
        NetworkAccess::SyncDownloadString(strSendLink, tempBuffer);
    }


    //俘获满足需求的IP字符串
    bool CatchIPStr( QString &buffer, QString& ip)
    {
        // > . . . <

        QRegExp rx(">(\\d+\\.\\d+\\.\\d+\\.\\d+)<");

        QStringList matchedList;  //所有匹配的地址列表

        int textPos = 0; //非标签文本开始的位置

        //尝试匹配时间标签
        int pos = 0;
        while ((pos = rx.indexIn(buffer, pos)) != -1) {
            matchedList << rx.cap(1);
            pos += rx.matchedLength();

            textPos = pos;
        }

        if(matchedList.size() == 0)
            return false;
        else
        {
            ip = matchedList.at(0);
            return true;
        }
    }

private:
    bool m_bAnonymity;
};


#endif // ThreadLogin_H
