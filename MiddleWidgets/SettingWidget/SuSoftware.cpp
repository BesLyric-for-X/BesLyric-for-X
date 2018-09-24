#include <global.h>
#include "SuSoftware.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <Define/Static.h>

QString SuSoftware::getName()
{
   return tr("关于本软件");
}

int SuSoftware::getUnitHeight()
{
   return 350;
}

QWidget *SuSoftware::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer");

    labelSettingTitle = new QLabel(SettingUnitContainer);
    labelSettingTitle->setObjectName("settingUnitTitle");
    labelSettingTitle->setText(tr("关于本软件"));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelSettingTitle);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QLabel* label1 = new QLabel(SettingUnitContainer);
    QLabel* label2 = new QLabel(SettingUnitContainer);
    QLabel* label3 = new QLabel(SettingUnitContainer);
    QLabel* label4 = new QLabel(SettingUnitContainer);

    label1->setObjectName("labelSettingHighColor");
    label2->setObjectName("labelSettingHighColor");
    label3->setObjectName("labelSettingHighColor");
    label4->setObjectName("labelSettingHighColor");

    label1->setText(tr("本软件 名称：BesLyric ") + " ( "+VERSION_NAME + " )");

    label2->setText(tr("<p style='line-height:130%'>本软件 试图提供给用户一个 上手简单、功能实用的 滚动歌词制作软件，为广大网易云音乐爱好者提供一个良好的歌词制作体验</p>"));

    label3->setText(tr("本软件 详细使用说明 以及 解决常见问题 : <a style='color:#666666;' href='http://www.cnblogs.com/BensonLaur/p/6262565.html'>点击这里</a>"));

    label4->setText(tr("<p style='line-height:130%'>本软件 <a style='color:#666666;' href='https://blog.csdn.net/testcs_dn/article/details/37722355'>自由开源</a>"
    " ，License 为 GPL，使用跨平台框架 C++ 跨平台框架 QT 实现，对软件实现感兴趣的童鞋可以在此查看所有实现代码："
    "<a style='color:#666666;' href='https://github.com/BensonLaur/Beslyric-for-X'>GitHub 源码</a></p>"));

    label3->setOpenExternalLinks(true);
    label4->setOpenExternalLinks(true);

    label2->setWordWrap(true);
    label4->setWordWrap(true);

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    vLayout1->addWidget(label1);
    vLayout1->addWidget(label2);
    vLayout1->addWidget(label3);
    vLayout1->addWidget(label4);

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(vLayout1);
    vLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    return SettingUnitContainer;
}
