#include <global.h>
#include <Define/Static.h>
#include "SuUpgrade.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "BesMessageBox.h"
#include "BesScaleUtil.h"

QString SuUpgrade::getName()
{
   return tr("升级");
}

int SuUpgrade::getUnitHeight()
{
   return 240* BesScaleUtil::mscale();
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
    hLayout1->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    checkboxAutoUpgrade = new QCheckBox(SettingUnitContainer);
    checkboxAutoUpgrade->setText(tr("自动检测新版本并提示"));

    btnCheckUpgrade = new BesButton(SettingUnitContainer);
    btnCheckUpgrade->setText(tr("检测更新"));
    btnCheckUpgrade->setMinimumSize(80* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnCheckUpgrade->setMaximumSize(80* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnCheckUpgrade->setSizePolicy( QSizePolicy::Fixed,  QSizePolicy::Fixed);

    labelCurrentVersionTip = new QLabel(SettingUnitContainer);
    labelCurrentVersion    = new QLabel(SettingUnitContainer);
    labelCurrentVersionTip->setText("当前版本号：");
    labelCurrentVersionTip->setMinimumSize(80* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    labelCurrentVersionTip->setMaximumSize(80* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    labelCurrentVersionTip->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelCurrentVersion->setMinimumSize(50* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    labelCurrentVersion->setMaximumSize(50* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    labelCurrentVersionTip->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelCurrentVersionTip->setSizePolicy( QSizePolicy::Fixed,  QSizePolicy::Fixed);
    labelCurrentVersion->setSizePolicy( QSizePolicy::Fixed,  QSizePolicy::Fixed);
    labelCurrentVersion->setText(VERSION_NUMBER);

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(checkboxAutoUpgrade);
    hLayout2->addSpacerItem(new QSpacerItem(120* BesScaleUtil::mscale(),20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout2->addWidget(labelCurrentVersionTip);
    hLayout2->addWidget(labelCurrentVersion);
    hLayout2->addSpacerItem(new QSpacerItem(10* BesScaleUtil::mscale(),40* BesScaleUtil::mscale(),QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout2->addWidget(btnCheckUpgrade);
    hLayout2->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15* BesScaleUtil::mscale());
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(20,20* BesScaleUtil::mscale(), QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

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
            BesMessageBox::information(tr("提示"),tr("保存失败，可能是程序没有写权限"));
        }
    });

    connect(&threadCheck, SIGNAL(sig_haveCheckResult(CheckUpgradeResult)), this,
            SLOT(onShowCheckResult(CheckUpgradeResult)));

    connect(btnCheckUpgrade, SIGNAL(clicked(bool)),this, SLOT(onCheckUpgrade()));

    return SettingUnitContainer;
}

void SuUpgrade::onCheckUpgrade()
{
    threadCheck.checkForUpdate(false, QThread::HighestPriority);
}

void SuUpgrade::onShowCheckResult(CheckUpgradeResult result)
{
    QString strUpdate;

    switch(result.nResult)
    {
    case -1:
        BesMessageBox::information(tr("提示"),tr("无法连接网络"));
        break;
    case 0:
        BesMessageBox::information(tr("提示"),tr("服务端数据出错"));
        break;
    case 1:
        BesMessageBox::information(tr("提示"),tr("当前版本已是最新版本"));
        break;
    case 2:

        if(result.infoList.size() == 0)
            strUpdate = "没有具体的更新信息";
        else
        {
            strUpdate += "<p>";
            for(auto line: result.infoList)
            {
                strUpdate += line;
                strUpdate += "<br/>";
            }
            strUpdate += "</p>";
        }

        BesMessageBox::information(tr("有新版本") + result.versionNum, strUpdate);
        break;
    }

}
