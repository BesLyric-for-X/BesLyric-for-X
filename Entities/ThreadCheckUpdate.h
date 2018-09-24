#ifndef ThreadCheckUpdate_H
#define ThreadCheckUpdate_H

#include "Define/Static.h"
#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QMutex>
#include <BesMessageBox.h>
#include <QMutexLocker>
#include <QRegExp>
#include <QStringList>
#include "NetAccess.h"
#include "SettingManager.h"

class CheckUpgradeResult
{
public:
    int nResult;            //0 成功获取，但是数据不合法  1 成功获取，无更新； 2 成功获取，有更新； -1 网络连接失败
    QString versionNum;
    QStringList infoList;

    CheckUpgradeResult()
    {
        nResult = 1;
        versionNum = "";
        infoList.clear();
    }

    CheckUpgradeResult(int result)
    {
        nResult = result;
        versionNum = "";
        infoList.clear();
    }
};


class ThreadCheckUpdate :public QThread
{
    Q_OBJECT

public:
    ThreadCheckUpdate():m_bIsChecking(false), m_muteWhenNoUpdateFound(true)
    {

    }

    bool checkForUpdate(bool muteWhenNoUpdateFound ,Priority priority)
    {
        QMutexLocker locker(&mutex);

        if(m_bIsChecking)
        {
            BesMessageBox::information(tr("等待提示"),tr("正在检测中......"));
            return false;
        }

        m_muteWhenNoUpdateFound = muteWhenNoUpdateFound;

       start(priority);
       return true;
    }


signals:
    void sig_haveCheckResult(CheckUpgradeResult result);


protected:
    virtual void run()
    {
        bool mute;
        {
           QMutexLocker locker(&mutex);
           m_bIsChecking = true; //进入
           mute = m_muteWhenNoUpdateFound;
        }

        CheckUpdate(mute);

        {
           QMutexLocker locker(&mutex);
           m_bIsChecking = false; //退出
        }
    }


    void CheckUpdate(bool mute)
    {
        QString tempBuffer;
        bool bRet = NetworkAccess::SyncDownloadString(LINK_LAST_VERSION_INFO ,tempBuffer);
        if(bRet == false)
        {
            if(!mute)
                emit sig_haveCheckResult(CheckUpgradeResult(-1));

            return;
        }

        bool endWithN = tempBuffer.contains('\n');
        bool endWithR = tempBuffer.contains('\r');
        if(!endWithN && !endWithR)
        {
            if(!mute)
                emit sig_haveCheckResult(CheckUpgradeResult(0));
            return ;
        }

        CheckUpgradeResult result;

        QStringList list;

        if(endWithN)
        {
            list = tempBuffer.split('\n');
        }
        else
        {
            list = tempBuffer.split('\r');
        }

        for(int i= 0; i< list.size() ; i++)
        {
            if(i == 0)
                result.versionNum = list.at(i).trimmed();
            else
            {
                QString line = list.at(i).trimmed();
                if(line != "[update]")
                    result.infoList.push_back(list.at(i).trimmed());
            }
        }

        if(!isStringValidVersionNum(result.versionNum))
        {
            result.nResult = 0;

            if(!mute)
                emit sig_haveCheckResult(result);
            return;
        }

        if(isNewerVersion(result.versionNum))
        {
            result.nResult = 2;

            emit sig_haveCheckResult(result);       //有更新，无论如何都要发送
        }
        else
        {
            result.nResult = 1;

            if(!mute)
                emit sig_haveCheckResult(result);
        }



    }

private:
    bool isStringValidVersionNum(QString str)
    {
        int dotCount = 0;
        for(auto ch: str)
        {
            if(ch == '.')
                dotCount++;
             else if( !(ch>='0' &&ch<='9'))
                return false;
        }

        if(dotCount != 2)
            return false;

        if(str.split('.').size() != 3)
            return false;

        return true;
    }

    bool isNewerVersion(QString test)
    {
        QString current = VERSION_NUMBER;

        QStringList curList = current.split('.');
        QStringList testList = test.split('.');

        int nCur1 =  curList.at(0).toInt();
        int nCur2 =  curList.at(1).toInt();
        int nCur3 =  curList.at(2).toInt();
        int nTest1 =  testList.at(0).toInt();
        int nTest2 =  testList.at(1).toInt();
        int nTest3 =  testList.at(2).toInt();

        if(nTest1 > nCur1)
        {
            return true;
        }
        else if(nTest1 == nCur1)
        {
            if(nTest2 > nCur2)
            {
                return true;
            }
            else if(nTest2 == nCur2)
            {
                if(nTest3 > nCur3)
                {
                    return true;
                }
                else if(nTest3 == nCur3)
                {
                    return false;
                }
            }
        }

        return false;
    }

private:

    QMutex mutex;
    bool m_bIsChecking;
    bool m_muteWhenNoUpdateFound;            //静默
};


#endif // ThreadCheckUpdate
