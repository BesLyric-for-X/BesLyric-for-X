#include "SuNavigator.h"
#include <QPainter>
#include <QPixmap>

SuNavigator::SuNavigator(QVector<ISettingUnit*>& settingUnits, QWidget *parent)
    : QWidget(parent), pSettings(&settingUnits)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    initEntity();
    initLayout();
    initConnection();
}

SuNavigator::~SuNavigator()
{

}

void SuNavigator::initEntity()
{

}

void SuNavigator::initLayout()
{

}

void SuNavigator::initConnection()
{

}

void SuNavigator::paintEvent(QPaintEvent *event)
{
    int marginTop = 10;

    QPainter p(this);
    p.setPen(Qt::transparent);

    QPixmap indicatorPixmap;
    QColor LineColor;
    QColor BackgroundColor;


    indicatorPixmap = QPixmap(":/resource/image/box_indicator_black.png");
    BackgroundColor = QColor("#2d2f33");
    LineColor=QColor("#3a3c40");

    p.setBrush(BackgroundColor);//刷背景
    QRect mainLayerRect = QRect(8 ,8 + marginTop, this->width()- 16, this->height()-16 - marginTop);
    p.drawRoundedRect(mainLayerRect,5,5);

    //绘制直线 和 指标
    p.setPen(QPen(LineColor,2));
//    p.drawLine(25,btnTheme->geometry().bottom()+ marginTop+border,
//               this->width()-25, btnTheme->geometry().bottom()+ marginTop+border);
    p.drawPixmap(this->width()/2 - indicatorPixmap.width()/2, 0 ,indicatorPixmap);  //在中上位置绘制指标

    QWidget::paintEvent(event);
}

void SuNavigator::OnSettingUnitPanelPosChanged(int pos)
{

}
