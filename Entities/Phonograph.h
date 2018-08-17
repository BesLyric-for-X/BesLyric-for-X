#ifndef PHONOGRAPH_H
#define PHONOGRAPH_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>

//留声机类
class Phonograph: public QWidget
{
    Q_OBJECT
public:
    Phonograph(QWidget* parent = nullptr);

    void setAlbumCover(QPixmap cover);      //设置专辑封面

    void play();                            //播放
    void stop();                            //暂停

    bool isPlaying();                       //查询是否在播放

    void setBackgroundColor();              //设置背景颜色

protected:
    virtual void paintEvent(QPaintEvent* event);

private slots:
    void updateAngleOfDisk();
    void updateAngleOfArm();

private:
    void initPaintingData(bool firstTime);

    int albumWidth;
    int albumHeight;
    int toneArmWidth;
    int toneArmHeight;

    QPixmap picAlumCover;
    QPixmap picDisk;
    QPixmap picToneArm;

protected:

    QTimer* timerDisk;              //唱片转动定时器
    QTimer* timerArm;               //唱臂转动定时器

    int YoffsetOfDisk;              //唱片y方向上离上边缘的偏移量
    int maxAngleOfArm;              //唱臂最大旋转的角度
    int currentDiskAngle;           //唱片的角度在0~360之间,这里为了便于避免不必要的浮点计算误差（这里每一帧角度要小于1度），单位为0.1度
    int currentArmAngle;

    bool playing;                   //是否正在播放

    QPixmap AlbumCover;             //专辑封面
    QPixmap disk;                   //唱片
    QPixmap toneArm;                //唱臂

    QColor backgroundColor;         //背景颜色

};


#endif //PHONOGRAPH_H
