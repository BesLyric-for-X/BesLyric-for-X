#include "SuScrollPanel.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QScrollBar>

SuScrollPanel::SuScrollPanel(QWidget *parent)
    : QScrollArea(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

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
        ISettingUnit* suDemo1 = new SuDemo1();
        ISettingUnit* suDemo2 = new SuDemo2();
        ISettingUnit* suDemo3 = new SuDemo1();
        ISettingUnit* suDemo4 = new SuDemo2();

        settings.push_back(suDemo1);
        settings.push_back(suDemo2);
        settings.push_back(suDemo3);
        settings.push_back(suDemo4);
    }

    return settings;
}

void SuScrollPanel::OnScrollToIndex(int index)
{

}


void SuScrollPanel::initLayout()
{
    settingUnitContainer = new QWidget(this);
    settingUnitContainer->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    settingUnitContainer->setObjectName("settingUnitContainer");
    settingUnitContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    this->setWidgetResizable(true);
    this->setWidget(settingUnitContainer); //
    this->setObjectName("scrollAreaSetting");
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFrameShape(QFrame::NoFrame);

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
}

void SuScrollPanel::initConnection()
{
    connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, [=](int min, int max){
        nScrollMin = min;
        nScrollMax = max;
    });

    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, [=](int value){
        emit sig_scrollPosChanged(value);
    });
}
