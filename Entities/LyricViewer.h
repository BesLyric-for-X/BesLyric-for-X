#ifndef LYRIC_VIEWER_H
#define LYRIC_VIEWER_H

#include "global.h"
#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QVector>
#include <QPair>
#include <QFontMetrics>

#include "BesScaleUtil.h"

class LyricPanel : public QWidget
{
    Q_OBJECT
public:
    LyricPanel(QWidget* parent):QWidget(parent)
    {
        bIsLrcLyric = false;
        lrcLyrics.clear();
        currentLine = -1;

        QColor lightWhite("#999999");
        normalColor = lightWhite;
    }

    int getHeigth()
    {
        int height = 300* BesScaleUtil::scale();
        int actualHeight = lrcLyrics.size() * 45;

        return height > actualHeight ? height : actualHeight;
    }

    void setLyric(QVector<QPair<int, QString>> lyrics, bool isLrcLyric)
    {
        if(lyrics.size() == 0)
        {
            lrcLyrics.clear();
            lrcLyrics.push_back(QPair<int,QString>(0,""));
            lrcLyrics.push_back(QPair<int,QString>(0,""));
            lrcLyrics.push_back(QPair<int,QString>(0,""));
            lrcLyrics.push_back(QPair<int,QString>(0,"          没有可显示的歌词"));
            lrcLyrics.push_back(QPair<int,QString>(0,""));
            lrcLyrics.push_back(QPair<int,QString>(0,""));
            lrcLyrics.push_back(QPair<int,QString>(0,""));
            lrcLyrics.push_back(QPair<int,QString>(0,""));
        }
        else
        {
            lrcLyrics = lyrics;

            int lastLineTime = lrcLyrics.last().first;

            lrcLyrics.push_back(QPair<int,QString>(lastLineTime+6000000,""));
            lrcLyrics.push_back(QPair<int,QString>(lastLineTime+6000000,"歌词贡献者：勤劳的云村村民"));

        }
        bIsLrcLyric = isLrcLyric;
        currentLine = -1;
    }

    int getTotalLine()
    {
        return lrcLyrics.size();
    }

signals:
    void lineChanged(int lineNum);

public slots:
    void higthLineLyricAtPos(int pos)
    {
        int nSel =lrcLyrics.size();
        for(int i = 0; i < lrcLyrics.size(); i++)
        {
            if(pos < lrcLyrics[i].first)
            {
                nSel = i-1;
                break;
            }
        }

        if(nSel != currentLine)
        {
            currentLine = nSel;
            emit lineChanged(currentLine);
            update();
        }
    }

    void switchColor(bool bBlackStyle)
    {
        if(bBlackStyle)
            normalColor = QColor("#999999");
        else
            normalColor = QColor("#343434");
    }

protected:
    virtual void paintEvent(QPaintEvent *e)
    {
        QPainter painter(this);

        QFont font;
        font.setPixelSize(BesScaleUtil::fontSizeScale(17));
        font.setWeight(BesScaleUtil::fontSizeScale(61));
        font.setFamily("Microsoft YaHei");
        painter.setFont(font);

        QRect outerRect(0,0,this->width()-1,this->height()-1);
        //painter.fillRect(outerRect, QBrush("#00000000"));                   //绘制背景颜色

        for(int i = 0; i< lrcLyrics.size();i++)
        {
            normalColor.setAlpha(180);
            if(i == currentLine)
                painter.setPen("#ffffff");
            else
                painter.setPen(normalColor);

            QFontMetrics fm(painter.font());
            QRect rec = fm.boundingRect( lrcLyrics.at(i).second);

            int height = 30;
            if(rec.width() > 490* BesScaleUtil::scale())
                height += 30;

            QRectF lineRect;
            lineRect.setRect(2, (i) * 45,
                             496* BesScaleUtil::scale(),height* BesScaleUtil::scale());
            painter.drawText(lineRect,lrcLyrics.at(i).second);

        }

        QWidget::paintEvent(e);
    }

private:
    int currentLine;

    QColor normalColor;

    bool bIsLrcLyric;
    QVector<QPair<int, QString>> lrcLyrics;
};

