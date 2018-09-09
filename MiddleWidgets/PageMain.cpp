#include "PageMain.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "global.h"

PageMain::PageMain(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
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
    leftBoardMainPage->setMouseTracking(true);

    btnMakingLyric = new BesButton(leftBoardMainPage);
    btnDownloadSong = new BesButton(leftBoardMainPage);
    btnDownloadLyric = new BesButton(leftBoardMainPage);

    btnMakingLyric->setCheckable(true);     btnMakingLyric->setChecked(true);
    btnDownloadSong->setCheckable(true);
    btnDownloadLyric->setCheckable(true);

    btnMakingLyric->setAutoExclusive(true);
    btnDownloadSong->setAutoExclusive(true);
    btnDownloadLyric->setAutoExclusive(true);

    btnMakingLyric->setObjectName("btnMakingLyric");
    btnDownloadSong->setObjectName("btnDownloadSong");
    btnDownloadLyric->setObjectName("btnDownloadLyric");

    btnMakingLyric->setText(tr("制作歌词"));
    btnDownloadSong->setText(tr("下载歌曲"));
    btnDownloadLyric->setText(tr("下载歌词"));

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
    subPageContainer->setMouseTracking(true);

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
            SLOT(OnAutoSelectRawLyric(const QString&)));
}

void PageMain::OnAutoSelectRawLyric(const QString& RawlyricPath)
{
    //自动切换页面
    subpageStackedLayout->setCurrentIndex(0);

    //自动填入新的歌词路径
    subPageMaking->selectLyricPath(RawlyricPath);
}

