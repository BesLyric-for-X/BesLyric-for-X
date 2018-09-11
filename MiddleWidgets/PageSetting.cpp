#include <Define/Define.h>
#include "PageSetting.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>

PageSetting::PageSetting(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    initLayout();
    initConnection();
}

PageSetting::~PageSetting()
{

}

void PageSetting::initLayout()
{
    QGridLayout* mainLayout = new QGridLayout(this);
    pageSettingContainer = new QWidget(this);
    pageSettingContainer->setObjectName("pageSettingContainer");
    pageSettingContainer->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(pageSettingContainer);


    QVBoxLayout* vMainLayout = new QVBoxLayout(pageSettingContainer);

    settingTopPanel = new QWidget(pageSettingContainer);
    settingTopPanel->setMaximumHeight(65);
    settingTopPanel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    settingTopPanel->setObjectName("settingTopPanel");
    settingTopPanel->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    settingTitle = new QLabel(settingTopPanel);
    btnPackupSetting = new BesButton(settingTopPanel);
    settingTitle->setObjectName("settingTitle");
    btnPackupSetting->setObjectName("btnPackupSetting");
    settingTitle->setText(tr("设置"));

    QHBoxLayout* hSettingLayout = new QHBoxLayout(settingTopPanel);
    hSettingLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hSettingLayout->addWidget(settingTitle);
    hSettingLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    hSettingLayout->addWidget(btnPackupSetting);
    hSettingLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::Fixed));

    settingLeftPanel = new QWidget(pageSettingContainer);
    settingRightPanel = new QWidget(pageSettingContainer);
    settingLeftPanel->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    settingRightPanel->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    settingLeftPanel->setObjectName("settingLeftPanel");
    settingRightPanel->setObjectName("settingRightPanel");
    settingLeftPanel->setMinimumWidth(270);
    settingLeftPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    settingRightPanel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QHBoxLayout* hMainLayout = new QHBoxLayout();
    hMainLayout->addWidget(settingLeftPanel);
    hMainLayout->addWidget(settingRightPanel);
    hMainLayout->setMargin(0);
    hMainLayout->setSpacing(0);

    vMainLayout->setMargin(0);
    vMainLayout->setSpacing(0);
    vMainLayout->addWidget(settingTopPanel);
    vMainLayout->addLayout(hMainLayout);

}

void PageSetting::initConnection()
{

}
