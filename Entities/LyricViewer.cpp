#include "LyricViewer.h"
#include "global.h"
#include <QFileInfo>
#include <QRegExp>
#include <BesMessageBox.h>
#include <QSpacerItem>

LyricViewer::LyricViewer(QWidget* parent):QWidget(parent)
{
    setMouseTracking(true);

    initEntity();
    initLayout();
    initConnection();
}


void LyricViewer::initEntity()
{
    lrcLyrics.clear();
    bIsLrcLyric = false;
    bIsLyricValid = false;
    bIsNeteaseLrcFormat = true;
}

void LyricViewer::initLayout()
{
    labelPreviewTitle = new QLabel(this);
    labelPreviewSongTip= new QLabel(this);
    labelPreviewLyricTip= new QLabel(this);
    labelPreviewTitle->setObjectName("labelPreviewTitle");
    labelPreviewSongTip->setObjectName("labelPreviewSongTip");
    labelPreviewLyricTip->setObjectName("labelPreviewLyricTip");
    labelPreviewTitle->setText("");
    labelPreviewSongTip->setText("歌曲路径:");
    labelPreviewLyricTip->setText("歌词路径:");

    labelPreviewSongTip->setMaximumSize(80,30);
    labelPreviewLyricTip->setMaximumSize(80,30);
    labelPreviewSongTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelPreviewLyricTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    labelPreviewSongPath = new QLabel(this);
    labelPreviewLyricPath = new QLabel(this);
    labelPreviewSongPath->setObjectName("labelPreviewSongPath");
    labelPreviewLyricPath->setObjectName("labelPreviewLyricPath");
    labelPreviewSongPath->setText("无");
    labelPreviewLyricPath->setText("无");

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelPreviewSongTip);
    hLayout1->addWidget(labelPreviewSongPath);

    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(labelPreviewLyricTip);
    hLayout2->addWidget(labelPreviewLyricPath);

    scrollLyricPanel = new ScrollLyricPanel(this);
    scrollLyricPanel->setObjectName("scrollLyricPanel");
    scrollLyricPanel->setMinimumSize(548,500);
    scrollLyricPanel->setMaximumSize(548,500);
    scrollLyricPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addWidget(labelPreviewTitle);
    vLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout1);
    vLayout->addSpacerItem(new QSpacerItem(10,5,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Fixed));
    vLayout->addWidget(scrollLyricPanel);
}

void LyricViewer::initConnection()
{



}

//歌词是否有效(路径不存在或者为空都是无效)
bool  LyricViewer::isLyricValid()
{
    return bIsLyricValid;
}

//歌词是否是LRC歌词
bool LyricViewer::isLrcLyric()
{
    return bIsLrcLyric;
}

void LyricViewer::setMusicTitle(QString title)
{
    labelPreviewTitle->setText(title);
}

void LyricViewer::setMusicPath(QString path)
{
    QFileInfo fileinfo(path);
    setMusicTitle(fileinfo.baseName());     //先使用歌曲的文件的名字，来设置音乐标题，如果播放时，从歌曲中发现了标题，那么再由外界设置新标题

    labelPreviewSongPath->setText(path);
}

void LyricViewer::setLrcLyricPath(QString path)
{
    labelPreviewLyricPath->setText(path);

    //使用该路径初始化歌词预览面板
    if(!loadLyricFromFile(path))
        BesMessageBox::information(tr("提示"),tr("载入lrc歌词时发生错误：")+path);

    scrollLyricPanel->updateLyric(lrcLyrics, bIsLrcLyric);
}

//设置空的歌词
void LyricViewer::setEmptyLyric()
{
    labelPreviewLyricPath->setText(tr("无"));

    lrcLyrics.clear();
    scrollLyricPanel->updateLyric(lrcLyrics, true);

    bIsLyricValid = false;
}


void LyricViewer::skinNameChanged(QString skinName)
{
    if(skinName == "black")
        scrollLyricPanel->lyricPanel->switchColor(true);
    else
       scrollLyricPanel->lyricPanel->switchColor(false);
}


