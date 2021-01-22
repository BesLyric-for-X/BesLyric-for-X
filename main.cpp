#include "StackFrame.h"
#include "AppHelper.h"
#include "MyApplication.h"

//------------------- test ----------------------
#include "BesMessageBox.h"
#include "Define/Define.h"
#include "LyricSearcherFactory/LyricSearcherFactory.h"
#include <QDebug>

#include <LyricListManager.h>

// --- For SDL_main.h Begin ---
#ifdef Q_OS_WIN32
//
// 1. In qtmain_win.cpp, Qt's WinMain() will call our main(),
//   so don't let SDL change main() to SDL_main().
//   https://github.com/qt/qtbase/blob/5.12.4/src/winmain/qtmain_win.cpp#L97
// 2. On Q_OS_WIN32 (not all windows, such as Q_OS_WINRT), SDL_main is not required if we provide own WinMain().
//   (That is, Qt provided one WinMain() as this program's entrypoint).
//   https://github.com/spurious/SDL-mirror/blob/release-2.0.3/include/SDL_main.h#L33-L41
//   https://github.com/SDL-mirror/SDL/blob/release-2.0.3/include/SDL_main.h#L33-L41
//
#define SDL_MAIN_HANDLED
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef Q_OS_MAC
// For SDL2 installed by HomeBrew on macOS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#ifdef __cplusplus
}
#endif
// --- For SDL_main.h End ---

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
