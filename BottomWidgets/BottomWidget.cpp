#include "global.h"
#include "BottomWidget.h"
#include <QHBoxLayout>
#include "MusicPlayer/musicPlayer.h"
#include "BesMessageBox.h"
#include "SettingManager.h"
#include "BesScaleUtil.h"

BottomWidget::BottomWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    initLayout();
    initEntity();
    initConnection();
}

BottomWidget::~BottomWidget()
{

}

void BottomWidget::initLayout()
{
    QWidget* bottomWidgetContainer = new QWidget(this);
    bottomWidgetContainer->setObjectName("bottomWidgetContainer");
    bottomWidgetContainer->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    btnPreSong = new BesButton(bottomWidgetContainer);
    btnPlayAndPause = new BesButton(bottomWidgetContainer);
    btnNextSong = new BesButton(bottomWidgetContainer);
    btnPreSong->setObjectName("btnPreSong");
    btnPlayAndPause->setObjectName("btnPlayAndPause");
    btnNextSong->setObjectName("btnNextSong");
    btnPreSong->setFocusPolicy(Qt::NoFocus);
    btnPlayAndPause->setFocusPolicy(Qt::NoFocus);
    btnNextSong->setFocusPolicy(Qt::NoFocus);

    btnPreSong->setVisible(false);
    btnNextSong->setVisible(false);

    labelTimeCurrent = new QLabel(bottomWidgetContainer);
    labelTimeEnding = new QLabel(bottomWidgetContainer);
    labelTimeCurrent->setText("00:00.000");
    labelTimeEnding->setText("00:00.000");
    labelTimeCurrent->setObjectName("labelTimeCurrent");
    labelTimeEnding->setObjectName("labelTimeEnding");

    labelTimeCurrent->setAlignment(Qt::AlignRight);
    labelTimeCurrent->setMinimumWidth(100 * BesScaleUtil::mscale());
    labelTimeCurrent->setMaximumWidth(100 * BesScaleUtil::mscale());
    labelTimeCurrent->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    sliderSong = new QSlider(bottomWidgetContainer);
    sliderSound= new QSlider(bottomWidgetContainer);
    sliderSong->setObjectName("sliderSong");
    sliderSound->setObjectName("sliderSound");
    sliderSong->setOrientation(Qt::Horizontal);
    sliderSound->setOrientation(Qt::Horizontal);
    sliderSound->setMaximumWidth(100);
    sliderSong->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
    sliderSound->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    btnSound = new BesButton(bottomWidgetContainer);
    btnPlayModeSingle = new BesButton(bottomWidgetContainer);
    btnPlayModeSingleCycle = new BesButton(bottomWidgetContainer);
    btnDesktopLyric = new BesButton(bottomWidgetContainer);
    btnSound->setObjectName("btnSound");
    btnPlayModeSingle->setObjectName("btnPlayModeSingle");
    btnPlayModeSingleCycle->setObjectName("btnPlayModeSingleCycle");
    btnDesktopLyric->setObjectName("btnDesktopLyric");
    btnSound->setFocusPolicy(Qt::NoFocus);
    btnPlayModeSingle->setFocusPolicy(Qt::NoFocus);
    btnPlayModeSingleCycle->setFocusPolicy(Qt::NoFocus);
    btnDesktopLyric->setFocusPolicy(Qt::NoFocus);
    btnDesktopLyric->setVisible(false);

    btnPlayModeSingle->setToolTip(tr("当前播放模式为【单曲播放】，点击切换为 【单曲循环】"));
    btnPlayModeSingleCycle->setToolTip(tr("当前播放模式为【单曲循环】，点击切换为 【单曲播放】"));

    btnSound->setCheckable(true);

    QHBoxLayout* hLayout = new QHBoxLayout(bottomWidgetContainer);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnPreSong);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnPlayAndPause);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnNextSong);
    hLayout->addSpacerItem(new QSpacerItem(25,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(labelTimeCurrent);
    hLayout->addWidget(sliderSong);
    hLayout->addWidget(labelTimeEnding);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnSound);
    hLayout->addWidget(sliderSound);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnPlayModeSingle);
    hLayout->addWidget(btnPlayModeSingleCycle);
    hLayout->addWidget(btnDesktopLyric);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));

    QVBoxLayout* vLayoutContainer = new QVBoxLayout(this);
    vLayoutContainer->setMargin(0);
    vLayoutContainer->addWidget(bottomWidgetContainer);

    setLayout(vLayoutContainer);
}

