#ifndef PAGEPREVIEWLYRIC_H
#define PAGEPREVIEWLYRIC_H

#include <QWidget>
#include <QPainter>
#include <QThread>
#include "BesButton.h"
#include "Phonograph.h"
#include "LyricViewer.h"
#include <QMutex>


//计算背景的线程
class ThreadCalcBackgroundImage:public QThread
{
    Q_OBJECT
    public:
        ThreadCalcBackgroundImage(QObject* parent):QThread(parent){}
        ~ThreadCalcBackgroundImage();
        virtual void run();

        void showPic(QPixmap pic);
    signals:
        void ready(QPixmap pixmap);

private:
        QMutex m_mutex;
        QVector<QPixmap> vecPic;
};

//预览页面
class PagePreviewLyric : public QWidget
{
    Q_OBJECT

public:
    PagePreviewLyric(QWidget *parent = 0);
    ~PagePreviewLyric();

    void initLayout();
    void initEntity();
    void initConnection();
    void finishInit();

    void calcNewBackgroundImage(QPixmap pixmap);         //开线程计算并设置新的背景图片

    void setWheterToUseBlackMask(bool);                  //设置是否使用黑色mask图层

public slots:
    void playPhonagraph();
    void stopPhonagraph();
    void AlbumImageChanged(QPixmap newPixmap);

    void setToDefaultAlbumImage();

private slots:
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

    ThreadCalcBackgroundImage* calPicThread;  //计算图片的线程
};

#endif // PAGEPREVIEWLYRIC_H
