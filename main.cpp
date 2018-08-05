#include "StackFrame.h"
#include "AppHelper.h"
#include "MyApplication.h"

int main(int argc, char *argv[])
{
    MyApplication app(argc, argv);
    StackFrame MainFrameWidget(&app);

    app.SetMakingLyricNotifyWidget(MainFrameWidget.mainWidget);

    MainFrameWidget.show();
    return app.exec();
}
