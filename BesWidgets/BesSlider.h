#ifndef BesSlider_H
#define BesSlider_H

#include <QDebug>

#include <QSlider>
#include <QMouseEvent>

#include <QStyleOption>
#include <QProxyStyle>

//实现参考：https://blog.csdn.net/liutingxi0709/article/details/51985618
//  注意，现在的实现已经与上述链接中的内容差很远了。
class BesSlider :public QSlider
{
    Q_OBJECT
public:
    BesSlider(QWidget* parent = Q_NULLPTR):QSlider(parent){
        setStyle(new BesSliderStyle(this->style()));
        setMouseTracking(true);// 为了鼠标在 slider 上方（不点击）移动时触发 mouseMoveEvent 以显示正确的鼠标形状
    }

    //是否启用鼠标事件响应
    void enableMouseEvent(bool enable)
    {
        enableMouseEvt = enable;
    }

private:
    QRect getHandleRect(){
        // https://stackoverflow.com/questions/52550633/how-to-emit-a-signal-if-double-clicking-on-slider-handle

        initStyleOption(&qStyleOptionStyle);
        QRect subControlRect = this->style()->subControlRect(QStyle::CC_Slider, &qStyleOptionStyle, QStyle::SC_SliderHandle, this);

        return subControlRect;
    }

protected:
    // 用 mouseMoveEvent 事件来处理鼠标形状
    void mouseMoveEvent(QMouseEvent *ev){
        qDebug()<<"BesSlider::mouseMoveEvent: "<<ev->pos();

        // 通过偏移计算 handle 的正确位置
        //ev->setLocalPos(ev->pos() += click_pos_offset);//?没有该函数？

        // 如果需要在 enableMouseEvt == false 时，鼠标移动到 handle 上也改变鼠标样式，就去掉关于 enableMouseEvt 的判断。
        if (enableMouseEvt && getHandleRect().contains(ev->pos())) {
            qDebug() << "handle hovering";

            setCursor(QCursor(Qt::PointingHandCursor));
        }
        else{
            unsetCursor();
        }

        QSlider::mouseMoveEvent(ev);
    }

    void mouseReleaseEvent(QMouseEvent *ev){
        qDebug()<<"BesSlider::mouseReleaseEvent ev->pos()"<<ev->pos()<<value();

        if(!enableMouseEvt){
            return;
        }

        QSlider::mouseReleaseEvent(ev);

        // 复位偏移，避免对 mouseMoveEvent() 方法中关于鼠标是否在 handle 上的判断造成干扰。
        click_pos_offset = QPoint(0, 0);
    }

    //重写QSlider的mousePressEvent事件 &QSlider::sliderPressed 的接收者由 sig_clickNotOnHandle 信号获得最新位置
    void mousePressEvent(QMouseEvent *ev)
    {
        qDebug()<<"BesSlider::mousePressEvent(QMouseEvent *ev), enableMouseEvt="<<enableMouseEvt;

        if(!enableMouseEvt){
            return;
        }

        qDebug()<<"BesSlider::mousePressEvent() old value:"<<value();

        QRect handle_rect = getHandleRect();
        if (handle_rect.contains(ev->pos())) {
            qDebug() << "handle clicked";

            QPoint center_of_handle = handle_rect.center();

            // 计算偏移
            click_pos_offset = center_of_handle - ev->pos();
            qDebug()<<"click_pos_offset:"<<click_pos_offset;

            //用 handle 中心点代替实际点击位置，避免在点击到 handle 非中心位置时移动 handle，使 handle 看起来是能在任何位置被拖动的
            //ev->setLocalPos(center_of_handle);//?没有该函数？
        }else{
            qDebug() << "handle not clicked";

            // 当鼠标点击 slider 非 handle 位置后，当 handle 移动到鼠标位置时，样式变为指针。
            setCursor(QCursor(Qt::PointingHandCursor));
        }

        QSlider::mousePressEvent(ev); //在这之后， handle 移动到了点击的位置，value() 也变为点击处的值，所以任何判断都要在之前进行

        qDebug()<<"BesSlider::mousePressEvent() new value:"<<value();

        // 发出包含点击位置处的取值的信号，接收者用新值刷新相关组件（例如实时显示 QSlider 取值的 QLabel）的值。
        //   &QSlider::sliderPressed() 信号不包含点击位置，所以需要这个额外的信号。
        emit sig_clickNotOnHandle(value());
    }

signals:
    void sig_clickNotOnHandle(int pos);

private:
    bool enableMouseEvt = false;  //默认禁用，仅在非制作模式播放时启用

    QStyleOptionSlider qStyleOptionStyle;

    //在 handle 上的点击位置距离 handle 中心点的偏移距离
    QPoint click_pos_offset = QPoint(0, 0);


    class BesSliderStyle : public QProxyStyle{
        // https://stackoverflow.com/questions/12409559/qslider-makes-unnecessary-steps
        // https://stackoverflow.com/questions/11132597/qslider-mouse-direct-jump

        using QProxyStyle::QProxyStyle;

        int styleHint(QStyle::StyleHint hint,const QStyleOption* option = nullptr, const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const{
            if (hint == QStyle::SH_Slider_AbsoluteSetButtons){
                return (Qt::LeftButton | Qt::MiddleButton | Qt::RightButton);
            }
            return QProxyStyle::styleHint(hint, option, widget, returnData);
        }
    };
};

#endif // BesSlider_H
