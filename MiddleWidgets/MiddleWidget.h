#ifndef MIDDLEWIDGET_H
#define MIDDLEWIDGET_H

#include <QWidget>
#include <QStackedLayout>
#include <QPropertyAnimation>

#include "PageMain.h"
#include "PagePreviewLyric.h"
#include "PageMatchNcmID.h"
#include "PageLyricList.h"
#include "PageSetting.h"

class MiddleWidget : public QWidget
{
    Q_OBJECT

public:
    MiddleWidget(QWidget *parent = 0);
    ~MiddleWidget();


    void initLayout();
    void initConnection();
    void initAnimation();

    void AnimateFromRectToRect(QPropertyAnimation& animation, QWidget* widget, QRect fromRect, QRect toRect);

public slots:
    void switchPage(int from, int to);
    void geometryAnimationFinish();

    void onReloadMusic(QString musicFileNamePath);
    void onSetMusicTitle(QString title);
    void onSetMusicArtist(QString artist);
    void onSetMusicAlbum(QString album);

protected:
    virtual void resizeEvent(QResizeEvent *event);


public:
    PageMain*           pageMain;                       //page 0
    PagePreviewLyric*   pagePreviewLyric;               //page 1
    PageLyricList*      pageLyricList;                  //page 2
    PageMatchNcmID*     pageMatchNcmID;                 //page 3
    PageSetting*        pageSetting;                    //page 4

    int currentPage;                                    //标记当前页面
                                                        // 0 是主页  1 是歌词预览页

    QPropertyAnimation  animationPreviewExtend;
    QPropertyAnimation  animationLrcListExtend;
    QPropertyAnimation  animationSettingExtend;

    int nRuningAnimationCount;

private:
    bool previewFromLyricListPage = false;               //为了区别预览页面触发来源，以便从预览页面返回时回到原来的页面
};

#endif // TOPWIDGET_H
