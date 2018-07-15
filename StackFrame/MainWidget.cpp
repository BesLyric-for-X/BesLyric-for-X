#include "MainWidget.h"
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);

    initEntity();
    initLayout();
    connectAll();
}

MainWidget::~MainWidget()
{

}

void MainWidget::initEntity()
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

    topWidget->setMinimumHeight(65);
    topWidget->setMaximumHeight(65);
    topWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    bottomWidget->setMinimumHeight(65);
    bottomWidget->setMaximumHeight(65);
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
