#include "StackFrame.h"
#include "AppHelper.h"
#include "MyApplication.h"

//------------------- test ----------------------
#include "BesMessageBox.h"
#include "Define/Define.h"
#include "LyricSearcherFactory/LyricSearcherFactory.h"
#include <QDebug>

#include <LyricListManager.h>
#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif


void test()
{


}
//------------------- test ----------------------


int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    // They are used to prevent the installer and the uninstaller of
    //   Inno Setup from being executed while BesLyric-for-X is running.
    CreateMutex(NULL, FALSE, TEXT("AppMutex_{7ACD3BB0-DE1F-416E-A8DC-5C6EE4AECB50}"));
    CreateMutex(NULL, FALSE, TEXT("Global\\AppMutex_{7ACD3BB0-DE1F-416E-A8DC-5C6EE4AECB50}"));
#endif

    MyApplication app(argc, argv);
    app.setApplicationName("BesLyric-for-X");

    StackFrame MainFrameWidget(&app);
    app.SetMakingLyricNotifyWidget(MainFrameWidget.mainWidget);
    app.SetStackFrame(&MainFrameWidget);
    MainFrameWidget.show();

    //test();


    return app.exec();
}
