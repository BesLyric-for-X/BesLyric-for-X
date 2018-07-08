#include "MainWidget.h"
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
    initLayout();
    connectAll();
}

MainWidget::~MainWidget()
{

}

void MainWidget::initLayout()
{
    topWidget = new TopWidget(this);
    middleWidget = new MiddleWidget(this);
    bottomWidget = new BottomWidget(this);
    topWidget->setObjectName("topWidget");
    middleWidget->setObjectName("middleWidget");
    bottomWidget->setObjectName("bottomWidget");

    topWidget->setMinimumHeight(60);
    topWidget->setMaximumHeight(60);
    topWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    bottomWidget->setMinimumHeight(60);
    bottomWidget->setMaximumHeight(60);
    bottomWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    vLayout->addWidget(topWidget);
    vLayout->addWidget(middleWidget);
    vLayout->addWidget(bottomWidget);
}

void MainWidget::connectAll()
{

}
