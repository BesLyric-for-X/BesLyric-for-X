#ifndef BESSCALEUTIL_H
#define BESSCALEUTIL_H
#include <QApplication>
#include <QScreen>
#include <QRect>

class BesScaleUtil{
public:
    //按和最大宽的比例缩放
    static double scale()
    {
        //获取主屏幕可用大小
        QRect primaryScreenRect = getPrimaryScreenRect();

        //窗口本身高800px，分辨率高小于800时缩放到一个固定比例。
        //              分辨率高大于800，小于1080的情况下不缩放
        //              分辨率高大于1080时，按比例缩放

        if(primaryScreenRect.height() < 800)
            return 0.72;
        else if(primaryScreenRect.height() <= 1080)
            return 1.0;
        else
            return primaryScreenRect.height() / 1080.0;
    }

    static QRect getPrimaryScreenRect()
    {

        QScreen* primaryScreen = QGuiApplication::primaryScreen();

        //获取主屏幕可用大小
        QRect primaryScreenRect = primaryScreen->availableGeometry();

        return primaryScreenRect;
    }
};

#endif // BESSCALEUTIL_H
