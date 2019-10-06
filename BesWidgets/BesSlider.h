﻿#ifndef BesSlider_H
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
    bool isPointInHandle(QPoint point){
        // https://stackoverflow.com/questions/52550633/how-to-emit-a-signal-if-double-clicking-on-slider-handle

        initStyleOption(&qStyleOptionStyle);
        QRect subControlRect = this->style()->subControlRect(QStyle::CC_Slider, &qStyleOptionStyle, QStyle::SC_SliderHandle, this);
        QPoint clickPoint = point;

        return subControlRect.contains(clickPoint);
    }

    // 用 mouseMoveEvent 事件来处理鼠标形状
    void mouseMoveEvent(QMouseEvent *ev){
//        qDebug()<<"mouseMoveEvent: "<<ev->pos();

        if(!enableMouseEvt){
            QSlider::mouseMoveEvent(ev);
            return;
        }

        if (isPointInHandle(ev->pos())) {
            qDebug() << "handle hovering";

            setCursor(QCursor(Qt::PointingHandCursor));
        }
        else{
            unsetCursor();
        }

        QSlider::mouseMoveEvent(ev);
    }

    //重写QSlider的mousePressEvent事件
    void mousePressEvent(QMouseEvent *ev)
    {
        qDebug()<<"void mousePressEvent(QMouseEvent *ev), enableMouseEvt="<<enableMouseEvt;

        if(!enableMouseEvt)
            return;

        if (isPointInHandle(ev->pos())) {
            qDebug() << "handle clicked";
            QSlider::mousePressEvent(ev);
        }
        else{
            QSlider::mousePressEvent(ev);
            //发送自定义的鼠标单击信号
            emit sig_clickNotOnHandle(value());
        }

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
    void sig_clickNotOnHandle(int pos);

private:
//    int minValue = 0;
//    int maxValue = 0;
//    double handleWidthValue = 30; //滑块handle的大小，这个取值会作为 sig_clickNotOnHandle 发送时判断是否在handle之外的依据
    bool enableMouseEvt = false;  //默认禁用

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
