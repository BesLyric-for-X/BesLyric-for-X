#include "LyricMaker.h"
#include "global.h"
#include <QTextStream>
#include <QStringList>
#include <UnicodeReader.h>
#include "SettingManager.h"

//载入原始的歌词数据
bool LyricMaker::loadRawLyric(QString lyricPath)
{
    rawLines.clear();

    QString content;
    UnicodeReader unicodeReader;
    if(!unicodeReader.ReadFromFile(lyricPath,content))
        return false;

    QRegExp sepRegExp = QRegExp("\n|\r");               //linux\mac\windows 换行符号
    QStringList lineList = content.split(sepRegExp);

    for(auto& line: lineList)
    {
        line = line.trimmed();
        if(!line.isEmpty())
            rawLines.push_back(line.trimmed());
    }

    return true;
}

//歌词是否为空
bool LyricMaker::isRawLyricEmpty()
{
    return rawLines.empty();
}

bool LyricMaker::saveLyrc(QString savePath)
{
    QFile file(savePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text |QIODevice::Truncate))
    {
        return false;
    }

    QTextStream streamFileOut(&file);
    streamFileOut.setCodec("UTF-8");
    streamFileOut << lrcContent;
    streamFileOut.flush();

    streamFileOut.setGenerateByteOrderMark(false);

    file.close();

    return true;
}


//开始制作歌词
void LyricMaker::startMaking()
{
    insertOffsetTime = SettingManager::GetInstance().data().shiftTime;  //现在开始制作的时候读取偏移设置

    rawCurrent = 0;
    lrcNext = 0;

    //初始化lrc歌词内容
    lrcLines.clear();
    lrcContent.clear();
}

//是否有前一个LRC行
bool LyricMaker::hasPreLrcLine()
{
    if(isRawLyricEmpty())
        return false;

    if(lrcNext <= 1)
        return false;

    return true;
}

//是否有前二个LRC行
bool LyricMaker::hasPPreLrcLine()
{
    if(isRawLyricEmpty())
        return false;

    if(lrcNext <= 2)
        return false;

    return true;
}

 //是否有下一个原歌词行
bool LyricMaker::hasNextRawLine()
{
    if(isRawLyricEmpty())
        return false;

    if(rawCurrent + 1 > uint(rawLines.size()))
        return false;

    return true;
}

//是否有后一个原歌词行
bool LyricMaker::hasNNextRawLine()
{
    if(isRawLyricEmpty())
        return false;

    if(rawLines.size()<2)
        return false;

    if(rawCurrent + 2 > uint(rawLines.size()))
        return false;

    return true;
}

//当前行为第一行时，将返回 false
bool LyricMaker::getPreLrcLineText(QString& line)
{
    if(!hasPreLrcLine())
        return false;

    line = lrcLines[lrcNext-2].second;

    return true;
}

//当前行为第一二行时，将返回 false
bool LyricMaker::getPPreLrcLineText(QString& line)
{
    if(!hasPPreLrcLine())
        return false;

    line = lrcLines[lrcNext-3].second;

    return true;
}

 //文档为空时，将返回 false
bool LyricMaker::getCurrentLrcLineText(QString& line)
{
    if(lrcNext <= 0)
        return false;

    line = lrcLines[lrcNext-1].second;

    return true;
}

//文档为空时，将返回 false
bool LyricMaker::getCurrentRawLineText(QString& line)
{
    if(isRawLyricEmpty())
        return false;

    if(rawCurrent == 0)
        return false;

    line = rawLines[rawCurrent-1];

    return true;
}

//当前行为最后一行时，将返回 false
bool LyricMaker::getNextRawLineText(QString& line)
{
    if(!hasNextRawLine())
        return false;

    line = rawLines[rawCurrent];

    return true;
}

//当前行为最后二行时，将返回 false
bool LyricMaker::getNNextRawLineText(QString& line)
{
    if(!hasNNextRawLine())
        return false;

    line = rawLines[rawCurrent+1];

    return true;
}

//更新当前行的歌词内容
bool LyricMaker::updateCurrentLineText(QString& line)
{
    QString text;
    if(getCurrentLrcLineText(text))
    {
        lrcLines[lrcNext-1].second = line;
        rawLines[rawCurrent-1] = line;
    }
    else
        return false;  //没有当前行可更新
}

//标记当前行为 time
bool LyricMaker::markCurrentRawLine(quint64 time)
{
    time -= insertOffsetTime;
    lrcLines.push_back(QPair<quint64, QString>(time,rawLines[rawCurrent]));

    rawCurrent++;
    lrcNext++;

    return true;
}

//标记时间为 time 的空行
bool LyricMaker::markEmptyLine(quint64 time)
{
    //只有当lrc 歌词为空，或者不为空时最后一个元素的内容不为空时，才插入一个空的时间行
    if(lrcLines.isEmpty() || lrcLines.last().second != "")
    {
        time -= insertOffsetTime;
        lrcLines.push_back(QPair<quint64, QString>(time,""));
        lrcNext++;
    }

    return true;
}

//回退到 time
bool LyricMaker::stepBackToTime(quint64 time)
{
    //从下标为 0 开始遍历到 lrcNext, 知道找到 比 time 大的，其下标即为跳转后下标
    for(uint i = 0; i< lrcNext; i++)
    {
        if(lrcLines[i].first > time)
        {
            lrcNext = i;        // i 对应的时间比 指定时间大，所以 i 为跳转后的当前下标
            break;
        }
    }

    //删除 lrcNext 后面可能有的 lrcLines 的项
    uint index = 0;
    for(auto iter = lrcLines.begin(); iter != lrcLines.end(); iter++, index++)
    {
        if(index == lrcNext)//index 到达 lrcNext,则 接下来的 的元素都是要删除的
        {
            while(iter != lrcLines.end())
            {
               iter = lrcLines.erase(iter);
            }

            break; //删除后跳出
        }
    }

    //重新纠正 rawCurrent 的 值，计算当前 lrcLines 有多少非空行，即为 rawCurrent的值
    uint count = 0;
    for(auto& line:lrcLines)
    {
        if(line.second != "")
            count++;
    }

    rawCurrent = count;

    return true;
}

//获得上一行LRC歌词的时间，没有上一行时返回-1
int LyricMaker::getLastLrcLineTime()
{
    if(!hasPreLrcLine())
        return -1;

    return int(lrcLines[lrcNext-2].first);
}

//结束制作
void LyricMaker::finishMaking()
{
    lrcContent.clear();

    for(auto& line:lrcLines)
    {
        quint64 time = line.first;
        int ms = time % 1000;
        time = time/1000;
        int s = time % 60;
        int m = time/60;

        QString timeLabel;
        timeLabel.sprintf("%.2d:%.2d.%.3d",m, s, ms);

        QString oneline = "["+timeLabel+"]"+line.second + "\n";
        lrcContent.append(oneline);
    }
}

//制作结束的lrc是否为空
bool LyricMaker::isResultLrcEmpty()
{
    return lrcContent.size() == 0;
}

