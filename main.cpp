#include "StackFrame.h"
#include <QApplication>
#include "AppHelper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AppHelper::SetStyle(app, "red");

    //app.setStyleSheet("QWidget{border:2px solid #000000;}");

    StackFrame MainFrameWidget;
    MainFrameWidget.show();

    return app.exec();
}
