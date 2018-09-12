#ifndef MY_APPLICATION_H
#define MY_APPLICATION_H

#include <QApplication>
#include <QMouseEvent>
#include "StackFrame.h"
#include <QWidget>

class MainWidget;
class StackFrame;

class MyApplication:public QApplication
{
public:
    MyApplication(int &argc,char **argv): QApplication(argc,argv)
    {notifyWidget = nullptr;stackFrame= nullptr;}

    void SetMakingLyricNotifyWidget(MainWidget* widget)
    {
        notifyWidget  = widget;
    }

    void SetStackFrame(StackFrame* stackFrameWidget)
    {
        stackFrame = stackFrameWidget;
    }

    bool notify(QObject *obj, QEvent *e)
    {
         if(e->type() == QEvent::KeyPress)
         {
             QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);

             if(notifyWidget)
             {
                 if(notifyWidget->keyPress(keyEvent))  //先交由指定窗口处理
                     return true;
             }
         }

         if(e->type() == QMouseEvent::MouseButtonPress)
         {
             //通过此方式来关闭悬浮窗口，不理想

//             QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(e);
//             if(stackFrame)
//             {
//                 stackFrame->mousePressFilter(mouseEvent);  //先交由stackFrame处理
//             }
         }

         return QApplication::notify(obj,e);
    }

private:
    MainWidget* notifyWidget;
    StackFrame* stackFrame;
};

#endif // MY_APPLICATION_H
