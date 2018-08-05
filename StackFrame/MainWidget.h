#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "TopWidget.h"
#include "MiddleWidget.h"
#include "BottomWidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void initEntity();      //初始化实体
    void initLayout();
    void initConnection();

    bool keyPress(QKeyEvent  *event);

public slots:

    //载入lrc歌词，并且播放当前歌曲来预览
    void onLoadLrcLyricAndSwitchToPreview(QString lycLyricPath);

    //从预览页面卸下歌词
    void onUnloadLyricFromPreviewPage();

public:
    TopWidget*      topWidget;
    MiddleWidget*   middleWidget;
    BottomWidget*   bottomWidget;
};

#endif // MAINWIDGET_H
