#ifndef BesSlider_H
#define BesSlider_H

#include <QSlider>
#include <QMouseEvent>

//实现参考：https://blog.csdn.net/liutingxi0709/article/details/51985618
class BesSlider :public QSlider
{
    Q_OBJECT
public:
    BesSlider(QWidget* parent = Q_NULLPTR, int handleWidth = 30):QSlider(parent),handleWidthValue(handleWidth){}

    //覆盖 QAbstractSlider::setRange,为了记录范围，判断单前点击的是不是在handle上
    void setRange(int min, int max)
    {
        QAbstractSlider::setRange(min, max);
        minValue = min;
        maxValue = max;
    }

protected:
    //重写QSlider的mousePressEvent事件
    void mousePressEvent(QMouseEvent *ev)
    {
        //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
        QSlider::mousePressEvent(ev);

        //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
        double posPercent = ev->pos().x() / (double)width();

        int pos = minValue + (int)((maxValue-minValue)* posPercent);
        int curValue = value();
        if(pos  < curValue - handleWidthValue/2 || pos  > curValue + handleWidthValue/2)
        {
            //发送自定义的鼠标单击信号
            emit sig_clickNotOnHandle(pos);
        }
    }


    virtual void enterEvent(QEvent *event){setCursor(QCursor(Qt::PointingHandCursor));QSlider::enterEvent(event);}
    virtual void leaveEvent(QEvent *event){unsetCursor();QSlider::leaveEvent(event);}

signals:
    void sig_clickNotOnHandle(int pos);

private:
    int minValue = 0;
    int maxValue = 0;
    double handleWidthValue = 30; //滑块handle的大小，这个取值会作为 sig_clickNotOnHandle 发送时判断是否在handle之外的依据
};

#endif // BesSlider_H
