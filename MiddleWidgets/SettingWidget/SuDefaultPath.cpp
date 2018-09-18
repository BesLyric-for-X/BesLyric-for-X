#include <global.h>
#include "SuDefaultPath.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

QString SuDefaultPath::getName()
{
   return tr("默认路径");
}

int SuDefaultPath::getUnitHeight()
{
   return 260;
}

QWidget *SuDefaultPath::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer");

    labelSettingTitle = new QLabel(SettingUnitContainer);
    labelSettingTitle->setObjectName("settingUnitTitle");
    labelSettingTitle->setText(tr("默认路径"));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelSettingTitle);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    btnSelectDefaultPathMusic  = new BesButton(SettingUnitContainer);
    btnSelectDefaultPathLyric  = new BesButton(SettingUnitContainer);
    btnSelectDefaultPathOutput = new BesButton(SettingUnitContainer);
    labelTipPathMusic  = new QLabel(SettingUnitContainer);
    labelTipPathLyric  = new QLabel(SettingUnitContainer);
    labelTipPathOutput = new QLabel(SettingUnitContainer);
    labelDefaultPathMusic = new QLabel(SettingUnitContainer);
    labelDefaultPathLyric = new QLabel(SettingUnitContainer);
    labelDefaultPathOutput= new QLabel(SettingUnitContainer);

    btnSelectDefaultPathMusic->setText(tr("选择"));
    btnSelectDefaultPathLyric->setText(tr("选择"));
    btnSelectDefaultPathOutput->setText(tr("选择"));
    btnSelectDefaultPathMusic->setMinimumSize(80,30);
    btnSelectDefaultPathMusic->setMaximumSize(80,30);
    btnSelectDefaultPathLyric->setMinimumSize(80,30);
    btnSelectDefaultPathLyric->setMaximumSize(80,30);
    btnSelectDefaultPathOutput->setMinimumSize(80,30);
    btnSelectDefaultPathOutput->setMaximumSize(80,30);
    btnSelectDefaultPathMusic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSelectDefaultPathLyric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSelectDefaultPathOutput->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    labelTipPathMusic->setText(tr("音乐目录："));
    labelTipPathLyric->setText(tr("歌词目录："));
    labelTipPathOutput->setText(tr("输出目录："));
    labelTipPathMusic->setMinimumSize(100,30);
    labelTipPathMusic->setMaximumSize(100,30);
    labelTipPathLyric->setMinimumSize(100,30);
    labelTipPathLyric->setMaximumSize(100,30);
    labelTipPathOutput->setMinimumSize(100,30);
    labelTipPathOutput->setMaximumSize(100,30);
    labelTipPathMusic->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelTipPathLyric->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelTipPathOutput->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    labelDefaultPathMusic->setMinimumHeight(30);
    labelDefaultPathMusic->setMaximumHeight(30);
    labelDefaultPathLyric->setMinimumHeight(30);
    labelDefaultPathLyric->setMaximumHeight(30);
    labelDefaultPathOutput->setMinimumHeight(30);
    labelDefaultPathOutput->setMaximumHeight(30);
    labelTipPathMusic->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    labelTipPathLyric->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    labelTipPathOutput->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(btnSelectDefaultPathMusic);
    hLayout2->addWidget(labelTipPathMusic);
    hLayout2->addWidget(labelDefaultPathMusic);

    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout3->addWidget(btnSelectDefaultPathLyric);
    hLayout3->addWidget(labelTipPathLyric);
    hLayout3->addWidget(labelDefaultPathLyric);

    QHBoxLayout* hLayout4 = new QHBoxLayout();
    hLayout4->addWidget(btnSelectDefaultPathOutput);
    hLayout4->addWidget(labelTipPathOutput);
    hLayout4->addWidget(labelDefaultPathOutput);

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addLayout(hLayout3);
    vLayout->addLayout(hLayout4);
    vLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    return SettingUnitContainer;
}
