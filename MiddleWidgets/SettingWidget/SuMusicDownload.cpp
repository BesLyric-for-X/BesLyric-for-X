#include <global.h>
#include "SuMusicDownload.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFileDialog>
#include <QDateTime>
#include "BesMessageBox.h"
#include "BesScaleUtil.h"

QString SuMusicDownload::getName()
{
   return tr("歌曲下载");
}

int SuMusicDownload::getUnitHeight()
{
   return 340* BesScaleUtil::mscale();
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
    hLayout1->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    btnSelectMusicDownloadPath = new BesButton(SettingUnitContainer);
    labelMusicDownloadPathTip = new QLabel(SettingUnitContainer);
    labelMusicDownloadPath = new QLabel(SettingUnitContainer);

    btnSelectMusicDownloadPath->setText(tr("选择"));
    labelMusicDownloadPathTip->setText(tr("歌曲下载目录："));

    btnSelectMusicDownloadPath->setMinimumSize(80* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnSelectMusicDownloadPath->setMaximumSize(80* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnSelectMusicDownloadPath->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelMusicDownloadPathTip->setMinimumSize(120* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    labelMusicDownloadPathTip->setMaximumSize(120* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    labelMusicDownloadPathTip->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelMusicDownloadPath->setMinimumHeight(30* BesScaleUtil::mscale());
    labelMusicDownloadPath->setMaximumHeight(30* BesScaleUtil::mscale());
    labelMusicDownloadPath->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    checkboxMusicDownload = new QCheckBox(SettingUnitContainer);
    checkboxMusicDownload->setText(tr("我已查看并同意《 BesLyric 音乐下载声明 》 "));

    labelNameFormat = new QLabel(SettingUnitContainer);
    labelNameFormat->setText(tr("命名格式："));
    labelNameFormat->setMinimumHeight(30* BesScaleUtil::mscale());
    labelNameFormat->setMaximumHeight(30* BesScaleUtil::mscale());
    radioFormatArtistSong = new QRadioButton(tr("歌手 - 歌曲名"));
    radioFormatSongArtist = new QRadioButton(tr("歌曲名 - 歌手"));
    radioFormatArtistSong->setMinimumHeight(30* BesScaleUtil::mscale());
    radioFormatArtistSong->setMaximumHeight(30* BesScaleUtil::mscale());
    radioFormatSongArtist->setMinimumHeight(30* BesScaleUtil::mscale());
    radioFormatSongArtist->setMaximumHeight(30* BesScaleUtil::mscale());
    groupNameFormat = new QButtonGroup(SettingUnitContainer);

    groupNameFormat->addButton(radioFormatArtistSong, ARTIST_SONG);
    groupNameFormat->addButton(radioFormatSongArtist, SONG_ARTIST);
    SettingManager::GetInstance().data().nameFormatStyle == NameFormatStyle::SONG_ARTIST?
    radioFormatSongArtist->setChecked(true):radioFormatArtistSong->setChecked(true);

    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(btnSelectMusicDownloadPath);
    hLayout2->addWidget(labelMusicDownloadPathTip);
    hLayout2->addWidget(labelMusicDownloadPath);

    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout3->addWidget(checkboxMusicDownload);
    hLayout3->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20, QSizePolicy::MinimumExpanding ,QSizePolicy::Fixed));

    QVBoxLayout* vLayout4 = new QVBoxLayout();
    vLayout4->addWidget(labelNameFormat);
    vLayout4->addWidget(radioFormatArtistSong);
    vLayout4->addWidget(radioFormatSongArtist);

    QHBoxLayout* hLayout4 = new QHBoxLayout();
    hLayout4->addLayout(vLayout4);
    hLayout4->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20, QSizePolicy::MinimumExpanding ,QSizePolicy::Minimum));

    QVBoxLayout* vLayout = new QVBoxLayout(SettingUnitContainer);
    vLayout->setSpacing(15);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    vLayout->addSpacerItem(new QSpacerItem(20,15* BesScaleUtil::mscale(), QSizePolicy::Fixed ,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout3);
    vLayout->addSpacerItem(new QSpacerItem(20,15* BesScaleUtil::mscale(), QSizePolicy::Fixed ,QSizePolicy::Fixed));
    vLayout->addLayout(hLayout4);
    vLayout->addSpacerItem(new QSpacerItem(20,40* BesScaleUtil::mscale(), QSizePolicy::Fixed ,QSizePolicy::MinimumExpanding));

    //如果不同意下载声明，下载路径置空
    if(!SettingManager::GetInstance().data().agreeDownloadDeclaration)
        SettingManager::GetInstance().data().musicDowloadPath = "";

    checkboxMusicDownload->setChecked(SettingManager::GetInstance().data().agreeDownloadDeclaration);
    labelMusicDownloadPath->setText(SettingManager::GetInstance().data().musicDowloadPath);

    //连接信号

    connect(btnSelectMusicDownloadPath, &QPushButton::clicked, [=]()
    {
        if(!SettingManager::GetInstance().data().agreeDownloadDeclaration)
        {
            BesMessageBox::information(tr("提示"),tr("下载歌曲需要先 勾选【我已查看并同意《BesLyric 音乐下载声明》】"));
            return;
        }


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
                BesMessageBox::information(tr("提示"),tr("保存失败，可能是程序没有写权限"));
            }
        }
    });

    connect(checkboxMusicDownload, &QCheckBox::clicked,[=]()
    {
        bool checkBackup = SettingManager::GetInstance().data().agreeDownloadDeclaration;

        bool agree = false;
        if(checkboxMusicDownload->isChecked())
        {

            QString lineHeight = "line-height:150%;";

//mac 平台样式实现效果不同，必须为 100%，否则会有超出部分看不到
#ifdef Q_OS_MAC
            lineHeight = "line-height:100%;";
#endif

            int nCount = 0;
            while(true)
            {
            nCount++;
            qint64 timestampBegin = QDateTime::currentDateTime().toMSecsSinceEpoch();

            if( QMessageBox::StandardButton::Ok ==
            BesMessageBox::question(tr("《BesLyric 音乐下载声明》"),
            tr("<p style=\"%1\">亲爱的 Beslyric 用户，感谢使用 Beslyric, 请花费您宝贵的【30秒】阅读本说明：\n</p>"
               "<p style=\"%2\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
               "本软件为 <a style='color:#666666;' href='https://blog.csdn.net/testcs_dn/article/details/37722355'>自由开源</a> 软件，是一款专门制作网易云音乐滚动歌词的软件。正是由于你们的默默支持，才使得本软件不断朝着“上手简单、功能实用”的目标发展更新，为广大网易云音乐爱好者提供一个良好的歌词制作体验。\n</p>"
               "<p style=\"%3\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
               "需要告诉你的是，从本软件下载的音乐，仅仅是以 方便用户制作歌词 为目的。音乐作品是音乐人辛勤劳动的成果，对正版音乐的支持，能够鼓励优秀的音乐人继续进行音乐创作，促进音乐生态良好地运作，最终使 音乐人 和 您 从中受益。</p>"
               "<p style=\"%4\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
               "如果您喜欢音乐人作品，请到拥有其版权的平台购买音乐，感谢您为音乐所做的付出！\n</p>"

               "<p style=\"%5\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
               "如需了解版权相关内容，可参考&nbsp;&nbsp;"
               "<a style='color:#666666;' href='http://www.ipcenter.fudan.edu.cn/5530/list.htm'>《版权法》</a>&nbsp;&nbsp;"
               "<a style='color:#666666;' href='http://www.ipcenter.fudan.edu.cn/_upload/article/9f/2b/8b7f82004392ac29f4974f7590e0/ce326ff7-6d52-4fbc-a7c6-59eb89867e9d.pdf'>《信息网络传播权保护条例》</a>"
               "</p>"

                "<p style=\"text-align:right;color:#666666;\">by 开发者 <a href=\"https://www.cnblogs.com/BensonLaur/\" target=\"_blank\" style=\"color:#666666;\">BensonLaur</a></p>"

               "<p style=\"%6text-align:center;\">对于以上说明，您是否表示 理解并同意？	</p>").arg(lineHeight).arg(lineHeight).arg(lineHeight).arg(lineHeight).arg(lineHeight).arg(lineHeight),
                      tr("是"), tr("否"), false, true)
                    )
            {
                qint64 timestampEnd = QDateTime::currentDateTime().toMSecsSinceEpoch();

                qint64 diff =timestampEnd-timestampBegin;

                bool bReadAgain = false;
                if(diff < 30000)
                {
                    if(nCount == 101)//用户已经点了100下了，手动再见
                    {
                        BesMessageBox::information(tr("不和你玩了"), tr("哼，再见！"));
                        exit(0);
                    }

                    QString tip = getTipForClickCount(nCount, diff);
                    if(QMessageBox::StandardButton::Ok ==
                      BesMessageBox::question(tr("温馨提示"), tip
                            +tr("\n\n您是否需要再考虑是否同意《BesLyric 音乐下载声明》?"),
                            tr("是的，我想再看一遍"),tr("不，我不想下载了")))
                    {
                        bReadAgain = true;
                    }

                }
                else
                {
                    BesMessageBox::information(tr("提示"),getTipAfterAgreeForTimes(nCount) +
                                               "\n\n"+tr("下载功能已为您开启，去选择一个歌曲下载目录吧 :)"));

                    agree = true;
                    break;
                }

                if(bReadAgain == false)
                    break;
            }
            else
                break;
            }
        }

        if(agree == false)
        {
            BesMessageBox::information(tr("提示"),tr("下载功能已为您关闭\n\n您仍可以使用本软件其他所有的功能！"));
        }

        checkboxMusicDownload->setChecked(agree);

        if(checkBackup != agree)
        {
            SettingManager::GetInstance().data().agreeDownloadDeclaration = agree;
            if(!SettingManager::GetInstance().saveSettingData())
            {
                SettingManager::GetInstance().data().agreeDownloadDeclaration = checkBackup;
                checkboxMusicDownload->setChecked(checkBackup);
                BesMessageBox::information(tr("提示"),tr("本次设置保存失败，可能是程序没有写权限"));
            }
            else
            {
                if(agree == false)
                {
                    if(!SettingManager::GetInstance().data().agreeDownloadDeclaration)
                        SettingManager::GetInstance().data().musicDowloadPath = "";

                    labelMusicDownloadPath->setText("");
                }
            }
        }


    });

    connect(groupNameFormat, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &SuMusicDownload::OnNameFormatClicked);

    return SettingUnitContainer;
}

