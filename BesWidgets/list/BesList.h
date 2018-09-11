#ifndef BES_LIST_H
#define BES_LIST_H

#include <QListWidget>
#include <QLabel>
#include "BesButton.h"

class BesList :public QListWidget
{
    Q_OBJECT
public:
    BesList(QWidget *parent = Q_NULLPTR);


public:

    virtual QSize sizeHint() const{return QSize(220,35);}   //默认宽度是256，和左侧列表250出现垂直滚动条时"挤压"，调小些
    //virtual QSize minimunSizeHint(){return QSize(256,35);}

    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
};

#endif // BES_LIST_H
