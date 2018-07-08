#include "MiddleWidget.h"

MiddleWidget::MiddleWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
    initLayout();
    connectAll();
}

MiddleWidget::~MiddleWidget()
{

}

void MiddleWidget::initLayout()
{
    pageMain = new PageMain(this);
    pagePreviewLyric = new PagePreviewLyric(this);
    pageMatchNcmID = new PageMatchNcmID(this);
    pageLyricList = new PageLyricList(this);
    pageSetting = new PageSetting(this);

    stackPageLayout = new QStackedLayout(this);
    stackPageLayout->setMargin(0);
    stackPageLayout->addWidget(pageMain);
    stackPageLayout->addWidget(pagePreviewLyric);
    stackPageLayout->addWidget(pageMatchNcmID);
    stackPageLayout->addWidget(pageLyricList);
    stackPageLayout->addWidget(pageSetting);

}

void MiddleWidget::connectAll()
{

}
