#ifndef BESBUTTON_H
#define BESBUTTON_H

#include <QPushButton>

class BesButton :public QPushButton
{
public:
    BesButton(QWidget* parent = nullptr):QPushButton(parent){}
    virtual void enterEvent(QEvent *event){setCursor(QCursor(Qt::PointingHandCursor));QPushButton::enterEvent(event);}
    virtual void leaveEvent(QEvent *event){unsetCursor();QPushButton::leaveEvent(event);}

};

#endif // BESBUTTON_H
