#include "StackFrame.h"
#include "AppHelper.h"
#include "MyApplication.h"

//------------------- test ----------------------
#include "BesMessageBox.h"
#include "Define/Define.h"
#include "LyricSearcherFactory/LyricSearcherFactory.h"
#include <QDebug>

#include <LyricListManager.h>

#ifdef Q_OS_MAC
// For SDL2 installed by HomeBrew on macOS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif


void test()
{


}
//------------------- test ----------------------


int main(int argc, char *argv[])
{
    MyApplication app(argc, argv);

    StackFrame MainFrameWidget(&app);
    app.SetMakingLyricNotifyWidget(MainFrameWidget.mainWidget);
    app.SetStackFrame(&MainFrameWidget);
    MainFrameWidget.show();

    //test();


    return app.exec();
}