void BottomWidget::initEntity()
{
    sliderSong->setRange(0,1000);
    sliderSound->setRange(0,128);
    AdjustingPos = false;
    posAdjust = 0;

    musicPlayer = new MusicPlayer(this);
    musicPlayer->setNotifyInterval(33);

    bInMakingMode = false;
}

void BottomWidget::initConnection()
{
    connect(btnPlayAndPause, SIGNAL(clicked(bool)), this, SLOT(onPlayOrPause()));

    connect(sliderSong,SIGNAL(sliderPressed()),this, SLOT(onSliderSongPressed()));
    connect(sliderSong,SIGNAL(sliderMoved(int)),this, SLOT(onSliderSongMoved(int)));
    connect(sliderSong,SIGNAL(sliderReleased()),this, SLOT(onSliderSongReleased()));

    connect(btnSound,SIGNAL(toggled(bool)),this,SLOT(onSoundToggle(bool)));
    connect(sliderSound,SIGNAL(valueChanged(int)),musicPlayer,SLOT(setVolume(int)));
    connect(sliderSound,SIGNAL(valueChanged(int)),this,SLOT(onSliderSoundChanged(int)));
    connect(sliderSound,SIGNAL(sliderPressed()),this, SLOT(onSliderSoundPressed()));
    connect(sliderSound,SIGNAL(sliderReleased()),this, SLOT(onSliderSoundReleased()));

    connect(btnPlayModeSingle, SIGNAL(clicked(bool)),this, SLOT(onModeSingleNext()));
    connect(btnPlayModeSingleCycle, SIGNAL(clicked(bool)),this, SLOT(onModeSingleCycleNext()));

    connect(musicPlayer, SIGNAL(durationChanged(qint64)),this,SLOT(durationChanged(qint64)));
    connect(musicPlayer, SIGNAL(errorOccur(int,QString)),this,SLOT(onErrorOccurs(int,QString)));

    connect(musicPlayer, SIGNAL(audioFinish(bool)),this,SLOT(onAudioFinished(bool)));
    connect(musicPlayer, SIGNAL(audioPlay()),this,SLOT(onAudioPlay()));
    connect(musicPlayer, SIGNAL(audioPause()),this,SLOT(onAudioPause()));

    sliderSound->setValue(SettingManager::GetInstance().data().volume);
    nVolumeBeforeMute = sliderSound->value();
    bSliderSoundPress = false;

    setCurrentPlayMode(SettingManager::GetInstance().data().playMode);
}

//middleWidget->pageMain->subPageMaking,SIGNAL(onReloadMusic(QString)) emitted
//void MainWidget::OnPlayNewMusicAndLyric(QString music, QString lrc) invoked
void  BottomWidget::reloadMusic(QString musicPath)
{
    qDebug()<<"void  BottomWidget::reloadMusic(QString="<<musicPath<<")+musicPlayer->state()="<<musicPlayer->state();

    if(musicPlayer->state() != MusicPlayer::StoppedState )
        musicPlayer->stop();

    musicPlayer->setMusicPath(musicPath);
}

//void MainWidget::OnPlayNewMusicAndLyric(QString music, QString lrc) invoked
//void MainWidget::onLoadLrcLyricAndSwitchToPreview(QString lycLyricPath) invoked
//void MainWidget::onAudioFinished(bool isEndWithForce) invoked
void BottomWidget::play()
{
    qDebug()<<"void BottomWidget::play() musicPlayer->getMusicPath()="<<musicPlayer->getMusicPath();

    if(musicPlayer->getMusicPath().size() != 0)
    {
        musicPlayer->play();
    }
}

//uncalled
void BottomWidget::pause()
{
    musicPlayer->pause();
}

//middleWidget->pageMain->subPageMaking,SIGNAL(onStartMaking()) emitted
//从开头开始播放
void BottomWidget::playFromBegin()
{
    if(musicPlayer->getMusicPath().size() != 0)
    {
        musicPlayer->play();

        musicPlayer->seek(0);
//        当前，seek后一定会开始播放，这里可能需要改。
    }
}

