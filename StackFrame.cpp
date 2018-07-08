#include "StackFrame.h"
#include <QVBoxLayout>

StackFrame::StackFrame(QWidget *parent)
    : BesFramelessWidget(parent)
{
    this->setMouseTracking(true);

    setBorderMain(8);
    initLayout();
    connectAll();
}

StackFrame::~StackFrame()
{

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
    connect(mainWidget->topWidget->btnSkinBox, SIGNAL(clicked(bool)), this, SLOT(toggleSkinBox()));
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
    QRect skinBoxRect = QRect(this->width()-500,80,400,320);

    mainWidget->setGeometry(mianWidgetRect);
    skinBoxWidget->setGeometry(skinBoxRect);


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
