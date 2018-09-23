#include "SettingNavigator.h"
#include <QPainter>
#include <QPixmap>
#include "SkinBoxWidget.h"

SuNavigator::SuNavigator(QVector<ISettingUnit*>& settingUnits, QWidget *parent)
    : QWidget(parent), pSettings(&settingUnits)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    initEntity();
    initLayout();
    initConnection();
}

SuNavigator::~SuNavigator()
{}

void SuNavigator::initEntity()
{
    nCurrentIndex = 0;

    nRight = 40;
    nTop = 40;
    nStep = 40;
    nr = 3;
    nR = 5;
    textHeight = 26;

    fontMetrics = nullptr;

    pureColorString = "#b82525"; //红色默认
}

void SuNavigator::initLayout()
{

}

void SuNavigator::initConnection()
{

}

void SuNavigator::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    //p.setPen(Qt::transparent);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(QPen(QColor(99,99,99,66),1));      //画笔颜色
    p.setBrush(QColor(99,99,99,66));            //画刷颜色

    if(fontMetrics == nullptr)
        fontMetrics = new QFontMetrics(p.font());       //获得当前使用的字体度量

    //先画右侧的滑动槽 （2个小圆1条直线）
    if(pSettings->size() >= 1)
    {
        QPoint pt1 = QPoint(this->width() - nRight, nTop);
        QPoint pt2 = QPoint(this->width() - nRight, nTop + (pSettings->size() -1) * nStep);

        p.drawLine(QPoint(pt1.x(), pt1.y() + nr),QPoint(pt2.x(), pt2.y() - nr-1));

        p.setPen(QPen(QColor(99,99,99,00),1));  //画填充圆的时候，边缘画笔透明
        p.drawEllipse(pt1, nr, nr);
        p.drawEllipse(pt2, nr, nr);
    }

    //再画当前节点 红点高亮

    QPoint pt = QPoint(this->width() - nRight, nTop + nCurrentIndex * nStep);
    p.setBrush(QColor(pureColorString));            //画刷颜色
    p.drawEllipse(pt, nR, nR);

    QColor colorText;

    //绘制文字
    for(int i =0; i < pSettings->size() ; i++)
    {
        if( i == nCurrentIndex)
            colorText = QColor(pureColorString);
        else
            colorText = QColor("#828385");

        p.setPen(colorText);
        p.drawText(vecRect.at(i), Qt::AlignRight, pSettings->at(i)->getName());
    }

    QWidget::paintEvent(event);
}

void SuNavigator::mouseMoveEvent(QMouseEvent *event)
{
    int index = getPointInRect(event->pos());

    if(index != -1)
        setCursor(QCursor(Qt::PointingHandCursor));
    else
        unsetCursor();
}

void SuNavigator::mousePressEvent(QMouseEvent *event)
{
    int index = getPointInRect(event->pos());
    if(index != -1)
        emit sig_scrollToSettingUnit(index);
}

void SuNavigator::resizeEvent(QResizeEvent *event)
{
    vecRect.clear();

    for(int i =0; i < pSettings->size() ; i++)
    {
        int nWidth = 200;
        if(fontMetrics != nullptr)
        {
            QRect boundRect = fontMetrics->boundingRect(pSettings->at(i)->getName());
            nWidth= boundRect.width() + 10;
        }
        QRect r(this->width() - nRight-28 - nWidth, nTop + i * nStep - textHeight/ 2
                ,nWidth,25);

        vecRect.push_back(r);
    }

    QWidget::resizeEvent(event);
}

void SuNavigator::setFinalSkinName(QString skinName)
{
    if(skinName.contains('#'))
    {
        pureColorString = skinName;
        return;
    }

    if(skinName == "black" || skinName == "red")
        pureColorString = "#b82525"; //红色
    else if(skinName == "pink")
        pureColorString = themColorPink;
    else if(skinName == "blue")
        pureColorString = themColorBlue;
    else if(skinName == "green")
        pureColorString = themColorGreen;
    else if(skinName == "gold")
        pureColorString = themColorGold;
    else
        pureColorString = "#b82525"; //红色默认
}

void SuNavigator::OnSettingUnitPanelPosChanged(int pos, int pageStep, int nScrollMax)
{
    int dividedStep = pageStep / pSettings->size();

    nCurrentIndex = 0;

    int nCumulate = 0;
    for(int i = 0; i< pSettings->size(); i++)
    {
        nCumulate += pSettings->at(i)->getUnitHeight();
        if(pos + dividedStep * (i+1) < nCumulate)
        {
            nCurrentIndex = i;
            break;
        }
    }

    //本软件特殊处理
    if(nCurrentIndex == pSettings->size() -1)   //在本软件中，滚动到最后时特殊处理，高亮倒数第2个（软件说明）
        nCurrentIndex = pSettings->size() -2;
    if(pos == nScrollMax-1 )//只有在 nScrollMax-1 位置才高亮最后一个
        nCurrentIndex = pSettings->size() -1;   //这个位置一般是点击最后一个触发 ScrollPanel的 OnScrollToIndex(int index)

    update();
}

int SuNavigator::getPointInRect(QPoint& pt)
{
    for(int i = 0; i< vecRect.size(); i++)
    {
        if(vecRect.at(i).contains(pt))
            return i;
    }
    return -1;
}
