#ifndef BESSCALEUTIL_H
#define BESSCALEUTIL_H
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

class BesScaleUtil{
public:
    //按和最大宽的比例缩放
    static double scale()
    {
        //获取可用桌面大小
        QRect deskRect = getDeskRect();

        //窗口本身高800px，分辨率高小于800时缩放到一个固定比例。
        //              分辨率高大于800，小于1080的情况下不缩放
        //              分辨率高大于1080时，按比例缩放

        if(deskRect.height() < 800)
            return 0.72;
        else if(deskRect.height() <= 1080)
            return 1.0;
        else
            return deskRect.height() / 1080.0;
    }

    static QRect getDeskRect()
    {

        QDesktopWidget* desktopWidget = QApplication::desktop();

        //获取可用桌面大小
        QRect deskRect = desktopWidget->availableGeometry();

        return deskRect;
    }
};

#endif // BESSCALEUTIL_H
