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

        //分辨率高小于 800 或宽小于 1224 时，按比例缩放
        //分辨率高大于等于 800 且宽大于等于 1224 时，不缩放
        //使用例子：窗口的基础高度与宽度分别为 800px 与 1224px , 实际设置的高度与宽度分别为 800 * BesScaleUtil::scale() 与 1224 * BesScaleUtil::scale()

        double heightScalingRatio = primaryScreenRect.height() / 800.0;
        double widthScalingRatio = primaryScreenRect.width() / 1224.0;

        if(heightScalingRatio >= 1.0 && widthScalingRatio >= 1.0)
            return 1.0;

        return heightScalingRatio < widthScalingRatio ? heightScalingRatio : widthScalingRatio;
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
