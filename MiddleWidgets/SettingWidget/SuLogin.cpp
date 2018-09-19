#include <global.h>
#include "SuLogin.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "BesMessageBox.h"

QString SuLogin::getName()
{
   return tr("匿名登录");
}

int SuLogin::getUnitHeight()
{
   return 150;
}

QWidget *SuLogin::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer");

    labelSettingTitle = new QLabel(SettingUnitContainer);
    labelSettingTitle->setObjectName("settingUnitTitle");
    labelSettingTitle->setText(tr("匿名登录"));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelSettingTitle);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));


    checkboxLoginAnonymously = new QCheckBox(SettingUnitContainer);
    checkboxLoginAnonymously->setText(tr("匿名登录"));

    linkWhatIsLoginAnonymously= new QLabel(SettingUnitContainer);
    linkWhatIsLoginAnonymously->setText(tr("<a style='color:#666666;' href='http://www.cnblogs.com/BensonLaur/p/6262565.html#title_4_4'>什么是匿名登录?</a>"));
    linkWhatIsLoginAnonymously->setOpenExternalLinks(true);

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(checkboxLoginAnonymously);
    hLayout2->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout2->addWidget(linkWhatIsLoginAnonymously);
    hLayout2->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    checkboxLoginAnonymously->setChecked(SettingManager::GetInstance().data().loginAnonymously);

    //连接信号

    connect(checkboxLoginAnonymously, &QPushButton::toggled, [=]()
    {
        bool backup = SettingManager::GetInstance().data().loginAnonymously;

        SettingManager::GetInstance().data().loginAnonymously = checkboxLoginAnonymously->isChecked();
        if(!SettingManager::GetInstance().saveSettingData())
        {
            SettingManager::GetInstance().data().loginAnonymously = backup;
            checkboxLoginAnonymously->setChecked(backup);
            BesMessageBox::information(tr("提示"),tr("保存失败"));
        }
    });



    return SettingUnitContainer;
}
