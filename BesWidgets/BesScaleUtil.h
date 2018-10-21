#ifndef BESSCALEUTIL_H
#define BESSCALEUTIL_H
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>

class BesScaleUtil{
public:
    //按和最大宽的比例缩放
    static float scale()
    {
        QDesktopWidget* desktopWidget = QApplication::desktop();

        //获取可用桌面大小
        QRect deskRect = desktopWidget->availableGeometry();

        // 如果分辨率已经在1920及以上，则不需要进行缩放处理
        if(deskRect.width() >= 1920)
            return 1.0;
        else
            return deskRect.width() / 1920.0;
    }

    //按和最大宽的比例缩放，但是又不要缩放太小，取缩放前后均值
    static float mscale()
    {
        QDesktopWidget* desktopWidget = QApplication::desktop();

        //获取可用桌面大小
        QRect deskRect = desktopWidget->availableGeometry();

        // 如果分辨率已经在1920及以上，则不需要进行缩放处理
        if(deskRect.width() >= 1920)
            return 1.0;
        else
        {
            return (deskRect.width() + 1920.0)/2 / 1920.0;
        }
    }

    //缩放字体大小
    static int fontSizeScale(int size)
    {
        int sizeAfterScale = size * BesScaleUtil::scale();

        if(sizeAfterScale < size)
        {
            sizeAfterScale = (sizeAfterScale + size)/2 + 1;
        }

        return sizeAfterScale;
    }
};

#endif // BESSCALEUTIL_H
