#ifndef MY_APPLICATION_H
#define MY_APPLICATION_H

#include <QApplication>
#include <QWidget>

class MainWidget;

class MyApplication:public QApplication
{
public:
    MyApplication(int &argc,char **argv): QApplication(argc,argv){notifyWidget = nullptr;}

    void SetMakingLyricNotifyWidget(MainWidget* widget)
    {
        notifyWidget  = widget;
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

         return QApplication::notify(obj,e);
    }

private:
    MainWidget* notifyWidget;
};

#endif // MY_APPLICATION_H
