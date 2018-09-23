#include "BesLListButtonDelegate.h"

#include "Define/Struct.h"
#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QToolTip>
#include <QProxyStyle>

#define NCM_LIST_OPERATE_COLUMN 1

BesLListButtonDelegate::BesLListButtonDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
  m_pBtnDeleteListItem(new BesButton()),
  m_pBtnEditListItem(new BesButton()),
  m_nSpacing(5),
  m_nWidth(25),
  m_nHeight(25)
{
    // 设置按钮正常、划过、按下样式

    m_pBtnDeleteListItem->setObjectName("m_pBtnDeleteListItem");
    m_pBtnEditListItem->setObjectName("m_pBtnEditListItem");

    m_list << QStringLiteral("删除项")<< QStringLiteral("编辑项");
}

void BesLListButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == NCM_LIST_OPERATE_COLUMN)
    {
        // 计算按钮显示区域
        int nCount = m_list.count();
        int nleft = (option.rect.width() - m_nWidth * nCount - m_nSpacing * (nCount - 1)) / 2;
        int nTop = (option.rect.height() - m_nHeight) / 2;

        for (int i = 0; i < nCount; ++i)
        {
            // 绘制按钮
            QStyleOptionButton button;
            button.rect = QRect(option.rect.left() + nleft + m_nWidth * i + m_nSpacing * i,
                                option.rect.top() + nTop,  m_nWidth, m_nHeight);
            button.state |= QStyle::State_Enabled;

            if (button.rect.contains(m_mousePoint))
            {
                if (m_nType == 0)
                {
                    button.state |= QStyle::State_MouseOver;
                    //button.icon = QIcon(QString(":/Images/%1Hover").arg(m_list.at(i)));
                }
                else if (m_nType == 1)
                {
                    button.state |= QStyle::State_Sunken;
                    //button.icon = QIcon(QString(":/Images/%1Pressed").arg(m_list.at(i)));
                }
            }

            QWidget *pWidget;

            bool bShowDownloadButton = true;

            if(i==2)
            {
                QVariant data = (index.model()->data(index,Qt::DisplayRole));
                int downloadPercent =  data.toInt();
                bool bShowDownLoadStatus = downloadPercent != -1; //-1为未下载,-2 表示网络文件无效

                if(downloadPercent > 100)
                    downloadPercent = 100;

                if(bShowDownLoadStatus)
                {
                    painter->setRenderHint(QPainter::Antialiasing, true);

                    double penWidth = 3;
                    QRect rect = button.rect;
                    rect.setLeft(rect.left()+penWidth+2);
                    rect.setTop(rect.top()+penWidth+2);
                    rect.setRight(rect.right()-penWidth-2);
                    rect.setBottom(rect.bottom()-penWidth-2);

                    if(downloadPercent == 100)
                        painter->drawPixmap(button.rect,QPixmap(":/resource/image/download_done.png"));
                    else if(downloadPercent == -2)
                        painter->drawPixmap(button.rect,QPixmap(":/resource/image/download_done_invalid.png"));
                    else
                    {
                        double angle = 360.0 * downloadPercent / 100;

                        QPen myPen = QPen(QColor("#3782ac"));
                        myPen.setWidth(penWidth);
                        painter->setPen(myPen);
                        painter->drawArc(rect, 0, angle*16);
                    }
                    bShowDownloadButton = false;
                }
            }

            if( i ==0 || i ==1 || bShowDownloadButton)
            {
                pWidget = (i == 0) ? m_pBtnDeleteListItem.data() :m_pBtnEditListItem.data() ;

                pWidget->style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
            }

        }
    }
}

bool BesLListButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(model);
    if (index.column() != NCM_LIST_OPERATE_COLUMN)
            return false;

        m_nType = -1;
        bool bRepaint = false;
        QMouseEvent *pEvent = static_cast<QMouseEvent *> (event);
        m_mousePoint = pEvent->pos();

        int nCount = m_list.count();
        int nHalf = (option.rect.width() - m_nWidth * nCount - m_nSpacing * (nCount - 1)) / 2;
        int nTop = (option.rect.height() - m_nHeight) / 2;

        // 还原鼠标样式
        //QApplication::restoreOverrideCursor();

        for (int i = 0; i < nCount; ++i)
        {
            QStyleOptionButton button;
            button.rect = QRect(option.rect.left() + nHalf + m_nWidth * i + m_nSpacing * i,
                                option.rect.top() + nTop,  m_nWidth, m_nHeight);

            // 鼠标位于按钮之上
            if (!button.rect.contains(m_mousePoint))
                continue;

            bRepaint = true;
            switch (event->type())
            {
            // 鼠标滑过
            case QEvent::MouseMove:
            {
                // 设置鼠标样式为手型
                //QApplication::setOverrideCursor(Qt::PointingHandCursor);

                m_nType = 0;

                //if(i == 2 && )

                QToolTip::showText(pEvent->globalPos(), m_list.at(i));

                break;
            }
            // 鼠标按下
            case QEvent::MouseButtonPress:
            {
                emit sig_rowClicked(index.row());
                m_nType = 1;
                break;
            }
            // 鼠标释放
            case QEvent::MouseButtonRelease:
            {
                if (i == 0)
                    emit sig_deleteItem(index.row());
                else if(i== 1)
                    emit sig_editItem(index.row());
                break;
            }
            default:
                break;
            }
        }

        return bRepaint;
}
