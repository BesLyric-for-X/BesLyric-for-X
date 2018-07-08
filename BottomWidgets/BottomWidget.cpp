#include "BottomWidget.h"
#include <QHBoxLayout>

BottomWidget::BottomWidget(QWidget *parent)
    :QWidget(parent)
{
    this->setMouseTracking(true);
    initLayout();
    connectAll();
}

BottomWidget::~BottomWidget()
{

}

void BottomWidget::initLayout()
{
    QWidget* bottomWidgetContainer = new QWidget(this);
    bottomWidgetContainer->setObjectName("bottomWidgetContainer");
    bottomWidgetContainer->setMouseTracking(true);

    btnPreSong = new BesButton(bottomWidgetContainer);
    btnPlayAndPause = new BesButton(bottomWidgetContainer);
    btnNextSong = new BesButton(bottomWidgetContainer);
    btnPreSong->setObjectName("btnPreSong");
    btnPlayAndPause->setObjectName("btnPlayAndPause");
    btnNextSong->setObjectName("btnNextSong");

    labelTimeCurrent = new QLabel(bottomWidgetContainer);
    labelTimeEnding = new QLabel(bottomWidgetContainer);
    labelTimeCurrent->setText("00:00");
    labelTimeEnding->setText("00:00");
    labelTimeCurrent->setObjectName("labelTimeCurrent");
    labelTimeEnding->setObjectName("labelTimeEnding");

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
    btnPlayMode = new BesButton(bottomWidgetContainer);
    btnDesktopLyric = new BesButton(bottomWidgetContainer);
    btnSound->setObjectName("btnSound");
    btnPlayMode->setObjectName("btnPlayMode");
    btnDesktopLyric->setObjectName("btnDesktopLyric");

    QHBoxLayout* hLayout = new QHBoxLayout(bottomWidgetContainer);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnPreSong);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnPlayAndPause);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnNextSong);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(labelTimeCurrent);
    hLayout->addWidget(sliderSong);
    hLayout->addWidget(labelTimeEnding);
    hLayout->addWidget(btnSound);
    hLayout->addWidget(sliderSound);
    hLayout->addWidget(btnPlayMode);
    hLayout->addWidget(btnDesktopLyric);

    QVBoxLayout* vLayoutContainer = new QVBoxLayout(this);
    vLayoutContainer->setMargin(0);
    vLayoutContainer->addWidget(bottomWidgetContainer);

    setLayout(vLayoutContainer);
}

void BottomWidget::connectAll()
{

}
