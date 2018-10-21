#include "Phonograph.h"
#include <QPainter>
#include "BesScaleUtil.h"

Phonograph::Phonograph(QWidget* parent):QWidget(parent),
    YoffsetOfDisk(76 * BesScaleUtil::scale()),
    maxAngleOfArm(36),
    currentDiskAngle(0),
    currentArmAngle(0),
    playing(false),
    AlbumCover(":/resource/image/AlbumCover1.jpg"),
    disk(":/resource/image/netease_disk.png"),
    toneArm(":/resource/image/netease_tone_arm.png")
{
    setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    timerDisk = new QTimer(this);              //唱片转动定时器
    timerArm = new QTimer(this);               //唱臂转动定时器

    timerDisk->setInterval(33);                 //人眼能分辨的每秒帧率一般在24-30帧
    timerArm->setInterval(25);

    connect(timerDisk,SIGNAL(timeout()),this,SLOT(updateAngleOfDisk()));
    connect(timerArm,SIGNAL(timeout()),this,SLOT(updateAngleOfArm()));

    initPaintingData(true);
}

void Phonograph::setAlbumCover(QPixmap cover)
{
    AlbumCover = cover;
    initPaintingData(false);
}

void Phonograph::play()
{
    playing = true;

    if(!timerDisk->isActive())    //未被启动时才启动
        timerDisk->start();
    if(!timerArm->isActive())     //未被启动时才启动
        timerArm->start();
}

void Phonograph::stop()
{
    playing = false;

    if(!timerArm->isActive())   //如果唱臂定时器已经关闭，启动定时器
        timerArm->start();
}

//查询是否在播放
bool Phonograph::isPlaying()
{
    return playing;
}

void Phonograph::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

    QRect outerRect(0,0,this->width()-1,this->height()-1);
    painter.fillRect(outerRect, QBrush("#00000000"));                   //绘制背景颜色
    QWidget::paintEvent(event);

////////////////////////////////////////////////////////////////////

    //绘制相关参数计算
    QPoint albumBase(this->width()/2-albumWidth/2, YoffsetOfDisk);                      //唱片左上角位置
    QPoint albumCenter(albumBase.x() + albumWidth/2, albumBase.y() + albumHeight/2);    //唱片中心点位置

    QPoint rotatePointOfArm(20,20);                 //唱臂图片上旋转的点
    QPoint ArmRotatePoint(this->width()/2, 0);      //唱臂在整个控件中旋转的点

//////////////////////////////////////////////////////////////////////

    painter.save();         //状态入栈

    //绘制唱片
    painter.translate(albumCenter);     //设置中心点到唱片要在整个控件上放置的中心点位置
    painter.rotate(currentDiskAngle * 1.0 / 10);

    painter.drawPixmap(- albumCoverWidth/2, - albumCoverHeight/2,albumCoverWidth, albumCoverHeight,picAlumCover);
    painter.drawPixmap(- albumWidth/2, - albumHeight/2,albumWidth, albumHeight, picDisk);

    painter.restore();      //恢复之前状态

//////////////////////////////////////////////////////////////

    painter.save();         //状态入栈
    //绘制唱臂
    painter.translate(ArmRotatePoint);     //设置在整个控件上旋转点
    painter.rotate(currentArmAngle);

    painter.drawPixmap( - rotatePointOfArm.x(), - rotatePointOfArm.y(), toneArmWidth, toneArmHeight, picToneArm);

    painter.restore();      //恢复之前状态
}

void Phonograph::updateAngleOfDisk()
{
    if(playing)
    {
        currentDiskAngle += 3; //每帧转动 0.3 度

        currentDiskAngle = currentDiskAngle % 3600;  // 0.1 * 3600 = 360

        update();
    }
    else
        timerDisk->stop();      //已经被要求停止，停止定时器
}

void Phonograph::updateAngleOfArm()
{
    if(playing && currentArmAngle != maxAngleOfArm) //播放中，但是磁头没到位，则转动一下
    {
        currentArmAngle+=4;  // 每帧转动4度

        if(currentArmAngle > maxAngleOfArm)
            currentArmAngle = maxAngleOfArm;

        update();
    }

    if(!playing && currentArmAngle != 0)  //已经没在播放，却没有复位，则转动一下
    {
        currentArmAngle-=4;

        if(currentArmAngle < 0)
            currentArmAngle = 0;
        update();
    }

    if((playing && currentArmAngle == maxAngleOfArm) ||
            (!playing && currentArmAngle == 0))
    {
        timerArm->stop(); //唱臂到位，停止定时器
    }

}

void Phonograph::initPaintingData(bool firstTime)
{
   if(firstTime)
   {
        albumWidth = 400 * BesScaleUtil::scale();
        albumHeight = 400 * BesScaleUtil::scale();
        toneArmWidth = 200 * BesScaleUtil::scale();
        toneArmHeight = 100 * BesScaleUtil::scale();

        picDisk = disk.scaled(albumWidth, albumHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        picToneArm = toneArm.scaled(toneArmWidth, toneArmHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        albumCoverWidth = albumWidth * 0.66;
        albumCoverHeight = albumHeight * 0.66;
   }

   picAlumCover = AlbumCover.scaled(albumCoverWidth, albumCoverHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}
