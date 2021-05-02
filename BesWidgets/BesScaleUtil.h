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

        //分辨率高小于 800 时，按比例缩放
        //分辨率高大于 800 而小于 1080 时，不缩放
        //分辨率高大于 1080 时，按比例缩放
        //使用例子：窗口的基础高度为 800px, 实际的设置高度为 800 * BesScaleUtil::scale()

        if(primaryScreenRect.height() < 800)
            return primaryScreenRect.height() / 800.0;
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
