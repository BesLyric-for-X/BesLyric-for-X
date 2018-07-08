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
protected:
    virtual void paintEvent(QPaintEvent *);

    int border;
};

#endif // BESSHADOWWIDGET_H
