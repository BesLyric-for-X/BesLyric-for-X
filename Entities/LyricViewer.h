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
    LyricPanel(QWidget* parent):QWidget(parent),fmText(QFont())
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

        int actualHeight = 0;
        for(auto height:heightList)
            actualHeight += height + lineGap;

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
            lrcLyrics.push_back(QPair<int,QString>(lastLineTime+6000000,""));
            //lrcLyrics.push_back(QPair<int,QString>(lastLineTime+6000000,"歌词贡献者：勤劳的云村村民"));
            //不额外添加歌词，之前添加后，引起用户误解
        }

        //获得行间隔，计算每一行歌词显示的高度
        {
            QRect textFrameRect(2,0,this->width()-1,this->height()-1);
            heightList.clear();

            //行间隔定为一倍的行高
            lineGap = fmText.boundingRect('T').height();

            for(int i = 0; i< lrcLyrics.size();i++)
            {
                QString text = lrcLyrics.at(i).second;
                QRect rect = fmText.boundingRect(textFrameRect, Qt::TextWrapAnywhere, text);
                heightList.push_back(rect.height());
            }
        }

        bIsLrcLyric = isLrcLyric;
        currentLine = -1;
    }

    //获得行的位置占整体控件高度的占比
    double getLinePositionPercent(int currentLine)
    {
        if(heightList.empty())
            return 0.0;
        if(currentLine >= heightList.size())
            return 1.0;

        int totalHeight = 0;
        int currentHeight = 0;
        for(int i = 0; i< heightList.size(); ++i)
        {
            totalHeight += heightList[i] + lineGap;
            if( i < currentLine )
                currentHeight = totalHeight;
        }

        return currentHeight * 1.0 / totalHeight;
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
        //先设置粗体，以让后续获得的 fmText 是基于粗体计算，从而避免高亮歌词显示不全问题（这里认为多空一行的空间可以接受）
        font.setBold(true);
        painter.setFont(font);

        //绘制背景颜色，方便调试时直观感知歌词面板位置和尺寸
        //QRect textFrameRect(0,0,this->width()-1,this->height()-1);
        //painter.fillRect(textFrameRect, QBrush("#4400ff00"));

        //获得绘制环境下的字体测量对象，用于在 setLyric 时计算歌词显示的高度
        fmText = QFontMetrics(painter.font());

        assert(heightList.size() == lrcLyrics.size());
        if(heightList.size() != lrcLyrics.size())
            return;

        //当前绘制的位置Y
        int currentY = 0;

        for(int i = 0; i< lrcLyrics.size();i++)
        {
            if(i == currentLine)
            {
                //加粗、使用不透明的白色
                font.setBold(true);
                painter.setFont(font);
                painter.setPen("#ffffff");
            }
            else
            {
                //不加粗、使用 180 透明度的 normalColor
                normalColor.setAlpha(180);

                font.setBold(false);
                painter.setFont(font);
                painter.setPen(normalColor);
            }

            int heightCurrent = heightList[i];

            //绘制的矩形框，和计算行高时的文字显示外框保持一致的宽度，textFrameRect(2,0,this->width()-1,this->height()-1);
            QRectF lineRect;
            lineRect.setRect(2, currentY, this->width()-1, heightCurrent);

            QString text = lrcLyrics.at(i).second;
            painter.drawText(lineRect, Qt::AlignCenter|Qt::TextWrapAnywhere, text);

            //累加当前行高和行间隔
            currentY += heightCurrent + lineGap;
        }

        QWidget::paintEvent(e);
    }

private:
    int currentLine;

    QColor normalColor;

    bool bIsLrcLyric;
    QVector<QPair<int, QString>> lrcLyrics;

    QVector<int> heightList;                //存储每一行的高度
    int lineGap;                            //定义行间隔

    QFontMetrics fmText;                    //存储从 paintEvent 中得到的字体测量对象
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
            double percent = lyricPanel->getLinePositionPercent(currentLine);

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

        connect(scrollTimer, &QTimer::timeout, this, &ScrollLyricPanel::srcollTimerTimeout);
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
