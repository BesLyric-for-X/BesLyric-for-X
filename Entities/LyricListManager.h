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

    //辅助变量，方便拖动排序的计算，以及在拖动后识别哪些项在拖动时被选中
    bool selected = false;//标记是否需要被取出
    bool target = false;  //标记是否是移动的目标位置（放置在目标之前）
};

class LyricList
{
public:
    QString name;
    QString albumCoverPath;
    QVector<LyricListItem> items;

    //辅助变量，用于异步获取专辑图片后，将图片匹配赋值到歌词单
    int id = 0;
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
