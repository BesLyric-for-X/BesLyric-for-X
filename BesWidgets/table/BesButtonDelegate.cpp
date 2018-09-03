#include "BesButtonDelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QToolTip>

#define FILE_OPERATE_COLUMN 4

BesButtonDelegate::BesButtonDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
  m_pBtnLyricRaw(new BesButton()),
  m_pBtnLyricLrc(new BesButton()),
  m_nSpacing(5),
  m_nWidth(85),
  m_nHeight(28)
{
    m_list << QStringLiteral("查看原歌词") << QStringLiteral("查看LRC歌词");
}


void BesButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
        initStyleOption(&viewOption, index);
        if (option.state.testFlag(QStyle::State_HasFocus))
            viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

        QStyledItemDelegate::paint(painter, viewOption, index);

        if (index.column() == FILE_OPERATE_COLUMN)
        {
            // 计算按钮显示区域
            int nCount = m_list.count();
            int nHalf = (option.rect.width() - m_nWidth * nCount - m_nSpacing * (nCount - 1)) / 2;
            int nTop = (option.rect.height() - m_nHeight) / 2;

            for (int i = 0; i < nCount; ++i)
            {
                // 绘制按钮
                QStyleOptionButton button;
                button.rect = QRect(option.rect.left() + nHalf + m_nWidth * i + m_nSpacing * i,
                                    option.rect.top() + nTop,  m_nWidth, m_nHeight);
                button.text = m_list.at(i);
                button.state |= QStyle::State_Enabled;

                //button.iconSize = QSize(16, 16);
                //button.icon = QIcon(QString(":/Images/%1").arg(m_list.at(i)));

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

                QWidget *pWidget = (i == 0) ? m_pBtnLyricRaw.data() : m_pBtnLyricLrc.data();
                //QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
                pWidget->style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
            }
        }
}

bool BesButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (index.column() != FILE_OPERATE_COLUMN)
            return false;

        m_nType = -1;
        bool bRepaint = false;
        QMouseEvent *pEvent = static_cast<QMouseEvent *> (event);
        m_mousePoint = pEvent->pos();

        int nCount = m_list.count();
        int nHalf = (option.rect.width() - m_nWidth * nCount - m_nSpacing * (nCount - 1)) / 2;
        int nTop = (option.rect.height() - m_nHeight) / 2;

        // 还原鼠标样式
        QApplication::restoreOverrideCursor();

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
                QApplication::setOverrideCursor(Qt::PointingHandCursor);

                m_nType = 0;
                QToolTip::showText(pEvent->globalPos(), m_list.at(i));

                break;
            }
            // 鼠标按下
            case QEvent::MouseButtonPress:
            {
                m_nType = 1;
                break;
            }
            // 鼠标释放
            case QEvent::MouseButtonRelease:
            {
                if (i == 0)
                {
                    emit showMsg(QString("i=0 ;row:%1 line:%2").arg(index.row()).arg(index.column()));
                }
                else
                {
                    emit showMsg(QString("i=1 ;row:%1 line:%2").arg(index.row()).arg(index.column()));
                }
                break;
            }
            default:
                break;
            }
        }

        return bRepaint;
}

void BesButtonDelegate::showMsg(QString str)
{
    QMessageBox msg;
    msg.setText(str);
    msg.exec();
}