//middleWidget->pageMain->subPageMaking,SIGNAL(onPlayOrPauseMusic()) emitted
//自动判断播放还是暂停
void BottomWidget::autoPlayOrPause()
{
    musicPlayer->playAndPause();
}

//middleWidget->pageMain->subPageMaking,SIGNAL(onStopMusic()) emitted
void BottomWidget::stop()
{
    qDebug()<<"void BottomWidget::stop()";

    if(musicPlayer->state() != MusicPlayer::StoppedState )
    {
        musicPlayer->stop();
    }
}

//uncalled
void BottomWidget::seek(quint64 pos)
{
     if(musicPlayer->state() != MusicPlayer::StoppedState )
         musicPlayer->seek(pos);
}

//uncalled
void BottomWidget::seekForward(quint64 step)
{
    if(musicPlayer->state() != MusicPlayer::StoppedState )
        musicPlayer->forwordSeek(step);
}

//middleWidget->pageMain->subPageMaking,SIGNAL(onSeekBackward(quint64)) emitted
void BottomWidget::seekBackward(quint64 step)
{
    if(musicPlayer->state() != MusicPlayer::StoppedState )
        musicPlayer->backwardSeek(step);
}

//middleWidget->pageMain->subPageMaking,SIGNAL(onEnterMakingMode()) emitted
//进入制作模式
void BottomWidget::enterMakingMode()
{
    bInMakingMode = true;
}

//middleWidget->pageMain->subPageMaking,SIGNAL(onExitMakingMode()) emitted
//退出制作模式
void BottomWidget::exitMakingMode()
{
    bInMakingMode = false;

}

//btnPlayAndPause->clicked(bool) emitted
void BottomWidget::onPlayOrPause()
{
    musicPlayer->playAndPause();
}

//musicPlayer, SIGNAL(durationChanged(qint64)) emitted
void BottomWidget::durationChanged(qint64 duration)
{
    int ms = duration % 1000;
    duration = duration/1000;
    int s = duration % 60;
    int m = duration/60;

    QString timeLabel;
    timeLabel.sprintf("%.2d:%.2d.%.3d",m, s, ms);

    labelTimeEnding->setText(timeLabel);

}

//void MainWidget::musicPositionChanged(int pos) invoked
void BottomWidget::positionChanged(int position)
{
    qDebug()<<"void BottomWidget::positionChanged(int position="<<position<<") sliderSong->value()="<<sliderSong->value();

    if(!AdjustingPos)
    {
        int pecentOfThousand = musicPlayer->duration() == 0? 0: int(1.0 * position / musicPlayer->duration() * 1000);
        sliderSong->setValue(pecentOfThousand);
    }

    int ms = position % 1000;
    position = position/1000;
    int s = position % 60;
    int m = position/60;

    QString timeLabel;
    timeLabel.sprintf("%.2d:%.2d.%.3d",m, s, ms);

    labelTimeCurrent->setText(timeLabel);

}

void BottomWidget::setCurrentPlayMode(int mode)
{
    //切换按钮状态
    if(mode == 0)
    {
        btnPlayModeSingle->setVisible(true);
        btnPlayModeSingleCycle->setVisible(false);
    }
    else // == 1
    {
        btnPlayModeSingle->setVisible(false);
        btnPlayModeSingleCycle->setVisible(true);
    }
}

void BottomWidget::onSliderSongMoved(int position)
{
    qDebug()<<"void BottomWidget::onSliderSongMoved(int position="<<position<<")";

    if(AdjustingPos){
        posAdjust = musicPlayer->duration() * position / 1000;
//        如果不需要拖动sliderSong时CurrentTimeLabel也改变，可以把posAdjust的计算放到onSliderSongReleased()中，position改为sliderSong->value()

//        positionChanged(posAdjust);
//        想要拖动sliderSong时CurrentTimeLabel也改变
//        暂时没有实现
    }
    else{
        sliderSong->setValue(sliderSongOriginalPos);//sliderSong回到原来的位置
//        制作时，按住或拖动sliderSong会使其不移动，这需要改
    }
}

