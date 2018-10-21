#include <Define/Define.h>
#include "SettingWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include "BesScaleUtil.h"

SettingWidget::SettingWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    initLayout();
    initConnection();
}

SettingWidget::~SettingWidget()
{
}

void SettingWidget::initLayout()
{
    settingScrollPanel = new SuScrollPanel(this);
    settingLeftNavigator = new SuNavigator(settingScrollPanel->getSettingUnits(), this);

    settingLeftNavigator->setMouseTracking(true);   //详见 BesFramelessWidget.h 注释
    settingScrollPanel->setMouseTracking(true);     //详见 BesFramelessWidget.h 注释
    settingLeftNavigator->setObjectName("settingLeftNavigator");    //("settingLeftPanel");
    settingScrollPanel->setObjectName("settingScrollPanel");        //("settingRightPanel");
    settingLeftNavigator->setMinimumWidth(270 * BesScaleUtil::scale());
    settingLeftNavigator->setMaximumWidth(270 * BesScaleUtil::scale());
    settingLeftNavigator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);

    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->addWidget(settingLeftNavigator);
    hLayout->addWidget(settingScrollPanel);
    hLayout->setMargin(0);
}

void SettingWidget::initConnection()
{
    connect(settingLeftNavigator, SIGNAL(sig_scrollToSettingUnit(int)),
            settingScrollPanel, SLOT(OnScrollToIndex(int)));

    connect(settingScrollPanel, SIGNAL(sig_scrollPosChanged(int,int,int)),
            settingLeftNavigator, SLOT(OnSettingUnitPanelPosChanged(int,int,int)));

}
