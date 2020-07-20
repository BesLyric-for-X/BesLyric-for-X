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
#include "Utility/ConfigurationFile.h"

enum NameFormatStyle{
    ARTIST_SONG = 0,
    SONG_ARTIST = 1
};

class SettingData
{
public:
    //设置页面的设置
    int shiftTime;

    QString defaultMusicPath;
    QString defaultLyricPath;
    QString defaultOutputPath;

    QString musicDowloadPath;
    bool agreeDownloadDeclaration;

    bool loginAnonymously;
    bool autoCheckForUpgrade;

    int nameFormatStyle;

    //程序运行过程用户操作保存
    QString machineCode;        //存储上次获取的设备相关的唯一标志，在设备更换时，需要重置 agreeDownloadDeclaration 等设置
    QString skinName;
    int volume;                 //音量
    bool isMute;                //是否静音
    int playMode;               //播放模式 【0 单曲播放】 【1 单曲循环】 【2 随机播放】【3 顺序播放】【4 列表循环】

    //运行时全局变量，不进行文件读写
    //...

    SettingData()
    {
        shiftTime = 300;
        agreeDownloadDeclaration = false;
        loginAnonymously = false;
        autoCheckForUpgrade = true;
        nameFormatStyle = ARTIST_SONG;

        machineCode = "";
        skinName = "black";
        volume = 128;           //最大值128
        isMute = false;
        playMode = 0;

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

public:

    static QString MakeSureBaseDataPathAvailable();

private:
    SettingManager():configurationData{ConfigurationFileNames::settingFileName}{}

    void loadFromDataDir();
    bool LoadSettingData(QString filePath);
    bool parseAll(QXmlStreamReader &reader);

    QString MakeSureConfigPathAvailable();  //确保配置路径可用，返回配置路径

    void preProcessSetting();//预处理设置
private:
    SettingData settingData;

    ConfigurationFile configurationData;
};


#endif // SettingManager_H
