#include "MainWidget.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include "MusicPlayer/musicPlayer.h"
#include "BesScaleUtil.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent),topWidget(nullptr),middleWidget(nullptr),bottomWidget(nullptr)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    initEntity();
    initLayout();
    initConnection();
}

MainWidget::~MainWidget()
{

}

void MainWidget::initEntity()
{
}

void MainWidget::initLayout()
{
    topWidget = new TopWidget(this);
    middleWidget = new MiddleWidget(this);
    bottomWidget = new BottomWidget(this);
    topWidget->setObjectName("topWidget");
    middleWidget->setObjectName("middleWidget");
    bottomWidget->setObjectName("bottomWidget");

    topWidget->setMinimumHeight(65 * BesScaleUtil::scale());
    topWidget->setMaximumHeight(65 * BesScaleUtil::scale());
    topWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    bottomWidget->setMinimumHeight(65 * BesScaleUtil::scale());
    bottomWidget->setMaximumHeight(65 * BesScaleUtil::scale());
    bottomWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    vLayout->addWidget(topWidget);
    vLayout->addWidget(middleWidget);
    vLayout->addWidget(bottomWidget);
}

void MainWidget::initConnection()
{
    //页面切换响应
    connect(topWidget->btnIcon, &QPushButton::clicked, [=](){
        if(middleWidget->currentPage == 1)
            middleWidget->switchPage(1,0);
        if(middleWidget->currentPage == 2)
            middleWidget->switchPage(2,0);
        if(middleWidget->currentPage == 4)
            middleWidget->switchPage(4,0);

        //自动切换到3个子页面的制作歌词页面
        middleWidget->pageMain->btnMakingLyric->setChecked(true);
    });

    connect(topWidget->btnSetting, &QPushButton::clicked, [=](){
        if(middleWidget->currentPage == 1)
            middleWidget->switchPage(1,0);
        if(middleWidget->currentPage == 2)
            middleWidget->switchPage(2,0);

        if(middleWidget->currentPage != 4)
            middleWidget->switchPage(0,4);
    });

    connect(middleWidget->pageSetting->btnPackupSetting, &QPushButton::clicked, [=](){
            middleWidget->switchPage(4,0);});

    connect(middleWidget->pageLyricList->btnPackupLyricList,&QPushButton::clicked, [=](){
        middleWidget->switchPage(2,0);});

    //制作歌词页面信号响应
    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::onReloadMusic, this, &MainWidget::onUnloadLyricFromPreviewPage);

    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::onReloadMusic, middleWidget, &MiddleWidget::onReloadMusic);

    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::onReloadMusic, bottomWidget, &BottomWidget::reloadMusic);
    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::onStartMaking, bottomWidget, &BottomWidget::playFromBegin);
    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::onSeekBackward, bottomWidget, &BottomWidget::seekBackward);
    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::onPlayOrPauseMusic, bottomWidget, &BottomWidget::autoPlayOrPause);
    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::onStopMusic, bottomWidget, &BottomWidget::stop);
    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::loadLrcLyricAndSwitchToPreview, this, &MainWidget::onLoadLrcLyricAndSwitchToPreview);

    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::onEnterMakingMode, bottomWidget, &BottomWidget::enterMakingMode);
    connect(middleWidget->pageMain->subPageMaking, &SubPageMaking::onExitMakingMode, bottomWidget, &BottomWidget::exitMakingMode);

    //歌词单页面
    connect(middleWidget->pageLyricList->tableLrcList,&BesLListTableView::sig_playSongAndLyric,
                    [=](QString musicPath){
                            middleWidget->onReloadMusic(musicPath);
                            middleWidget->pageMain->subPageMaking->btnStartMaking->setEnabled(false);
                    }); //重新载入了音乐，就不能直接点击【开始制作】了
    connect(middleWidget->pageLyricList->tableLrcList, &BesLListTableView::sig_playSongAndLyric, this, &MainWidget::OnPlayNewMusicAndLyric);

    //底部播放器信号响应
    connect(bottomWidget->musicPlayer, &MusicPlayer::positionChanged, this, &MainWidget::musicPositionChanged);

    connect(bottomWidget->musicPlayer, &MusicPlayer::titleFound, middleWidget, &MiddleWidget::onSetMusicTitle);
    connect(bottomWidget->musicPlayer, &MusicPlayer::artistFound, middleWidget, &MiddleWidget::onSetMusicArtist);

    connect(bottomWidget->musicPlayer, &MusicPlayer::pictureFound, middleWidget->pageMain->boxPagePreviewLyric, &BoxPagePreviewLyric::changePic);
    connect(bottomWidget->musicPlayer, &MusicPlayer::pictureFound, middleWidget->pageLyricList->boxPagePreviewLyric, &BoxPagePreviewLyric::changePic);
    connect(bottomWidget->musicPlayer, &MusicPlayer::pictureFound, middleWidget->pagePreviewLyric, &PagePreviewLyric::AlbumImageChanged);
    connect(bottomWidget->musicPlayer, &MusicPlayer::audioPlay, middleWidget->pagePreviewLyric, &PagePreviewLyric::playPhonagraph);
    connect(bottomWidget->musicPlayer, &MusicPlayer::audioPause, middleWidget->pagePreviewLyric, &PagePreviewLyric::stopPhonagraph);
    connect(bottomWidget->musicPlayer, &MusicPlayer::audioFinish, middleWidget->pagePreviewLyric, &PagePreviewLyric::stopPhonagraph);

    connect(bottomWidget->musicPlayer, &MusicPlayer::audioFinish, this, &MainWidget::onAudioFinished);  //这里放在 finishMaking 前执行
    connect(bottomWidget->musicPlayer, &MusicPlayer::audioFinish, middleWidget->pageMain->subPageMaking, &SubPageMaking::finishMaking);

}