class ScrollLyricPanel :public QWidget
{
    Q_OBJECT
public:
    ScrollLyricPanel(QWidget* parent):QWidget(parent)
    {
        lyricPanel = new LyricPanel(this);
        int height = lyricPanel->getHeigth();
        lyricPanel->setObjectName("lyricPanel");
        lyricPanel->setMinimumWidth(500* BesScaleUtil::scale());
        lyricPanel->setMinimumHeight(height);
        lyricPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        scrollArea = new QScrollArea(this);
        scrollArea->setObjectName("ScrollLyricPanelScrollArea");
        //scrollArea->setStyleSheet("QScrollArea{background-color:#00000000;}");

        scrollArea->setWidget(lyricPanel);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);

        scrollbar = scrollArea->verticalScrollBar();

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(scrollArea);

        scrollTimer = new QTimer(this);
        scrollTimer->setInterval(1);

        nMin = 0;
        nMax = 0;

        connect(scrollbar, &QScrollBar::rangeChanged,[=](int min,int max){
            nMin = min;
            nMax = max;
        });

        connect(lyricPanel, &LyricPanel::lineChanged,[=](int currentLine){

            int pageStep = scrollbar->pageStep();       //一页的大小
            int totalLine = lyricPanel->getTotalLine();
            double percent = currentLine * 1.0 / totalLine;
            double linePos = int((nMax - nMin + pageStep) * percent) + nMin;

            //头中尾定位调整
            if(linePos < pageStep/2 - 45)//头
                nTargetPos = 0;
            else if(linePos > nMax + pageStep/2) //尾
                nTargetPos = nMax;
            else
                nTargetPos = linePos - pageStep/2 + 45;

            scrollToTargetPos();
        });

        connect(scrollTimer, SIGNAL(timeout()), this, SLOT(srcollTimerTimeout()));
    }

    void updateLyric(QVector<QPair<int, QString>> lyrics, bool isLrcLyric)
    {
        lyricPanel->setLyric(lyrics, isLrcLyric);

        int height = lyricPanel->getHeigth();
        lyricPanel->setMinimumHeight(height);


    }


private:

    void scrollToTargetPos()
    {
        if(!scrollTimer->isActive())
            scrollTimer->start();
    }

private slots:
    void srcollTimerTimeout()
    {
        int minStep = 10;
        int maxStep = 1000;

        int current = scrollbar->value();

        if(current == nTargetPos)
            scrollTimer->stop();
        else
        {
            if(current < nTargetPos)
            {
                int step = (nTargetPos - current)*3/4;
                if(step > maxStep)
                    step = maxStep;
                if(step < minStep)
                    step = minStep;

                current+=step;
                if(current > nTargetPos)
                    current = nTargetPos;
            }
            else
            {
                int step = (current - nTargetPos)*3/4;
                if(step > maxStep)
                    step = maxStep;
                if(step < minStep)
                    step = minStep;

                current-=step;
                if(current < nTargetPos)
                    current = nTargetPos;
            }

            scrollbar->setValue(current);
        }
    }


public:
    LyricPanel*         lyricPanel;
    QScrollArea *       scrollArea;
    QScrollBar*         scrollbar ;

    QTimer*             scrollTimer;        //滚动用的定时器

    int nMin;
    int nMax;

    int nTargetPos;
};


//歌词查看类
class LyricViewer: public QWidget
{
    Q_OBJECT
public:
    LyricViewer(QWidget* parent = nullptr);

    void initEntity();
    void initLayout();
    void initConnection();

    bool isLyricValid();            //歌词是否有效(路径不存在或者为空都是无效)
    bool isLrcLyric();              //歌词是否是LRC歌词

public slots:
    void setMusicTitle(QString title);
    void setMusicPath(QString path);
    void setLrcLyricPath(QString path);

    void setEmptyLyric();           //设置空的歌词


    void  skinNameChanged(QString skinName);

private:
    bool loadLyricFromFile(QString lyricPath);


private:
    QVector<QPair<int, QString>> lrcLyrics;
    bool                        bIsLrcLyric;
    bool                        bIsLyricValid;
    bool                        bIsNeteaseLrcFormat;             //标记是否是网易云标准格式            

public:
    QLabel                    *labelPreviewTitle;               //歌曲标题
    QLabel                    *labelPreviewSongTip;
    QLabel                    *labelPreviewLyricTip;

    QLabel                    *labelPreviewSongPath;
    QLabel                    *labelPreviewLyricPath;

    ScrollLyricPanel          *scrollLyricPanel;    //歌词滚动面板


};


#endif //LYRIC_VIEWER_H
