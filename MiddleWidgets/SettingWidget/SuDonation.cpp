#include <global.h>
#include "SuDonation.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QDesktopServices>
#include <QUrl>
#include "BesScaleUtil.h"

QString SuDonation::getName()
{
   return tr("打赏");
}

int SuDonation::getUnitHeight()
{
   return 450* BesScaleUtil::scale();
}

QWidget *SuDonation::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer");

    labelSettingTitle = new QLabel(SettingUnitContainer);
    labelSettingTitle->setObjectName("settingUnitTitle");
    labelSettingTitle->setText(tr("打赏"));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelSettingTitle);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QLabel* label1 = new QLabel(SettingUnitContainer);
    QLabel* label2 = new QLabel(SettingUnitContainer);
    QLabel* label3 = new QLabel(SettingUnitContainer);

    label1->setObjectName("labelSettingHighColor");
    label2->setObjectName("labelSettingHighColor");
    label3->setObjectName("labelSettingHighColor");

    label1->setText(tr("<p style='line-height:130%'>本软件的设计和开发源自 <a style='color:#666666;' href='https://www.cnblogs.com/BensonLaur/p/6262565.html'> BesLyric(使用SOUI界面框架实现) </a>, "
                       "自其发布后的一年余中，在广大网友的建议和默默支持下不断发展，可惜由于开发框架限制，只能在 Windows 下运行，效果也不是很理想。</p>"));
    label2->setText(tr("<p style='line-height:130%'>也是在此背景下，<a style='color:#666666;' href='https://github.com/BesLyric-for-X/BesLyric-for-X'>BesLyric-for-X</a> 应运而生。新版本使用了跨平台框架实现，支持 Windows、Linux(Ubuntu)、macOS 操作系统。"
                       "另外，强大的应用基础框架使得新版本软件的实现效果更加地“网易云”，以为用户提供更好的使用体验！</p>"));

    label3->setText(tr("<p style='line-height:130%'>您的喜欢和支持，是本软件不断发展的动力，如果喜欢本软件，欢迎访问 <a style='color:#666666;' href='http://www.cnblogs.com/BensonLaur/p/6262565.html#title7'>打赏页面</a></p>"));

    label1->setOpenExternalLinks(true);
    label2->setOpenExternalLinks(true);
    label3->setOpenExternalLinks(true);
    label1->setWordWrap(true);
    label2->setWordWrap(true);
    label3->setWordWrap(true);

    QVBoxLayout* vLayoutText = new QVBoxLayout();
    vLayoutText->addWidget(label1);
    vLayoutText->addWidget(label2);
    vLayoutText->addSpacerItem(new QSpacerItem(20,10, QSizePolicy::Fixed ,QSizePolicy::Fixed));
    vLayoutText->addWidget(label3);

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(vLayoutText);
    vLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    return SettingUnitContainer;
}
