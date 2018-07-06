#ifndef STACKFRAME_H
#define STACKFRAME_H

#include "BesFramelessWidget.h"
#include "MainWidget.h"
#include "SkinBoxWidget.h"

/*
 *   StackFrame 程序的主控件类，使用 Qstacklayout 布局
 *   包含程序的主体功能控件 和 最外层的“悬浮控件”（如皮肤盒等）
 */
class StackFrame : public BesFramelessWidget
{
    Q_OBJECT

public:
    StackFrame(QWidget *parent = 0);
    ~StackFrame();

    void initLayout();
    void connectAll();

private:
    MainWidget*      mainWidget;             //程序的主体功能控件
    SkinBoxWidget*   skinBoxWidget;          //皮肤盒
};

#endif // STACKFRAME_H
