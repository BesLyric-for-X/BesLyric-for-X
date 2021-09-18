#include <global.h>
#include "SuSoftware.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <Define/Static.h>
#include "BesScaleUtil.h"
#include <QUrl>

QString SuSoftware::getName()
{
   return tr("关于本软件");
}

int SuSoftware::getUnitHeight()
{
   return 520 * BesScaleUtil::scale();
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
    hLayout1->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    QLabel* label1 = new QLabel(SettingUnitContainer);
    QLabel* label2 = new QLabel(SettingUnitContainer);
    QLabel* label3 = new QLabel(SettingUnitContainer);
    QLabel* label4 = new QLabel(SettingUnitContainer);
    QLabel* label5 = new QLabel(SettingUnitContainer);
    QLabel* label6 = new QLabel(SettingUnitContainer);
    QLabel* label7 = new QLabel(SettingUnitContainer);

    label1->setObjectName("labelSettingHighColor");
    label2->setObjectName("labelSettingHighColor");
    label3->setObjectName("labelSettingHighColor");
    label4->setObjectName("labelSettingHighColor");
    label5->setObjectName("labelSettingHighColor");
    label6->setObjectName("labelSettingHighColor");
    label7->setObjectName("labelSettingHighColor");

    label1->setText(tr("<p style='line-height:130%'>本软件名为 BesLyric。BesLyric 是一款 <a style='color:#666666;' href='https://blog.csdn.net/testcs_dn/article/details/37722355'>免费且开源</a> 的软件，我们开发本软件的目的，是为了提供给用户一个 上手简单、功能实用的 滚动歌词制作软件，为广大网易云音乐爱好者提供一个良好的歌词制作体验。</p>"));

    label2->setText(tr("软件使用说明: <a style='color:#666666;' href='https://www.cnblogs.com/BensonLaur/p/9695769.html'>点击这里</a>"));

    label3->setText(tr("<p style='line-height:130%'>软件用户交流: <a style='color:#666666;' href='https://jq.qq.com/?_wv=1027&k=5Uc1a79'>QQ群 1021317114</a></p>"));

    QString OSName = QString{"%1 (%2, %3, %4)"}
            .arg(QSysInfo::prettyProductName(),
                 QSysInfo::kernelType(),
                 QSysInfo::kernelVersion(),
                 QSysInfo::currentCpuArchitecture());
    QString BesLyricVersion = VERSION_NUMBER;
    QString CommitHash = GIT_COMMIT_SHA1;
    QString QtVersion = QT_VERSION_STR;
    QString IssueBody = QString(
    "### 运行版本和环境信息\n"
    "操作系统: %1\n"
    "BesLyric 版本号: %2\n"
    "提交哈希值: %3\n"
    "Qt 版本号: %4\n"
    "\n"
    "### 问题总述\n"
    "(在这里写问题概述)\n"
    "\n"
    "### 可复现性\n"
    "问题是否可复现: 每次都会 / 基本大部分时候会 / 有时会 / 很少会 / 无法复现\n"
    "\n"
    "### 问题复现步骤\n"
    "1. \n"
    "2. \n"
    "3. …\n"
    "\n"
    "### 观察到的现象\n"
    "\n"
    "\n"
    "### 预期现象\n"
    "\n"
    "\n"
    "### 额外信息\n"
    "(在这里放额外的链接或截图等)\n"
    "\n"
    "--------\n"
    "请在提交前删除任何不必要的模板部分\n").arg(OSName).arg(BesLyricVersion).arg(CommitHash).arg(QtVersion);

    label4->setText(QString(tr("<p style='line-height:130%'>使用问题反馈: <a style='color:#666666;' href='"
                       "https://github.com/BesLyric-for-X/BesLyric-for-X/issues/new?body=%1'>"
                       "在 GitHub 的 Issues 中进行反馈</a></p>")).arg(QUrl::toPercentEncoding(IssueBody).constData()));

    label5->setText(tr("<p style='line-height:130%'>软件实现代码: <a style='color:#666666;' href='https://github.com/BesLyric-for-X/BesLyric-for-X'>GitHub源码 (C++/Qt 项目)</a></p>"));

    QString GithubCommitLink = "https://github.com/BesLyric-for-X/BesLyric-for-X/commit/" + CommitHash;
    label6->setText(QString(tr("<p style='line-height:130%'>提交的哈希值: <a style='color:#666666;' href='%1'>%2</a></p>")).arg(GithubCommitLink).arg(CommitHash));

    label7->setText(tr("<p style='line-height:130%'>软件图标来源: <a style='color:#666666;' href='https://www.easyicon.net/1186871-netease_cloudmusic_icon.html'>Netease Cloudmusic icons - OPPO Color OS Icons - Easyicon</a></p>"));

    label2->setOpenExternalLinks(true);
    label3->setOpenExternalLinks(true);
    label4->setOpenExternalLinks(true);
    label5->setOpenExternalLinks(true);
    label6->setOpenExternalLinks(true);
    label7->setOpenExternalLinks(true);

    label1->setWordWrap(true);

    QVBoxLayout* vLayout1 = new QVBoxLayout();
    vLayout1->addWidget(label1);
    vLayout1->addWidget(label2);
    vLayout1->addWidget(label3);
    vLayout1->addWidget(label4);
    vLayout1->addWidget(label5);
    vLayout1->addWidget(label6);
    vLayout1->addWidget(label7);

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15* BesScaleUtil::scale());
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(vLayout1);
    vLayout->addSpacerItem(new QSpacerItem(20,20* BesScaleUtil::scale(), QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    return SettingUnitContainer;
}
