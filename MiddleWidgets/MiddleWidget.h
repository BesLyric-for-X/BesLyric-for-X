#ifndef MIDDLEWIDGET_H
#define MIDDLEWIDGET_H

#include <QWidget>
#include "PageMain.h"
#include "PagePreviewLyric.h"
#include "PageMatchNcmID.h"
#include "PageLyricList.h"
#include "PageSetting.h"
#include <QStackedLayout>

class MiddleWidget : public QWidget
{
    Q_OBJECT

public:
    MiddleWidget(QWidget *parent = 0);
    ~MiddleWidget();


    void initLayout();
    void connectAll();

public:
    QStackedLayout*     stackPageLayout;
    PageMain*           pageMain;
    PagePreviewLyric*   pagePreviewLyric;
    PageMatchNcmID*     pageMatchNcmID;
    PageLyricList*      pageLyricList;
    PageSetting*        pageSetting;

};

#endif // TOPWIDGET_H
