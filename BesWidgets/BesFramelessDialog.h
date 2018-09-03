#ifndef BESFRAMELESSDIALOG_H
#define BESFRAMELESSDIALOG_H

#include <QWidget>
#include<QMouseEvent>
#include "BesShadowDialog.h"
#include "BesFramelessWidget.h"

//无边框可缩放控件（注意：使用本控件作为主窗体，
//                      只有子窗体也设置 this->setMouseTracking(true); 才能在理想状态下工作）
/*
    另外，如果希望只有“标题栏” 和边框处理 拖动事件，可以如下在下一级子窗口处理 press 事件

    //由于 BesFramelessWidget 要求获得所有鼠标信息，处理鼠标图案和拖动功能，但是
    //          这里只希望处理标题拖动 和 边缘resize 操作，所以判断是否在标题 或 边缘，决定是否处理 Release
    QPoint currentPoint = event->pos();
    if(mainWidget->topWidget->geometry().contains(currentPoint) ||
            !mainWidget->geometry().contains(currentPoint))
        BesFramelessWidget::mouseReleaseEvent(event);
    else
        QWidget::mouseReleaseEvent(event);

*/
class BesFramelessDialog : public BesShadowDialog
{
    Q_OBJECT
public:
    BesFramelessDialog(QWidget *parent = 0);
    virtual~BesFramelessDialog(){}

    void SetFrameBorder(int boder);
private:
    void region(const QPoint &cursorPoint);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    bool isLeftPressDown;
    QPoint dragPosition;
    Direction dir;
    int border;
};

#endif // BESFRAMELESSDIALOG_H
