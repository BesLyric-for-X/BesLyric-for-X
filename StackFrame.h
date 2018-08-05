#ifndef STACKFRAME_H
#define STACKFRAME_H

#include "BesFramelessWidget.h"
#include "MainWidget.h"
#include "SkinBoxWidget.h"
#include "MyApplication.h"

/*
 *   StackFrame 程序的主控件类，使用 Qstacklayout 布局
 *   包含程序的主体功能控件 和 最外层的“悬浮控件”（如皮肤盒等）
 */
class StackFrame : public BesFramelessWidget
{
    Q_OBJECT

public:
    StackFrame(MyApplication* pApplication, QWidget *parent = 0);
    ~StackFrame();

    void initLayout();
    void initConnection();

    void setBorderMain(int border);

signals:
    void onFinalSkinNameChanged(QString);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    virtual void resizeEvent(QResizeEvent *event);

public slots:

    void toggleMaxRestoreStatus();          //切换最大化和恢复2个状态
    void toggleSkinBox();                   //显示或隐藏皮肤盒
    void SetSkin(QString skinName);

public:
    bool             isMainOnTop;            //标记当期主体控件是否在最顶层
    MainWidget*      mainWidget;             //程序的主体功能控件
    SkinBoxWidget*   skinBoxWidget;          //皮肤盒
    int              borderMain;             //边框厚度

    MyApplication*    pApp;
};

#endif // STACKFRAME_H
