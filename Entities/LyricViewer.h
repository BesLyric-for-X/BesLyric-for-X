#ifndef LYRIC_VIEWER_H
#define LYRIC_VIEWER_H

#include "global.h"
#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPainter>
#include <QLabel>

#include <QPixmap>
#include <QTimer>

class LyricPanel : public QWidget
{
public:
    LyricPanel(QWidget* parent):QWidget(parent)
    {
       list <<  " 作曲 : 王力宏\n"
            <<  "作词 : 王力宏/陳信延\n"
            <<  "已经听了一百遍 怎么听都不会倦\n"
            <<  "从白天唱到黑夜 你一直在身边（一直在身边）\n"
            <<  "如果世界太危险 只有音乐最安全  带着我进梦里面 让歌词都实现\n"
            <<  "无论是开心还是难过我的爱一直不变\n"
            <<  "（不必担心时间流逝带走一切）\n"
            <<  "无论是HIP-HOP还是摇滚我的爱一直不变\n"
            <<  "（所有美好回忆记录在里面）\n"
            <<  "这种Forever Love 那么深\n"
            <<  "我们的歌 那么真\n"
            <<  "无国界 跨时代\n"
            <<  "再不会叫我KISS GOODBYE\n"
            <<  "要每一句能够动人心弦  YE~~\n"
            <<  "情人总分分合合\n"
            <<  "可是我们却越爱越深\n"
            <<  "认识你让我的幸福如此  悦耳\n"
            <<  "能不能不要切歌  继续唱我们的歌\n"
            <<  "让感动一辈子都记得\n"
            <<  "en heng~~~\n"
            <<  "wo~  o~\n"
            <<  "已经听了一百遍 怎么听都不会倦\n"
            <<  "从白天唱到黑夜 你一直在身边（一直在身边）\n"
            <<  "如果世界太危险 只有音乐最安全  带着我进梦里面 让歌词都实现\n"
            <<  "无论是开心还是难过我的爱一直不变\n"
            <<  "（不必担心时间流逝带走一切）\n"
            <<  "无论是HIP-HOP还是摇滚我的爱一直不变\n"
            <<  "（所有美好回忆记录在里面）\n"
            <<  "这种Forever Love那么深 我们的歌那么真\n"
            <<  "无国界 跨时代\n"
            <<  "再不会叫我KISS GOODBYE\n"
            <<  "要每一句能够动人心弦 YE~~\n"
            <<  "情人总分分合合\n"
            <<  "可是我们却越爱越深\n"
            <<  "认识你让我的幸福如此 悦耳\n"
            << " 能不能不要切歌 继续唱我们的歌\n"
            <<  "让感动一辈子都记得\n"
            <<  "\n"
            <<  "（一辈子都记得）\n"
            << " 情人总分分合合\n"
            <<  "可是我们却越爱越深\n"
            <<  "认识你让我的幸福如此 悦耳\n"
            << " 能不能不要切歌 继续唱我们的歌\n"
            <<  "让感动一辈子都记得\n";

    }

    int getHeigth()
    {
        int height = 300;
        int actualHeight = list.size() * 30;

        return height > actualHeight ? height : actualHeight;
    }

protected:
    virtual void paintEvent(QPaintEvent *e)
    {
        QPainter painter(this);
        //painter.setPen("#00000000");
        QRect outerRect(0,0,this->width()-1,this->height()-1);
        //painter.fillRect(outerRect, QBrush("#00000000"));                   //绘制背景颜色

        for(int i = 0; i< list.size();i++)
        {
            painter.drawText(5, (i + 1) * 30,list.at(i));
        }

        QWidget::paintEvent(e);
    }


    QStringList list;

};

class ScrollLyricPanel :public QWidget
{

public:
    ScrollLyricPanel(QWidget* parent):QWidget(parent)
    {
        lyricPanel = new LyricPanel(this);
        int height = lyricPanel->getHeigth();
        lyricPanel->setMinimumWidth(300);
        lyricPanel->setMinimumHeight(height);
        lyricPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        scrollArea = new QScrollArea(this);
        //scrollArea->setStyleSheet("QScrollArea{background-color:#00000000;}");

        scrollArea->setWidget(lyricPanel);
        scrollArea->setWidgetResizable(true);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(scrollArea);
    }
public:
    LyricPanel*          lyricPanel;
    QScrollArea * scrollArea;
};


//歌词查看类
class LyricViewer: public QWidget
{
    Q_OBJECT
public:
    LyricViewer(QWidget* parent = nullptr);

    void initLayout();
    void initConnection();



public:
    QLabel                    *labelPreviewTitle;               //歌曲标题
    QLabel                    *labelPreviewSongTip;
    QLabel                    *labelPreviewLyricTip;
    ScrollLyricPanel          *scrollLyricPanel;    //歌词滚动面板

};


#endif //LYRIC_VIEWER_H
