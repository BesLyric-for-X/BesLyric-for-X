#include <global.h>
#include "SuLyricMaker.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QIntValidator>
#include "BesMessageBox.h"
#include "BesScaleUtil.h"

QString SuLyricMaker::getName()
{
   return tr("歌词制作");
}

int SuLyricMaker::getUnitHeight()
{
   return 150 * BesScaleUtil::scale();
}

QWidget *SuLyricMaker::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer");

    labelSettingTitle = new QLabel(SettingUnitContainer);
    labelSettingTitle->setObjectName("settingUnitTitle");
    labelSettingTitle->setText(tr("歌词制作"));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelSettingTitle);
    hLayout1->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    labelShiftBackwardTime = new QLabel(SettingUnitContainer);
    editShiftBackwardTime = new QLineEdit(SettingUnitContainer);
    btnEditShiftTime = new BesButton(SettingUnitContainer);

    editShiftBackwardTime->setValidator(new QIntValidator(0, 3000, this));

    labelShiftBackwardTime->setText(tr("提前毫秒数："));
    labelShiftBackwardTime->setMinimumSize(100* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    labelShiftBackwardTime->setMaximumSize(100* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    labelShiftBackwardTime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editShiftBackwardTime->setMinimumHeight(30* BesScaleUtil::scale());
    editShiftBackwardTime->setMaximumHeight(30* BesScaleUtil::scale());
    editShiftBackwardTime->setMaximumWidth(200* BesScaleUtil::scale());
    editShiftBackwardTime->setMinimumWidth(200* BesScaleUtil::scale());
    editShiftBackwardTime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnEditShiftTime->setMinimumSize(80* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    btnEditShiftTime->setMaximumSize(80* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    btnEditShiftTime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnEditShiftTime->setText(tr("修改"));

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(labelShiftBackwardTime);
    hLayout2->addWidget(editShiftBackwardTime);
    hLayout2->addWidget(btnEditShiftTime);
    hLayout2->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15* BesScaleUtil::scale());
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(20,20* BesScaleUtil::scale(), QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    //连接信号

    connect(editShiftBackwardTime,&QLineEdit::textChanged, [=](QString text){
        int time  = text.toInt();
        if(time == SettingManager::GetInstance().data().shiftTime)
            btnEditShiftTime->setEnabled(false);
        else
            btnEditShiftTime->setEnabled(true);
    });

    connect(btnEditShiftTime, &QPushButton::clicked, [=]()
    {
        int shiftTimeBackup = SettingManager::GetInstance().data().shiftTime;
        SettingManager::GetInstance().data().shiftTime = editShiftBackwardTime->text().toInt();
        if(SettingManager::GetInstance().saveSettingData())
        {
            BesMessageBox::information(tr("提示"),tr("保存成功"));
            btnEditShiftTime->setEnabled(false);
        }
        else
        {
            SettingManager::GetInstance().data().shiftTime = shiftTimeBackup;
            BesMessageBox::information(tr("提示"),tr("保存失败，可能是程序没有写权限"));
        }
    });

    editShiftBackwardTime->setText(QString().number(SettingManager::GetInstance().data().shiftTime));

    return SettingUnitContainer;
}
