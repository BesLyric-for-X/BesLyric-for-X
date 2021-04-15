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
   return tr("捐款");
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
    labelSettingTitle->setText(tr("捐款"));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelSettingTitle);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QLabel* label1 = new QLabel(SettingUnitContainer);
    QLabel* label2 = new QLabel(SettingUnitContainer);
    QLabel* label3 = new QLabel(SettingUnitContainer);
    QLabel* labelDownloadTip = new QLabel(SettingUnitContainer);
    BesButton* btnDownloadWindow = new BesButton(SettingUnitContainer);
    BesButton* btnDownloadLinux= new BesButton(SettingUnitContainer);
    BesButton* btnDownloadMac = new BesButton(SettingUnitContainer);

    btnDownloadWindow->setText("Windows版");
    btnDownloadLinux->setText("Linux版");
    btnDownloadMac->setText("Mac版");
    btnDownloadWindow->setObjectName("btnDownloadWindow");
    btnDownloadLinux->setObjectName("btnDownloadLinux");
    btnDownloadMac->setObjectName("btnDownloadMac");

    btnDownloadWindow->setMinimumSize(100,30);
    btnDownloadWindow->setMaximumSize(100,30);
    btnDownloadLinux->setMinimumSize(90,30);
    btnDownloadLinux->setMaximumSize(90,30);
    btnDownloadMac->setMinimumSize(80,30);
    btnDownloadMac->setMaximumSize(80,30);

    label1->setObjectName("labelSettingHighColor");
    label2->setObjectName("labelSettingHighColor");
    label3->setObjectName("labelSettingHighColor");

    label1->setText(tr("<p style='line-height:130%'>如果喜欢本软件，欢迎查看 <a style='color:#666666;' href='http://www.cnblogs.com/BensonLaur/p/6262565.html#title7'>捐款页面</a></p>"));

    label2->setText(tr("<p style='line-height:130%'>本软件原为 <a style='color:#666666;' href='https://www.cnblogs.com/BensonLaur/p/6262565.html'> BesLyric </a>, "
                       "自发布一年余中，在广大网友的建议和默默支持下不断发展，可惜由于开发框架限制，只能在windows下运行，效果也不是很理想。</p>"));
    label3->setText(tr("<p style='line-height:130%'>也是在此背景下，BesLyric-for-X 应运而生，使用了跨平台框架实现，支持 Windows、Linux (Debian)、Mac Os。"
                       "另外，强大的应用基础框架使得实现效果更加地“网易云”，以提供更好的使用体验！</p>"));


    label1->setOpenExternalLinks(true);
    label2->setOpenExternalLinks(true);
    label1->setWordWrap(true);
    label2->setWordWrap(true);
    label3->setWordWrap(true);


    labelDownloadTip->setText(tr("<p style='line-height:130%'>不同版本下载</p>"));

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(labelDownloadTip);
    hLayout2->addWidget(btnDownloadWindow);
    hLayout2->addWidget(btnDownloadLinux);
    hLayout2->addWidget(btnDownloadMac);
    hLayout2->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QVBoxLayout* vLayoutText = new QVBoxLayout();
    vLayoutText->addWidget(label1);
    vLayoutText->addSpacerItem(new QSpacerItem(20,10, QSizePolicy::Fixed ,QSizePolicy::Fixed));
    vLayoutText->addWidget(label2);
    vLayoutText->addWidget(label3);
    vLayoutText->addLayout(hLayout2);

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(vLayoutText);
    vLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));


    connect(btnDownloadWindow, &BesButton::clicked, this, &SuDonation::onDownloadProgram);
    connect(btnDownloadLinux, &BesButton::clicked, this, &SuDonation::onDownloadProgram);
    connect(btnDownloadMac, &BesButton::clicked, this, &SuDonation::onDownloadProgram);

    return SettingUnitContainer;
}

void SuDonation::onDownloadProgram()
{
    QString strLink = "https://www.cnblogs.com/BensonLaur/p/9695769.html#title2";
    QDesktopServices::openUrl(QUrl(strLink));
}
