#include <global.h>
#include "SuMusicDownload.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFileDialog>
#include "BesMessageBox.h"

QString SuMusicDownload::getName()
{
   return tr("歌曲下载");
}

int SuMusicDownload::getUnitHeight()
{
   return 230;
}

QWidget *SuMusicDownload::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer");

    labelSettingTitle = new QLabel(SettingUnitContainer);
    labelSettingTitle->setObjectName("settingUnitTitle");
    labelSettingTitle->setText(tr("歌曲下载"));

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelSettingTitle);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    btnSelectMusicDownloadPath = new BesButton(SettingUnitContainer);
    labelMusicDownloadPathTip = new QLabel(SettingUnitContainer);
    labelMusicDownloadPath = new QLabel(SettingUnitContainer);

    btnSelectMusicDownloadPath->setText(tr("选择"));
    labelMusicDownloadPathTip->setText(tr("歌曲下载路径："));

    btnSelectMusicDownloadPath->setMinimumSize(80,30);
    btnSelectMusicDownloadPath->setMaximumSize(80,30);
    btnSelectMusicDownloadPath->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelMusicDownloadPathTip->setMinimumSize(120,30);
    labelMusicDownloadPathTip->setMaximumSize(120,30);
    labelMusicDownloadPathTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelMusicDownloadPath->setMinimumHeight(30);
    labelMusicDownloadPath->setMaximumHeight(30);
    labelMusicDownloadPath->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    checkboxMusicDownload = new QCheckBox(SettingUnitContainer);
    checkboxMusicDownload->setText(tr("我已查看并同意《 BesLyric 音乐下载声明 》 "));

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(btnSelectMusicDownloadPath);
    hLayout2->addWidget(labelMusicDownloadPathTip);
    hLayout2->addWidget(labelMusicDownloadPath);

    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout3->addWidget(checkboxMusicDownload);
    hLayout3->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::MinimumExpanding ,QSizePolicy::Fixed));

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(20,15, QSizePolicy::Fixed ,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout3);
    vLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    labelMusicDownloadPath->setText(SettingManager::GetInstance().data().musicDowloadPath);

    //连接信号

    connect(btnSelectMusicDownloadPath, &QPushButton::clicked, [=]()
    {
        QString backup = SettingManager::GetInstance().data().musicDowloadPath;

        QString dir = QFileDialog::getExistingDirectory(parent, tr("选择歌曲下载路径"),  backup,
                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

        if(dir.size() !=0)
        {
            labelMusicDownloadPath->setText(dir);

            SettingManager::GetInstance().data().musicDowloadPath = dir;

            if(!SettingManager::GetInstance().saveSettingData())
            {
                SettingManager::GetInstance().data().musicDowloadPath = backup;
                labelMusicDownloadPath->setText(backup);
                BesMessageBox::information(tr("提示"),tr("保存失败"));
            }
        }
    });


    return SettingUnitContainer;
}
