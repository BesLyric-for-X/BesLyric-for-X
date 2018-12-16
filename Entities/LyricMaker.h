#ifndef LYRIC_MAKER_H
#define LYRIC_MAKER_H

#include "global.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QTextCodec>

class LyricMaker:public QObject
{
    Q_OBJECT
public:
    LyricMaker(QObject* parent = nullptr):QObject(parent),insertOffsetTime(0){}

    //歌词是否为空
    bool isRawLyricEmpty();

    bool saveLyrc(QString savePath);

public slots:

    //载入原始的歌词数据
    bool loadRawLyric(QString lyricPath);


    void startMaking();                         //开始制作歌词

    bool hasPreLrcLine();                       //是否有前一个LRC行
    bool hasPPreLrcLine();                      //是否有前二个LRC行
    bool hasNextRawLine();                      //是否有下一个原歌词行
    bool hasNNextRawLine();                     //是否有后一个原歌词行

    bool getPreLrcLineText(QString& line);         //当前行为第一行时，将返回 false
    bool getPPreLrcLineText(QString& line);        //当前行为第一二行时，将返回 false
    bool getCurrentLrcLineText(QString& line);     //文档为空时，将返回 false
    bool getCurrentRawLineText(QString& line);     //文档为空时，将返回 false
    bool getNextRawLineText(QString& line);        //当前行为最后一行时，将返回 false
    bool getNNextRawLineText(QString& line);       //当前行为最后二行时，将返回 false

    bool updateCurrentLineText(QString& line);      //更新当前行的歌词内容

    bool markCurrentRawLine(quint64 time);         //标记当前行为 time
    bool markEmptyLine(quint64 time);              //标记时间为 time 的空行
    bool stepBackToTime(quint64 time);             //回退到 time

    int getLastLrcLineTime();                       //获得上一行LRC歌词的时间，没有上一行时返回-1

    void finishMaking();                            //结束制作
    bool isResultLrcEmpty();                        //制作结束的lrc是否为空

    void getLyricData(QVector<QString>& _rawLines, QVector<QPair<quint64, QString>>& _lrcLines);
    void updateLyricData(QVector<QString>& _rawLines, QVector<QPair<quint64, QString>>& _lrcLines);
private:
    QVector<QString> rawLines;                          //原始歌词
    QVector<QPair<quint64, QString>> lrcLines;          //lrc歌词

    uint rawCurrent;                                    //原始歌词当前下标
    uint lrcNext;                                       //lrc歌词当前的下一个下标

    QString lrcContent;                                 //最终得到的lrc歌词内容

    int insertOffsetTime;                           //插入歌词时提前偏移的时间值
};



#endif //LYRIC_MAKER_H