void BottomWidget::onSliderSongPressed()
{
    qDebug()<<"void BottomWidget::onSliderSongPressed() sliderSong->value()="<<sliderSong->value()<<" musicPlayer->state()="<<musicPlayer->state();

    sliderSongOriginalPos = sliderSong->value();//记录当前值，准备与最终值比较

    if(bInMakingMode || musicPlayer->state() == MusicPlayer::StoppedState){
        return ; //制作模式或停止状态时不允许sliderSong被成功拖动，但并没有阻止信号被接收
    }

    AdjustingPos = true;
}

void BottomWidget::onSliderSongReleased()
{
    qDebug()<<"void BottomWidget::onSliderSongReleased() sliderSong->value()="<<sliderSong->value()<<" posAdjust="<<posAdjust;

    if(AdjustingPos){
        if(sliderSong->value() != sliderSongOriginalPos){
            //sliderSong.value精度不够，让一段时间均落在同一个value值中。
            //低精度并不影响定位，但如果最终并没有改变位置，那就可能由于onSliderSongMoved()计算的posAdjust精度不够而发生偏移；

            musicPlayer->seek(posAdjust);
        }
        AdjustingPos = false;
    }
}

void BottomWidget::onSoundToggle(bool mute)
{
    if(bSliderSoundPress)   //拖动声音时，不响应
        return;

    if(mute)
    {
        nVolumeBeforeMute = sliderSound->value();
        sliderSound->setValue(0);
    }
    else
    {
        musicPlayer->setVolume(SettingManager::GetInstance().data().volume);
        sliderSound->setValue(nVolumeBeforeMute);
    }

    SettingManager::GetInstance().data().isMute = mute;
    SettingManager::GetInstance().saveSettingData();
}

void BottomWidget::onSliderSoundChanged(int volume)
{
    btnSound->setChecked(volume == 0);

    //值发送改变立刻响应改变声音按钮，但是不立刻保存，在 onSliderSoundReleased 时再保存
}

void BottomWidget::onSliderSoundPressed()
{
    bSliderSoundPress = true;
}

void BottomWidget::onSliderSoundReleased()
{
    //每次结束音量的改变，需要即时保存
    SettingManager::GetInstance().data().volume = sliderSound->value();

    if(SettingManager::GetInstance().data().isMute != (sliderSound->value() == 0))
    {
        SettingManager::GetInstance().data().isMute = (sliderSound->value() == 0);
    }

    SettingManager::GetInstance().saveSettingData();  //出错不报提示 //TODOTODO 考虑是否和配置页面配置分开储存

    bSliderSoundPress = false;
}

void BottomWidget::onModeSingleNext()
{
    setCurrentPlayMode(1);

    SettingManager::GetInstance().data().playMode = 1;
    SettingManager::GetInstance().saveSettingData();
}

void BottomWidget::onModeSingleCycleNext()
{
    setCurrentPlayMode(0);
    SettingManager::GetInstance().data().playMode = 0;
    SettingManager::GetInstance().saveSettingData();
}



void BottomWidget::onErrorOccurs(int code, QString strErr)
{
    Q_UNUSED(code)
    BesMessageBox::information(tr("提示"),
        tr("播放音频时发生错误，请尝试使用别的音频文件")+ "\n\n" + tr("出错细节:")+ strErr);
}

void BottomWidget::onAudioFinished(bool isEndWithForce)
{
    qDebug()<<"void BottomWidget::onAudioFinished(bool isEndWithForce="<<isEndWithForce<<")";

    setStyleSheet("QPushButton#btnPlayAndPause{border-image:url(\":/resource/image/btn_play.png\");}");
}

void BottomWidget::onAudioPlay()
{
    qDebug()<<"void BottomWidget::onAudioPlay()";

    setStyleSheet("QPushButton#btnPlayAndPause{border-image:url(\":/resource/image/btn_pause.png\");}");
}

void BottomWidget::onAudioPause()
{
    qDebug()<<"void BottomWidget::onAudioPause()";

    setStyleSheet("QPushButton#btnPlayAndPause{border-image:url(\":/resource/image/btn_play.png\");}");
}


