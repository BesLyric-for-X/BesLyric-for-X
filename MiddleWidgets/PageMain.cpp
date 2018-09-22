#include "PageMain.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "global.h"

PageMain::PageMain(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    initEntity();
    initLayout();
    initConnection();
}

PageMain::~PageMain()
{

}

void PageMain::initEntity()
{
    qRegisterMetaType<LyricSearchResult>("LyricSearchResult");
}

void PageMain::initLayout()
{
    leftBoardMainPage = new QWidget(this);
    leftBoardMainPage->setObjectName("leftBoardMainPage");
    leftBoardMainPage->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    btnMakingLyric = new BesButton( QIcon(":/resource/image/maker_black.png"),
                                    tr("  制作歌词"),leftBoardMainPage);
    btnDownloadSong = new BesButton(QIcon(":/resource/image/download_music_black.png"),
                                    tr("  下载歌曲"),leftBoardMainPage);
    btnDownloadLyric = new BesButton(QIcon(":/resource/image/download_lyric_black.png"),
                                    tr("  下载歌词"),leftBoardMainPage);

    btnMakingLyric->setCheckable(true);     btnMakingLyric->setChecked(true);
    btnDownloadSong->setCheckable(true);
    btnDownloadLyric->setCheckable(true);

    btnMakingLyric->setAutoExclusive(true);
    btnDownloadSong->setAutoExclusive(true);
    btnDownloadLyric->setAutoExclusive(true);

    btnMakingLyric->setObjectName("btnMakingLyric");
    btnDownloadSong->setObjectName("btnDownloadSong");
    btnDownloadLyric->setObjectName("btnDownloadLyric");

    btnMakingLyric->setFocusPolicy(Qt::NoFocus);
    btnDownloadSong->setFocusPolicy(Qt::NoFocus);
    btnDownloadLyric->setFocusPolicy(Qt::NoFocus);

    boxPageLyricList = new BoxPageLyricList(leftBoardMainPage);
    boxPagePreviewLyric = new BoxPagePreviewLyric(leftBoardMainPage);
    boxPageLyricList->setObjectName("boxPageLyricList");
    boxPagePreviewLyric->setObjectName("boxPagePreviewLyric");

    boxPageLyricList->setMinimumSize(100,55+10);
    boxPageLyricList->setMaximumSize(300,55+10);
    boxPageLyricList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    boxPagePreviewLyric->setMinimumSize(100,55+10);
    boxPagePreviewLyric->setMaximumSize(300,55+10);
    boxPagePreviewLyric->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    subPageContainer = new QWidget(this);
    subPageContainer->setObjectName("subPageContainer");
    subPageContainer->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    subPageMaking = new SubPageMaking(subPageContainer);
    subPageDownloadSong = new SubPageDownloadSong(subPageContainer);
    subPageDownloadLyric = new SubPageDownloadLyric(subPageContainer);

    QHBoxLayout* hLayoutMain = new QHBoxLayout(this);
    hLayoutMain->setMargin(0);
    hLayoutMain->setSpacing(0);

    QVBoxLayout* vLayoutButtons = new QVBoxLayout(leftBoardMainPage);                //左侧按钮垂直布局
    vLayoutButtons->setMargin(0);
    vLayoutButtons->setSpacing(0);
    vLayoutButtons->addWidget(btnMakingLyric);
    vLayoutButtons->addWidget(btnDownloadSong);
    vLayoutButtons->addWidget(btnDownloadLyric);
    vLayoutButtons->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

    vLayoutButtons->addWidget(boxPageLyricList);
    vLayoutButtons->addWidget(boxPagePreviewLyric);


    subpageStackedLayout = new QStackedLayout(subPageContainer);         //右侧页面层叠布局
    subpageStackedLayout->setStackingMode(QStackedLayout::StackingMode::StackOne);
    subpageStackedLayout->addWidget(subPageMaking);
    subpageStackedLayout->addWidget(subPageDownloadSong);
    subpageStackedLayout->addWidget(subPageDownloadLyric);
    subpageStackedLayout->setCurrentIndex(0);

    hLayoutMain->addWidget(leftBoardMainPage);
    hLayoutMain->addWidget(subPageContainer);
}

void PageMain::initConnection()
{
    connect(btnMakingLyric,&QPushButton::toggled, [=](bool checked)
    {if(checked)subpageStackedLayout->setCurrentIndex(0);});
    connect(btnDownloadSong,&QPushButton::toggled, [=](bool checked)
    {if(checked)subpageStackedLayout->setCurrentIndex(1);});
    connect(btnDownloadLyric,&QPushButton::toggled, [=](bool checked)
    {if(checked)subpageStackedLayout->setCurrentIndex(2);});

    connect(subPageDownloadLyric,SIGNAL(sig_autoSelectRawLyric(const QString&)),this,
            SLOT(onAutoSelectRawLyric(const QString&)));

    connect(subPageDownloadSong->tableNcmSongSearch,SIGNAL(sig_setMusicPathToMakingPage(QString)),
            this,SLOT(onAutoSelectMusic(QString)));


    connect(&subPageMaking->threadGuessLyricInfo,SIGNAL(sig_loadLyricInfoGuessResult(QString,QString)),
            this, SLOT(onLoadLyricGuess(QString,QString)));
    connect(&subPageMaking->threadGuessLyricInfo,SIGNAL(sig_loadNcmInfoGuessResult(QString,QString)),
            this, SLOT(onLoadNcmGuess(QString,QString)));

}

void PageMain::onAutoSelectRawLyric(const QString& RawlyricPath)
{
    //自动切换页面
	btnMakingLyric->setChecked(true);

    //自动填入新的歌词路径
    subPageMaking->selectLyricPath(RawlyricPath);
}

void PageMain::onAutoSelectMusic(const QString &musicPath)
{
    //自动切换页面
	btnMakingLyric->setChecked(true);

    //自动填入新的歌词路径
    subPageMaking->selectMusicPath(musicPath);
}

void PageMain::onLoadLyricGuess(QString strSong, QString strArtist)
{
    //自动切换页面
    btnDownloadLyric->setChecked(true);

    subPageDownloadLyric->searchLyricDirectly(strArtist, strSong);

    //重新启用按钮
    subPageMaking->btnGuessLyricInfo->setEnabled(true);
}

void PageMain::onLoadNcmGuess(QString strSong, QString strArtist)
{
    //自动切换页面
    btnDownloadSong->setChecked(true);

    subPageDownloadSong->searchNcmDirectly(strArtist, strSong);

    //重新启用按钮
    subPageMaking->btnDownloadMp3->setEnabled(true);
}

void PageMain::setFinalSkinName(QString skinName)
{
    if(skinName == "black")
    {
        btnMakingLyric->setIcon(QIcon(":/resource/image/maker_black.png"));
        btnDownloadSong->setIcon(QIcon(":/resource/image/download_music_black.png"));
        btnDownloadLyric->setIcon(QIcon(":/resource/image/download_lyric_black.png"));
    }
    else
    {
        btnMakingLyric->setIcon(QIcon(":/resource/image/maker_white.png"));
        btnDownloadSong->setIcon(QIcon(":/resource/image/download_music_white.png"));
        btnDownloadLyric->setIcon(QIcon(":/resource/image/download_lyric_white.png"));
    }

}

