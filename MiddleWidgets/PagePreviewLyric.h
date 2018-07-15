#ifndef PAGEPREVIEWLYRIC_H
#define PAGEPREVIEWLYRIC_H

#include <QWidget>
#include <QPainter>
#include <QThread>
#include "BesButton.h"
#include "Phonograph.h"
#include "LyricViewer.h"

//计算背景的线程
class ThreadCalcBackgroundImage:public QThread
{
    Q_OBJECT
    public:
        ThreadCalcBackgroundImage(QPixmap pixmap, QObject* parent):QThread(parent){pixmapToDeal = pixmap;}
        virtual void run();
    signals:
        void ready(QPixmap pixmap);
private:
        QPixmap pixmapToDeal;
};

//预览页面
class PagePreviewLyric : public QWidget
{
    Q_OBJECT

public:
    PagePreviewLyric(QWidget *parent = 0);
    ~PagePreviewLyric();

    void initLayout();
    void connectAll();

    void calcNewBackgroundImage(QPixmap pixmap);         //开线程计算并设置新的背景图片

    void setWheterToUseBlackMask(bool);                  //设置是否使用黑色mask图层
public slots:
    void setNewBackgroundPixmap(QPixmap newPixmap);

protected:
    virtual void mousePressEvent(QMouseEvent* event);

    virtual void paintEvent(QPaintEvent* event);

public:
    QWidget* widgetMainPreview;         //这个父控件

    Phonograph* phonagraph;             //留声机类
    LyricViewer* lyricViewer;           //歌词查看器
    BesButton*  btnPackupLyricBox;      //将歌词收起按钮

    QPixmap  blurbackgroudImage;        //模糊后的背景图片
    QPixmap  whiteMaskImage;            //白色的过滤图片
    QPixmap  blackMaskImage;            //黑色的过滤图片

    bool useBlackMask;                  //标记是否使用黑色的mask图层
};

#endif // PAGEPREVIEWLYRIC_H
