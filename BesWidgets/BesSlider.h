#ifndef BesSlider_H
#define BesSlider_H

#include <QDebug>

#include <QSlider>
#include <QMouseEvent>

#include <QStyleOption>
#include <QProxyStyle>

//实现参考：https://blog.csdn.net/liutingxi0709/article/details/51985618
class BesSlider :public QSlider
{
    Q_OBJECT
public:
//    BesSlider(QWidget* parent = Q_NULLPTR, int handleWidth = 30):QSlider(parent),handleWidthValue(handleWidth){}
    BesSlider(QWidget* parent = Q_NULLPTR):QSlider(parent){
        setStyle(new BesSliderStyle(this->style()));
        setMouseTracking(true);// 为了鼠标在 sliderSong 上方（不单击）移动时触发 mouseMoveEvent 以显示正确的鼠标形状
    }

    //覆盖 QAbstractSlider::setRange,为了记录范围，判断单前点击的是不是在handle上
//    void setRange(int min, int max)
//    {
//        QAbstractSlider::setRange(min, max);
//        minValue = min;
//        maxValue = max;
//    }

    //是否启用鼠标事件响应
    void enableMouseEvent(bool enable)
    {
        enableMouseEvt = enable;
    }

protected:
    QRect getHandleRect(){
        // https://stackoverflow.com/questions/52550633/how-to-emit-a-signal-if-double-clicking-on-slider-handle

        initStyleOption(&qStyleOptionStyle);
        QRect subControlRect = this->style()->subControlRect(QStyle::CC_Slider, &qStyleOptionStyle, QStyle::SC_SliderHandle, this);

        return subControlRect;
    }

    bool isPointInHandle(QPoint point){
        return getHandleRect().contains(point);
    }

    // 用 mouseMoveEvent 事件来处理鼠标形状
    void mouseMoveEvent(QMouseEvent *ev){
//        qDebug()<<"mouseMoveEvent: "<<ev->pos();

        // 为了精简逻辑，这里无条件调用父类方法，不知有何隐患
        QSlider::mouseMoveEvent(ev);

        if (enableMouseEvt && isPointInHandle(ev->pos())) {
            qDebug() << "handle hovering";

            setCursor(QCursor(Qt::PointingHandCursor));
        }
        else{
            unsetCursor();// 去掉 enableMouseEvt 条件判断后，能让鼠标在歌曲结束前变为 Qt::PointingHandCursor 后，在歌曲结束后变回默认样式
        }
    }

    //重写QSlider的mouseReleaseEvent事件以使 &QSlider::sliderReleased 的接收者由 sig_refreshClickPos 信号获得最新位置
    void mouseReleaseEvent(QMouseEvent *ev){
//        qDebug()<<"mouseReleaseEvent: "<<ev->pos();

        // isHandleClicked 判断释放鼠标前是否点击了 handle
        // ev->pos()== oldPos 判断释放鼠标前后位置是否相同
        if(isHandleClicked && ev->pos()== oldPos){
            qDebug()<<"handle will not move";

            //没有移动，用旧位置刷新，以回到 sliderSong（的 handle 的）之前的位置
            emit sig_refreshClickPos(oldValue);
        }else{
            //如果移动了，就刷新当前位置
            emit sig_refreshClickPos(value());
        }

        //在这之前发送 sig_refreshClickPos，避免 &QSlider::sliderReleased 的接收者没有拿到最新位置
        QSlider::mouseReleaseEvent(ev);
    }

    //重写QSlider的mousePressEvent事件
    void mousePressEvent(QMouseEvent *ev)
    {
        qDebug()<<"void mousePressEvent(QMouseEvent *ev), enableMouseEvt="<<enableMouseEvt;

        if(!enableMouseEvt)
            return;

        oldPos = ev->pos();
        oldValue = value();

        isHandleClicked = isPointInHandle(oldPos);
        if (isHandleClicked) {
            qDebug() << "handle clicked";

            //用 handle 中心点代替实际点击位置，避免在点击到 handle 非中心位置时移动 handle，使 handle 看起来是能在任何位置被拖动的
            ev->setLocalPos(getHandleRect().center());
        }

        QSlider::mousePressEvent(ev); //在这之后，手柄移动到了点击的位置，value() 也变为点击处的值，所以任何判断都要在之前进行

        //刷新当前位置
        // 在这里执行没有意义，&QSlider::sliderPressed 的接收者也无法做更多的工作，所以放到 mouseReleaseEvent(ev) 中发送
//        emit sig_refreshClickPos(value());

        //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
//        QSlider::mousePressEvent(ev); //这会造成移动，之后拿到的value()都是移动后的

        //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
//        double posPercent = ev->pos().x() / (double)width();

//        int pos = minValue + (int)((maxValue-minValue)* posPercent);
//        int curValue = value();
//        if(pos  < curValue - handleWidthValue/2 || pos  > curValue + handleWidthValue/2)
//        {
            //发送自定义的鼠标单击信号
//            emit sig_clickNotOnHandle(pos);
//        }
    }

//    virtual void enterEvent(QEvent *event)
//    {
//        if(enableMouseEvt)
//        {
//            setCursor(QCursor(Qt::PointingHandCursor));
//            QSlider::enterEvent(event);
//        }
//        else
//            setCursor(QCursor(Qt::ArrowCursor));
//    }

//    virtual void leaveEvent(QEvent *event)
//    {
//        unsetCursor();

//        if(enableMouseEvt)
//            QSlider::leaveEvent(event);
//    }

signals:
    void sig_refreshClickPos(int pos);

private:
//    int minValue = 0;
//    int maxValue = 0;
//    double handleWidthValue = 30; //滑块handle的大小，这个取值会作为 sig_clickNotOnHandle 发送时判断是否在handle之外的依据
    bool enableMouseEvt = false;  //默认禁用

    QPoint oldPos;
    int oldValue = 0;
    bool isHandleClicked = false;

    QStyleOptionSlider qStyleOptionStyle;


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
