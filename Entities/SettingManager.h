#ifndef SettingManager_H
#define SettingManager_H

#include "global.h"
#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class SettingData
{
public:
    int shiftTime;

    QString defaultMusicPath;
    QString defaultLyricPath;
    QString defaultOutputPath;

    QString musicDowloadPath;
    bool agreeDownloadDeclaration;

    bool loginAnonymously;
    bool autoCheckForUpgrade;

    QString skinName;
    QString machineCode;        //存储上次获取的设备相关的唯一标志，在设备更换时，需要重置 agreeDownloadDeclaration 等设置

    SettingData()
    {
        shiftTime = 300;
        agreeDownloadDeclaration = false;
        loginAnonymously = false;
        autoCheckForUpgrade = true;
        skinName = "black";
        machineCode = "";
    }
};

class SettingManager:public QObject
{
    Q_OBJECT

    /* 单例访问 */
public:
    static SettingManager& GetInstance();
    ~SettingManager(){}

    void loadSettingData();
    bool saveSettingData();

    SettingData& data();

private:
    SettingManager(){}
    void loadFromDataDir();
    bool LoadSettingData(QString filePath);

//    bool parseAll(QXmlStreamReader &reader, LyricListData &data);
//    bool parseLyricList (QXmlStreamReader &reader,QVector<LyricList> &lists, QString parentName);

    QString MakeSureConfigPathAvailable();  //确保配置路径可用，返回配置路径

private:
    SettingData settingData;
};


#endif // SettingManager_H
