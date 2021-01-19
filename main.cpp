#include "StackFrame.h"
#include "AppHelper.h"
#include "MyApplication.h"

//------------------- test ----------------------
#include "BesMessageBox.h"
#include "Define/Define.h"
#include "LyricSearcherFactory/LyricSearcherFactory.h"
#include <QDebug>

#include <LyricListManager.h>

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef Q_OS_WIN
// On Windows, SDL will do initialization in "SDL_main()" before calling our "main()".
//   https://stackoverflow.com/questions/11976084/why-sdl-defines-main-macro
//   http://wiki.libsdl.org/FAQWindows#I_get_.22Undefined_reference_to_.27SDL_main.27.22_...
//   https://stackoverflow.com/questions/64396979/how-do-i-use-sdl2-in-my-programs-correctly
//   https://www.libsdl.org/tmp/SDL/VisualC.html
#include <SDL2/SDL.h>
#endif
#ifdef __cplusplus
}
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