QString SuMusicDownload::getTipAfterAgreeForTimes(int time)
{
    switch(time)
    {
    case 1:
        return tr("好样的！");
    case 2:
        return tr("我就知道第一次你不是故意点那么快的！");
    case 3:
        return tr("事不过三，刚好看了3次，我很欣赏你！");
    default:
        return tr("是那 %1 次的回眸，让我们在这里相遇！").arg(time);
    }
}

QString SuMusicDownload::getTipForClickCount(int count, qint64 currentTime)
{
    static qint64 acumulate = 0;
    if(count == 1)
        acumulate = 0;

    acumulate += currentTime;

    bool tooQuick = false;
    if(count%7==0) //7次判断一次是否点太快提示
    {
        qint64 averageTime = acumulate/7;
        acumulate = 0;

        if(averageTime < 1000)
            tooQuick = true;
    }

    QString normal = tr("不好意思，这里需要阅读并思考至少【30秒】的时间""\n\n您本次阅读的时间为：") + QString().number(currentTime*1.0/1000) + tr(" 秒");

    QString strReturn;
    if(count%5==0)//5次显示一次特殊提示
    {
       switch(count/5)
       {
       case 1:
           strReturn = tr("我想你该好好地看一次《BesLyric 音乐下载声明》");
           break;
       case 2:
           strReturn = tr("告诉你，不要乱来哦");
           break;
       case 3:
           strReturn = tr("程序即将奔溃");
           break;
       case 4:
           strReturn = tr("一直点下去是不会有结果的");
           break;
       case 5:
           strReturn = tr("温馨提示，你已经点了25下了");
           break;
       case 6:
           strReturn = tr("啊，我想吟诗一首");
           break;
       case 7:
           strReturn = tr("空山新雨后 自挂东南枝");
           break;
       case 8:
           strReturn = tr("欲穷千里目 自挂东南枝");
           break;
       case 9:
           strReturn = tr("爷娘闻女来 自挂东南枝");
           break;
       case 10:
           strReturn = tr("洞房花烛夜 自挂东南枝");
           break;
       case 11:
           strReturn = tr("风萧萧兮易水寒 壮士自挂东南枝");
           break;
       case 12:
           strReturn = tr("两只黄鹂鸣翠柳 白鹭自挂东南枝");
           break;
       case 13:
           strReturn = tr("人生在世不如意 不如自挂东南枝");
           break;
       case 14:
           strReturn = tr("天生我材必有用 各种自挂东南枝");
           break;
       case 15:
           strReturn = tr("呃...");
           break;
       case 16:
           strReturn = tr("温馨提示，你已经点了80下了");
           break;
       case 17:
           strReturn = tr("我看你能点到什么时候");
           break;
       case 18:
           strReturn = tr("温馨提示，你已经点了90下了");
           break;
       case 19:
           strReturn = tr("你再继续这么玩，我就不理你了");
           break;
       case 20:
           strReturn = tr("我真的不理你了");
           break;
       }

    }
    else
    {
        if(tooQuick && count <22) //前3次才提示点太快，后面就不提示点太快了
        {
            QString a[5] = {tr("别太激动，我又不会跑了，慢慢点~"),
                            tr("我心脏不好，你不要点太快~"),
                            tr("你再点这么快，我要报警了！"),
                            tr("真心疼你的鼠标，鼠标都被你摁疼了~"),
                            tr("你别以为我不知道你点这么快~")};

            strReturn = a[QDateTime::currentDateTime().toMSecsSinceEpoch()%5];
        }
        else
            strReturn = normal;
    }

    return strReturn;
}


void SuMusicDownload::OnNameFormatClicked(int radioId)
{
    int backup = SettingManager::GetInstance().data().nameFormatStyle;
    SettingManager::GetInstance().data().nameFormatStyle = (NameFormatStyle)radioId;

    if(!SettingManager::GetInstance().saveSettingData())
    {
        SettingManager::GetInstance().data().nameFormatStyle = backup;

        backup == NameFormatStyle::SONG_ARTIST?
            radioFormatSongArtist->setChecked(true):radioFormatArtistSong->setChecked(true);

        BesMessageBox::information(tr("提示"),tr("本次设置保存失败，可能是程序没有写权限"));
    }
}
