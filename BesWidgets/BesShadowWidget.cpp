#include "BesShadowWidget.h"
#include <QPainter>
#include <QtMath>

BesShadowWidget::BesShadowWidget(QWidget *parent)
    : QWidget(parent)
{
    this->border = 8;
}

void BesShadowWidget::SetShadowBorder(int boder)
{
    this->border = boder;
}

void BesShadowWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
 // draw shadow margin

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(border, border, this->width()- 2*border, this->height()- 2*border);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(0, 0, 0, 0);
    for(int i=0; i<border; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRect(border-i, border-i, this->width()-(border-i)*2, this->height()-(border-i)*2);
        color.setAlpha(60 - qSqrt(i)*20);
        painter.setPen(color);
        painter.drawPath(path);
    }

    QWidget::paintEvent(e);
}
