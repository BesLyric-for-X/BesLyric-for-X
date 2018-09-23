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
    LyricListItem(){}
    LyricListItem(QString song, QString lyric):song(song),lyric(lyric){}
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
    QVector<LyricList> listsHistory;  //只有1个项，为了方便和listsCreated一样的方式处理，也用QVector
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
    void loadFromDataDir();
    bool LoadListData(QString filePath);

    bool parseAll(QXmlStreamReader &reader, LyricListData &data);
    bool parseLyricList (QXmlStreamReader &reader,QVector<LyricList> &lists, QString parentName);

    QString MakeSureConfigPathAvailable();  //确保配置路径可用，返回配置路径

private:
    bool bDataLoaded;
    LyricListData listData;
};


#endif // LYRIC_LIST_MANAGER_H
