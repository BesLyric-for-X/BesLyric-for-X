#include "StackFrame.h"
#include <QApplication>
#include "AppHelper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    StackFrame MainFrameWidget(&app);

    MainFrameWidget.show();
    return app.exec();
}
