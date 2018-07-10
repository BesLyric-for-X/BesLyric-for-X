#include "BesShadowWidget.h"
#include <QPainter>
#include <QtMath>

BesShadowWidget::BesShadowWidget(QWidget *parent)
    : QWidget(parent)
{
    this->border = 8;
    marginTop = 0;
    bgColor = Qt::white;
}

void BesShadowWidget::SetShadowBorder(int boder)
{
    this->border = boder;
}

void BesShadowWidget::SetMarginTop(int margin)
{
    marginTop = margin;
}

void BesShadowWidget::SetBackgroundColor(QColor bgColor)
{
    this->bgColor = bgColor;
}

void BesShadowWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
 // draw shadow margin

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(border, border + marginTop,
                 this->width()- 2*border, this->height()- 2*border - marginTop);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(bgColor));

    QColor color(22, 22, 22, 60);
    for(int i=0; i<border; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(border-i, border-i + marginTop,
                     this->width()-(border-i)*2, this->height() - marginTop-(border-i)*2);
        color.setAlpha(60 - qSqrt(i)*20);
        painter.setPen(color);
        painter.drawPath(path);
    }

    QWidget::paintEvent(e);
}