bool MainWidget::keyPress(QKeyEvent  *event)
{
    if( middleWidget->currentPage == 0 &&                    //在制作页面，且在制作歌词过程
            middleWidget->pageMain->subPageMaking->isMaking) //才响应以下按键
    {
        if(middleWidget->pageMain->subPageMaking->isEditing)
        {
            if(event->key() == Qt::Key_Return) //结束制作
            {
                middleWidget->pageMain->subPageMaking->toggleMiddleLineEdit(false);
                return true;
            }
        }
        else if(!middleWidget->pageMain->subPageMaking->isBatchEditing)
        {
            if(event->key() == Qt::Key_Up)  //推上一行
            {
                middleWidget->pageMain->subPageMaking->markOneLine();
            }
            else if(event->key() == Qt::Key_Right) //空出一行
            {
                middleWidget->pageMain->subPageMaking->markEmptyLine();
            }
            else if(event->key() == Qt::Key_B) //回退 5 秒
            {
                middleWidget->pageMain->subPageMaking->backBy5Second();
            }
            else if(event->key() == Qt::Key_Down) //回退一行
            {
                middleWidget->pageMain->subPageMaking->backOneLine();
            }
            else if(event->key() == Qt::Key_Space) //暂停
            {
                middleWidget->pageMain->subPageMaking->playOrPause();
            }
            else if(event->key() == Qt::Key_Return) //结束制作
            {
                middleWidget->pageMain->subPageMaking->finishMaking();
            }
            return true;
        }
    }

    return false;
}

void MainWidget::OnPlayNewMusicAndLyric(QString music, QString lrc)
{
    if(middleWidget->pageMain->subPageMaking->isMaking)
    {
        BesMessageBox::information(tr("提示"),tr("制作歌词过程中，不能播放其他音乐"));
        return;
    }

    //重载入音乐
    bottomWidget->reloadMusic(music);

    //载入歌词
    middleWidget->pagePreviewLyric->lyricViewer->setLrcLyricPath(lrc);

    //修改歌曲显示名称
    middleWidget->pagePreviewLyric->lyricViewer->setMusicPath(music);

    //播放歌曲
    bottomWidget->play();

//    //执行切换页面相关逻辑，如果当前正在歌词单页面，收起歌词单页面，展开预览页面
//    if(middleWidget->currentPage == 2)
//    {
//        middleWidget->switchPage(2,0);
//        middleWidget->switchPage(0,1);
//    }
}

//载入lrc歌词，并且播放当前歌曲来预览
void MainWidget::onLoadLrcLyricAndSwitchToPreview(QString lycLyricPath)
{
    //为了防止逻辑上的不明确，必须等到没有歌曲在播放的时候，才响应歌词的载入与预览
    if(bottomWidget->musicPlayer->state() != MusicPlayer::StoppedState)
    {
        //没有有效歌词时尝试载入歌词
        if(!middleWidget->pagePreviewLyric->lyricViewer->isLyricValid())
            middleWidget->pagePreviewLyric->lyricViewer->setLrcLyricPath(lycLyricPath);

        //切换页面(从主页面 0，到预览歌词页面1 切换 )
        middleWidget->switchPage(0,1);  //已经有歌词在播放，直接切换页面
        return ;
    }

    //载入歌词
    middleWidget->pagePreviewLyric->lyricViewer->setLrcLyricPath(lycLyricPath);

    //切换页面(从主页面 0，到预览歌词页面1 切换 )
    middleWidget->switchPage(0,1);

    //播放歌曲
    bottomWidget->play();
}


//从预览页面卸下歌词
void MainWidget::onUnloadLyricFromPreviewPage()
{
     middleWidget->pagePreviewLyric->lyricViewer->setEmptyLyric();
}

//音乐位置发生改变
void MainWidget::musicPositionChanged(int pos)
{
    //改变下方音乐条
    bottomWidget->positionChanged(pos);

    //改变制作歌词时的音乐时间
    if(middleWidget->pageMain->subPageMaking->isMaking
            && middleWidget->currentPage == 0)
        middleWidget->pageMain->subPageMaking->updatePos(pos);

    //改变预览歌词时的滚动状态
    if( middleWidget->pagePreviewLyric->lyricViewer->isLyricValid()
            && middleWidget->currentPage == 1)
        middleWidget->pagePreviewLyric->lyricViewer->scrollLyricPanel->lyricPanel->higthLineLyricAtPos(pos);
}

void MainWidget::onAudioFinished(bool isEndWithForce)
{
    if(middleWidget->pageMain->subPageMaking->isMaking)
        return; // 制作歌词模式，永远只播放一首歌

    if(isEndWithForce) //对于非自然结束的歌曲，不用考虑下一首
        return;

    int nPlayMode = SettingManager::GetInstance().data().playMode;
    if(nPlayMode == 0)  //单曲播放模式，也不继续播放
        return;

    if(nPlayMode == 1) //单曲循环模式，直接重新播放
    {
        if(!bottomWidget->musicPlayer->isCurrentInvalidMedia())
            //播放歌曲
            bottomWidget->play();
    }
}

