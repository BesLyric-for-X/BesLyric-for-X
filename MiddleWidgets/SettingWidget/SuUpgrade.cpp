#include <global.h>
#include "SuUpgrade.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "BesMessageBox.h"

QString SuUpgrade::getName()
{
   return tr("升级");
}

int SuUpgrade::getUnitHeight()
{
   return 240;
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
    checkboxAutoUpgrade->setText(tr("自动检测新版本并提示"));

    btnCheckUpgrade = new BesButton(SettingUnitContainer);
    btnCheckUpgrade->setText(tr("检测更新"));
    btnCheckUpgrade->setMinimumSize(80,30);
    btnCheckUpgrade->setMaximumSize(80,30);
    btnCheckUpgrade->setSizePolicy( QSizePolicy::Fixed,  QSizePolicy::Fixed);

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
    hLayout2->addSpacerItem(new QSpacerItem(20,40,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout2->addWidget(btnCheckUpgrade);
    hLayout2->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    checkboxAutoUpgrade->setChecked(SettingManager::GetInstance().data().autoCheckForUpgrade);

    //连接信号

    connect(checkboxAutoUpgrade, &QPushButton::toggled, [=]()
    {
        bool backup = SettingManager::GetInstance().data().autoCheckForUpgrade;

        SettingManager::GetInstance().data().autoCheckForUpgrade = checkboxAutoUpgrade->isChecked();
        if(!SettingManager::GetInstance().saveSettingData())
        {
            SettingManager::GetInstance().data().autoCheckForUpgrade = backup;
            checkboxAutoUpgrade->setChecked(backup);
            BesMessageBox::information(tr("提示"),tr("保存失败"));
        }
    });

    return SettingUnitContainer;
}
