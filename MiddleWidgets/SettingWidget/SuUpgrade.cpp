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
   return 200;
}

QWidget *SuUpgrade::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer");

    labelSettingTitle = new QLabel(SettingUnitContainer);
    labelSettingTitle->setObjectName("settingTitle");
    labelSettingTitle->setText(tr("升级"));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelSettingTitle);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    labelShiftBackwardTime = new QLabel(SettingUnitContainer);
    editShiftBackwardTime = new QLineEdit(SettingUnitContainer);
    btnEditShiftTime = new BesButton(SettingUnitContainer);

    labelShiftBackwardTime->setText(tr("提前毫秒数："));
    labelShiftBackwardTime->setMinimumSize(100,30);
    labelShiftBackwardTime->setMaximumSize(100,30);
    labelShiftBackwardTime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editShiftBackwardTime->setMinimumHeight(30);
    editShiftBackwardTime->setMaximumHeight(30);
    editShiftBackwardTime->setMaximumWidth(200);
    editShiftBackwardTime->setMinimumWidth(200);
    editShiftBackwardTime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnEditShiftTime->setMinimumSize(80,30);
    btnEditShiftTime->setMaximumSize(80,30);
    btnEditShiftTime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnEditShiftTime->setText(tr("修改"));

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(labelShiftBackwardTime);
    hLayout2->addWidget(editShiftBackwardTime);
    hLayout2->addWidget(btnEditShiftTime);
    hLayout2->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    return SettingUnitContainer;
}
