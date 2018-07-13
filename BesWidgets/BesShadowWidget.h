#ifndef BESSHADOWWIDGET_H
#define BESSHADOWWIDGET_H

#include <QWidget>
#include<QMouseEvent>

class BesShadowWidget : public QWidget
{
    Q_OBJECT
public:
    BesShadowWidget(QWidget *parent = 0);
    virtual~BesShadowWidget(){}

    void SetShadowBorder(int boder);

    void SetMarginTop(int margin);

    void SetBackgroundColor(QColor bgColor);
protected:
    virtual void paintEvent(QPaintEvent *);

    QColor bgColor;     //背景颜色
    int marginTop;      //上边缘空出距离
    int border;
};

#endif // BESSHADOWWIDGET_H
