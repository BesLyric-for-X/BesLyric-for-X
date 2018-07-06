#include "StackFrame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StackFrame MainFrameWidget;
    MainFrameWidget.show();

    return a.exec();
}
