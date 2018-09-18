#include <global.h>
#include "SuUpgrade.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

QString SuUpgrade::getName()
{
   return tr("升级");
}

int SuUpgrade::getUnitHeight()
{
   return 150;
}

QWidget *SuUpgrade::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer");

    labelSettingTitle = new QLabel(SettingUnitContainer);
    labelSettingTitle->setObjectName("settingUnitTitle");
    labelSettingTitle->setText(tr("升级"));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelSettingTitle);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    checkboxAutoUpgrade = new QCheckBox(SettingUnitContainer);
    checkboxAutoUpgrade->setText(tr("自动升级到最新版本"));

    labelCurrentVersionTip = new QLabel(SettingUnitContainer);
    labelCurrentVersion    = new QLabel(SettingUnitContainer);
    labelCurrentVersionTip->setText("当前版本号：");
    labelCurrentVersionTip->setMinimumSize(100,30);
    labelCurrentVersionTip->setMaximumSize(100,30);
    labelCurrentVersion->setMinimumSize(100,30);
    labelCurrentVersion->setMaximumSize(100,30);
    labelCurrentVersionTip->setSizePolicy( QSizePolicy::Fixed,  QSizePolicy::Fixed);
    labelCurrentVersion->setSizePolicy( QSizePolicy::Fixed,  QSizePolicy::Fixed);

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(checkboxAutoUpgrade);
    hLayout2->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout2->addWidget(labelCurrentVersion);
    hLayout2->addWidget(labelCurrentVersionTip);
    hLayout2->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    return SettingUnitContainer;
}
