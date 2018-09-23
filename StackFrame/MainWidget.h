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

    //播放全新的音乐盒歌词
    void OnPlayNewMusicAndLyric(QString music, QString lrc);

    //载入lrc歌词，并且播放当前歌曲来预览
    void onLoadLrcLyricAndSwitchToPreview(QString lycLyricPath);

    //从预览页面卸下歌词
    void onUnloadLyricFromPreviewPage();

    //音乐位置发生改变(本来不想不同模块的逻辑理想下放在各个模块内，通过消息连接起来感觉分离比较好
    // 不过 cpu 占用过高可能是 消息太多导致)
    void musicPositionChanged(int);

    //当前歌曲播放结束,准备播放下一首歌
    void onAudioTheadFinished(bool isEndWithForce);

public:
    TopWidget*      topWidget;
    MiddleWidget*   middleWidget;
    BottomWidget*   bottomWidget;
};

#endif // MAINWIDGET_H