bool LyricViewer::loadLyricFromFile(QString lyricPath)
{
    lrcLyrics.clear();

    //先读取所有文本
    QVector<QString> lines;

    bIsLyricValid = false;
    bIsLrcLyric = false;
    bIsNeteaseLrcFormat = true;

    QFile file(lyricPath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString strline = QString(line).trimmed();

        if(strline.size() != 0)
            lines.push_back(strline);   //收集非空行
    }

    file.close();

    //进一步判断是否为LRC歌词
    if(lines.size() != 0)
    {
        bIsLyricValid = true;


        QVector<QPair<int, QString>> lrcLyricsTemp; //临时存放初步收集的可能无序的结果

        //这里判断的标准以 比较广泛的方式支持读取进来

        //网易云标准上传的歌词格式只有 带时间标签的行，而且一行一个时间 [xx:xx.xx]  xxx

        //广泛的lrc格式，可能包含 [ar:歌手名]、[ti:歌曲名]、[al:专辑名]、[by:编辑者(指lrc歌词的制作人)]、[offset:时间补偿值]
        //                    时间标签，形式为“[mm:ss]”或“[mm:ss.ff]”
        //                  一行可能包含多个时间标签

        QStringList infoLabels;     //储存非歌词行的信息 [ar:歌手名] 、[offset:时间补偿值] 等

        QRegExp rx_time("(\\[\\s*\\d+\\s*:\\s*\\d+(\\.\\d+)?\\s*\\])");  //匹配时间标签
        QRegExp rx_other("(\\[\\s*\\w+\\s*:\\s*\\w+\\s*\\])");  //匹配其他标签
        for(auto& line:lines)
        {
            QStringList timeList;

            int textPos = 0; //非标签文本开始的位置

            //尝试匹配时间标签
            int pos = 0;
            while ((pos = rx_time.indexIn(line, pos)) != -1) {
                timeList << rx_time.cap(1);
                pos += rx_time.matchedLength();

                textPos = pos;
            }

            //尝试匹配其他标签
            int pos2 = 0;
            while ((pos2 = rx_other.indexIn(line, pos2)) != -1) {
                infoLabels << rx_other.cap(1);
                pos2 += rx_other.matchedLength();

                textPos = pos2;
            }

            QString strLeft; //剩下的字符串
            strLeft = line.right(line.size()-textPos);   //得到去除可能的标签后的字符串

            for(auto& time: timeList) //只有当收集到时间标签时，才收集对应的歌词进入 lrcLyricsTemp
            {
                //将时间标签转化为 毫秒时间
                int p1 = time.indexOf('[');

                time = time.right(time.size()-p1-1);
                int p2 = time.indexOf(':');

                QString strMin = time.left(p2);

                time = time.right(time.size()-p2-1);
                int p3 = time.indexOf(']');

                QString strSec = time.left(p3);

                int min = strMin.toInt();
                double sec = strSec.toDouble();

                int msec = min * 60 * 1000 + int(sec * 1000);

                lrcLyricsTemp.push_back(QPair<int,QString>(msec,strLeft));
            }

            if(timeList.size() > 1)  //一行包含多个时间，认为不是标准的网易云音乐歌词
                bIsNeteaseLrcFormat = false;
        }

        if(lrcLyricsTemp.size() != 0)
        {
            bIsLrcLyric = true; //认为是lrc歌词

            //对收集到的 lrcLyricsTemp 根据时间，从小到大排序，置于  lrcLyrics 中

            while(lrcLyricsTemp.size()!=0)
            {
                int minTime = lrcLyricsTemp.front().first;
                auto iterDel = lrcLyricsTemp.begin();           //存放最小的要被删除的那个
                for(auto iter = lrcLyricsTemp.begin()+1; iter != lrcLyricsTemp.end(); iter++)
                {
                    if(iter->first < minTime)
                    {
                        iterDel = iter;
                        minTime = iter->first;
                    }
                }

                lrcLyrics.push_back(*iterDel);

                lrcLyricsTemp.erase(iterDel);
            }

            if(infoLabels.size() != 0)//包含时间标签外的其他类型标签，认为不是标准的网易云音乐歌词
                bIsNeteaseLrcFormat = false;

            //查看 infoLabels中是否存在  [offset:时间补偿值]，存在则修正时间
            bool bExistOffset = false;
            int offsetValue = 0;
            for(auto label:infoLabels)
            {
                if(label.indexOf("offset")!= -1)
                {
                    int pos = label.indexOf(':');
                    if(pos == -1)
                        continue;  //无效标签，继续

                    label = label.right(label.size() - pos -1);

                    pos = label.indexOf(']');
                    label = label.left(pos);

                    offsetValue = label.toInt();
                    bExistOffset = true;
                }
            }

            if(bExistOffset)
            {
                for(auto& lrc: lrcLyrics)
                {
                    lrc.first += offsetValue;
                    if(lrc.first < 0)
                        lrc.first = 0;
                }
            }


        }
        else
        {
            //找不到任何含有时间标签的行，认为是原生歌词
            bIsLrcLyric = false;

            //收集歌词到 lrcLyric 结构中，时间都置为 0
            for(auto& line:lines)
            {
                lrcLyrics.push_back(QPair<int,QString>(0,line));
            }
        }
    }

    return true;
}

