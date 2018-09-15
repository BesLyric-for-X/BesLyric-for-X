#ifndef LYRIC_LIST_MANAGER_H
#define LYRIC_LIST_MANAGER_H

#include "global.h"
#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class LyricListItem
{
public:
    QString song;
    QString lyric;
};

class LyricList
{
public:
    QString name;
    QVector<LyricListItem> items;
};

class LyricListData
{
public:
    LyricList listHistory;
    QVector<LyricList> listsCreated;
};


class LyricListManager:public QObject
{
    Q_OBJECT

    /* 单例访问 */
public:
    static LyricListManager& GetInstance();

    ~LyricListManager(){}

    LyricListData getLyricListData();
    bool saveLyricListData(LyricListData data);

private:
    LyricListManager():bDataLoaded(false){}
    void loadFromSettingDir();

    QString MakeSureConfigPathAvailable();  //确保配置路径可用，返回配置路径

private:
    bool bDataLoaded;
    LyricListData listData;
};


//class AppConfig: public QObject{

//    Q_OBJECT

//    /* 单例访问 */
//public:
//    static AppConfig& GetInstance();

//    ~AppConfig(){}

//private:
//    AppConfig():m_hasLoad(false){}

//public:
//    //获得参数 (如果未加载自动去文件读取，已加载则使用已载入的参数。bForceLoadFromFile 强制从文件获得最新参数)
//    bool GetAppParameter(AppConfigParameter& param, bool bForceLoadFromFile = false);
//    bool SetAppParameter(const AppConfigParameter&  param);                             //设置参数

//    bool IsValidOnlyShowOneImage();         //判断当前配置是否是有效的“只显示一张图片”
//private:

//    //从默认路径中加载打印参数
//    bool LoadAppConfig();

//    //解析config
//    void parseConfig(QXmlStreamReader &reader, AppConfigParameter &param);

//    //解析user
//    void parseUser(QXmlStreamReader &reader, AppConfigParameter &param);

//    //解析default
//    void parseDefault(QXmlStreamReader &reader, AppConfigParameter &param);

//    //保存参数
//    bool SaveAppConfig();

//    bool MakeSureConfigPathAvailable(QString &path);  //确保配置路径可用，返回配置路径


//private:
//    AppConfigParameter m_param;             //应用参数
//    bool               m_hasLoad;           //标记是否已经加载参数
//};




#endif // LYRIC_LIST_MANAGER_H
