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
    //设置页面的设置
    int shiftTime;

    QString defaultMusicPath;
    QString defaultLyricPath;
    QString defaultOutputPath;

    QString musicDowloadPath;
    bool agreeDownloadDeclaration;

    bool loginAnonymously;
    bool autoCheckForUpgrade;

    //程序运行过程用户操作保存
    QString machineCode;        //存储上次获取的设备相关的唯一标志，在设备更换时，需要重置 agreeDownloadDeclaration 等设置
    QString skinName;
    int volume;                 //音量
    bool isMute;                //是否静音
    int playMode;               //播放模式 0 单曲 播放 1 单曲循环

    //运行时全局变量，不进行文件读写
    int flagStopAutoPlaying;    //用于在制作歌词时，阻止音频结束后的任何自动播放，当 flag = 1 时，将阻止任何自动播放
                                //相关逻辑全局搜索：【flag_play_mode】

    SettingData()
    {
        shiftTime = 300;
        agreeDownloadDeclaration = false;
        loginAnonymously = false;
        autoCheckForUpgrade = true;
        machineCode = "";
        skinName = "black";
        volume = 128;           //最大值128
        isMute = false;
        playMode = 0;

        flagStopAutoPlaying = 0;
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
    bool parseAll(QXmlStreamReader &reader);

    QString MakeSureConfigPathAvailable();  //确保配置路径可用，返回配置路径

    void preProcessSetting();//预处理设置
private:
    SettingData settingData;
};


#endif // SettingManager_H
