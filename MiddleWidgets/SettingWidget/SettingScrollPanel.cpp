#include "SettingScrollPanel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QScrollBar>

SuScrollPanel::SuScrollPanel(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    initEntity();
    initLayout();
    initConnection();
}

SuScrollPanel::~SuScrollPanel()
{

}

QVector<ISettingUnit *> &SuScrollPanel::getSettingUnits()
{
    if(settings.size() == 0)
    {
        ISettingUnit* suLyricMaker = new SuLyricMaker();
        ISettingUnit* suDefaultPath = new SuDefaultPath();
        ISettingUnit* suMusicDownload = new SuMusicDownload();
        ISettingUnit* suLogin = new SuLogin();
        ISettingUnit* suUpgrade = new SuUpgrade();
        ISettingUnit* suSoftware = new SuSoftware();
        ISettingUnit* suDonation = new SuDonation();

        settings.push_back(suLyricMaker);
        settings.push_back(suDefaultPath);
        settings.push_back(suMusicDownload);
        settings.push_back(suLogin);
        settings.push_back(suUpgrade);
        settings.push_back(suSoftware);
        settings.push_back(suDonation);

        //连接需要发出信号的控件
        connect((SuDefaultPath*)suDefaultPath, &SuDefaultPath::sig_defaultPathLyricChanged, [=](QString path){
            emit sig_defaultPathLyricChanged(path);
        });
        connect((SuDefaultPath*)suDefaultPath, &SuDefaultPath::sig_defaultPathOutputChanged, [=](QString path){
            emit sig_defaultPathOutputChanged(path);
        });
    }

    return settings;
}

void SuScrollPanel::OnScrollToIndex(int index)
{
    int devidedStep = nPageStep / settings.size();

    int pos = 0;
    for(int i = 0; i< settings.size(); i++ )
    {
        if(i < index)
        {
            pos += settings.at(i)->getUnitHeight();
        }
        else
            break;
    }

    nTargetPos = pos - devidedStep * index;

    //本软件特殊处理
    if(index == settings.size() -1)            //最后一个特殊处理，滑到最下
       nTargetPos = nScrollMax - 1;            // -1 最后的横线不显示

    if(index == settings.size() -2)            //在本软件中，最后第二个特殊处理，为了直接显示更多文字说明
       nTargetPos = pos;


    if(!scrollTimer->isActive())
        scrollTimer->start();
}

void SuScrollPanel::OnSrcollTimerTimeout()
{
    int minStep = 40;
    int maxStep = 200;

    int current = scrollAreaSetting->verticalScrollBar()->value();

    if(current == nTargetPos)
        scrollTimer->stop();
    else
    {
        if(current < nTargetPos)
        {
            int step = (nTargetPos - current)*3/4;
            if(step > maxStep)
                step = maxStep;
            if(step < minStep)
                step = minStep;

            current+=step;
            if(current > nTargetPos)
                current = nTargetPos;
        }
        else
        {
            int step = (current - nTargetPos)*3/4;
            if(step > maxStep)
                step = maxStep;
            if(step < minStep)
                step = minStep;

            current-=step;
            if(current < nTargetPos)
                current = nTargetPos;
        }

        scrollAreaSetting->verticalScrollBar()->setValue(current);
    }
}

void SuScrollPanel::initEntity()
{
    scrollTimer = new QTimer(this);
    scrollTimer->setInterval(1);
}

void SuScrollPanel::initLayout()
{
    settingUnitContainer = new QWidget(this);
    settingUnitContainer->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    settingUnitContainer->setObjectName("settingUnitContainer");
    settingUnitContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    scrollAreaSetting = new QScrollArea(this);
    scrollAreaSetting->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    scrollAreaSetting->setWidgetResizable(true);
    scrollAreaSetting->setWidget(settingUnitContainer);
    scrollAreaSetting->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    scrollAreaSetting->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreaSetting->setFrameShape(QFrame::NoFrame);
    scrollAreaSetting->setObjectName("scrollAreaSetting");

    //初始化所有的设置单元控件
    getSettingUnits();

    //下面使用生产的设置单元来初始化控件
    int nSpacing = 0;
    QVBoxLayout* vSettingUnits = new QVBoxLayout(settingUnitContainer);
    vSettingUnits->setMargin(0);
    vSettingUnits->setSpacing(nSpacing);

    int nTotalHeight = (settings.size() <= 1) ? 0 : ((settings.size()-1)* nSpacing);
    for(int i = 0; i< settings.size(); i++)
    {
        int  nHeight= settings[i]->getUnitHeight();
        nTotalHeight += nHeight;

        QWidget* suWidget = settings[i]->getUnitWidget(settingUnitContainer);
        suWidget->setMouseTracking(true);   //详见 BesFramelessWidget.h 注释
        suWidget->setMinimumHeight(nHeight);
        suWidget->setMaximumHeight(nHeight);
        suWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        vSettingUnits->addWidget(suWidget);
    }

    settingUnitContainer->setMinimumHeight(nTotalHeight);
    settingUnitContainer->setMaximumHeight(nTotalHeight);

    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(scrollAreaSetting);
}

void SuScrollPanel::initConnection()
{
    connect(scrollAreaSetting->verticalScrollBar(), &QScrollBar::rangeChanged, [=](int min, int max){
        nPageStep = scrollAreaSetting->verticalScrollBar()->pageStep();
        nScrollMin = min;
        nScrollMax = max;
    });

    connect(scrollAreaSetting->verticalScrollBar(), &QScrollBar::valueChanged, [=](int value){
        nPageStep = scrollAreaSetting->verticalScrollBar()->pageStep();
        emit sig_scrollPosChanged(value, nPageStep, nScrollMax);
    });

    connect(scrollTimer, &QTimer::timeout, this, &SuScrollPanel::OnSrcollTimerTimeout);
}
