#include "StackFrame.h"
#include <QVBoxLayout>
#include "AppHelper.h"

StackFrame::StackFrame(QApplication *pApplication,QWidget *parent)
    : BesFramelessWidget(parent)
{
    pApp = pApplication;
    SetSkin("black");

    this->setMouseTracking(true);

    setBorderMain(8);
    initLayout();
    connectAll();
}

StackFrame::~StackFrame()
{

}

void StackFrame::SetSkin(QString skinName)
{
    AppHelper::SetStyle(pApp, skinName);
}

void StackFrame::initLayout()
{
    mainWidget = new MainWidget(this);
    skinBoxWidget = new SkinBoxWidget(this);

    mainWidget->raise();
    isMainOnTop = true;
}

void StackFrame::connectAll()
{
    connect(mainWidget->topWidget, SIGNAL(OnDoubleClick()),this, SLOT(toggleMaxRestoreStatus()));

    connect(mainWidget->topWidget->btnMax, SIGNAL(clicked(bool)), this, SLOT(toggleMaxRestoreStatus()));
    connect(mainWidget->topWidget->btnRestore, SIGNAL(clicked(bool)), this, SLOT(toggleMaxRestoreStatus()));

    connect(mainWidget->topWidget->btnSkinBox, SIGNAL(clicked(bool)), this, SLOT(toggleSkinBox()));

    connect(mainWidget->topWidget->btnMini, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));
    connect(mainWidget->topWidget->btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));

    connect(skinBoxWidget->btnBlack, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnRed, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
}

void StackFrame::setBorderMain(int border)
{
    borderMain = border;
    BesFramelessWidget::SetFrameBorder(borderMain);
}

void StackFrame::mouseReleaseEvent(QMouseEvent *event)
{
    BesFramelessWidget::mouseReleaseEvent(event);
}

void StackFrame::mouseMoveEvent(QMouseEvent *event)
{
    //所有的mouseMove 都希望传给 BesFramelessWidget处理
    BesFramelessWidget::mouseMoveEvent(event);
}

void StackFrame::mousePressEvent(QMouseEvent *event)
{
    //由于 BesFramelessWidget 要求获得所有鼠标信息，处理鼠标图案和拖动功能，但是
    //          这里只希望处理标题拖动 和 边缘resize 操作，所以判断是否在标题 或 边缘，决定是否处理press
    QPoint currentPoint = event->pos();
    if(mainWidget->topWidget->geometry().contains(currentPoint)||
            !mainWidget->geometry().contains(currentPoint))
        BesFramelessWidget::mousePressEvent(event);
    else
        QWidget::mousePressEvent(event);

}

void StackFrame::resizeEvent(QResizeEvent *event)
{
    BesFramelessWidget::resizeEvent(event);

    QRect mianWidgetRect = QRect(borderMain ,borderMain,
                                 this->width()- 2*borderMain, this->height()-2*borderMain);
    QRect skinBoxRect = QRect(this->width()-420,58,400,330);

    mainWidget->setGeometry(mianWidgetRect);
    skinBoxWidget->setGeometry(skinBoxRect);

}

//切换最大化和最小化
void  StackFrame::toggleMaxRestoreStatus()
{
    if(isMaximized())
    {
        showNormal();

        mainWidget->topWidget->btnMax->setVisible(true);
        mainWidget->topWidget->btnRestore->setVisible(false);
    }
    else
    {
        showMaximized();

        setGeometry(-borderMain, -borderMain, width()+2*borderMain, height()+ 2*borderMain);

        mainWidget->topWidget->btnMax->setVisible(false);
        mainWidget->topWidget->btnRestore->setVisible(true);
    }
}

//显示或隐藏皮肤盒
void StackFrame::toggleSkinBox()
{
    if(isMainOnTop)
    {
        skinBoxWidget->raise();
        isMainOnTop = false;
    }
    else
    {
        mainWidget->raise();
        isMainOnTop = true;
    }
}
