#ifndef BES_LLIST_VIEW_STYLE_H
#define BES_LLIST_VIEW_STYLE_H
#include <QProxyStyle>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

//为了显示自定义的拖动 line indicator,重新实现拖动效果
//实现参考：https://stackoverflow.com/questions/7596584/qtreeview-draw-drop-indicator
//实现原理参考：
/*
    实际绘制代码在Qt源代码中被调用逻辑如下：
    inline void QAbstractItemViewPrivate::paintDropIndicator(QPainter *painter)
    {
        if (showDropIndicator && state == QAbstractItemView::DraggingState
#ifndef QT_NO_CURSOR
            && viewport->cursor().shape() != Qt::ForbiddenCursor
#endif
            ) {
            QStyleOption opt;
            opt.init(q_func());
            opt.rect = dropIndicatorRect;
            q_func()->style()->drawPrimitive(QStyle::PE_IndicatorItemViewItemDrop, &opt, painter, q_func());
        }
    }
 */
class BesLListViewStyle: public QProxyStyle{
public:
    BesLListViewStyle(QStyle* style = 0):QProxyStyle(style){};

    void drawPrimitive( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0 ) const override
    {
        if (element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull())
        {
            QRect rectToDraw = option->rect;
            rectToDraw.setLeft(0);
            if (widget)rectToDraw.setRight(widget->width());
            rectToDraw.setHeight(3);

            //qDebug()<<"rectToDraw:" << rectToDraw;

            painter->save();
            painter->setPen(Qt::NoPen);
            painter->setBrush(m_indicatorColor);
            painter->drawRect(rectToDraw);
            painter->restore();

            return;
        }
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }

    void setLineIndicatorColor(QColor indicatorColor)
    {
        m_indicatorColor = indicatorColor;
    }
private:
    QColor m_indicatorColor = QColor(255,0,0);
};

#endif
