#include "StackFrame.h"
#include "AppHelper.h"
#include "MyApplication.h"

//------------------- test ----------------------
#include "BesMessageBox.h"
#include "Define/Define.h"
#include "LyricSearcherFactory/LyricSearcherFactory.h"
#include <QDebug>

void test()
{
//    QSharedPointer<ISearcher> searcher = CLyricSearcherFactory::CreateSearcher(SEARCH_FROM::SF_NETEASE);

//    QVector<LyricInfo> info;
//    if(!searcher->SearchLyric("我们的歌","王力宏",info))
//    {
//        qDebug()<< searcher->GetLastResult();
//    }
//    else
//    {
//        qDebug()<< "success" <<searcher->GetLastResult();
//    }

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
