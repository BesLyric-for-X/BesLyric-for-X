#include "FinishLrcButtonDelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOption>
#include <QDesktopWidget>
#include <QToolTip>

#define LRC_FINISH_OPERATE_COLUMN 2

FinishLrcButtonDelegate::FinishLrcButtonDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
  m_pBtnEditOneLyric(new BesButton()),
  m_nSpacing(5),
  m_nWidth(115),
  m_nHeight(28)
{
    //m_list << QStringLiteral("查看原歌词") << QStringLiteral("查看LRC歌词");
    m_list<< QStringLiteral("编辑");
}

void FinishLrcButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
        initStyleOption(&viewOption, index);
        if (option.state.testFlag(QStyle::State_HasFocus))
            viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

        QStyledItemDelegate::paint(painter, viewOption, index);
        bool isEmpty = index.data().toString().isEmpty();
        if (!isEmpty && index.column() == LRC_FINISH_OPERATE_COLUMN)
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
                }

                QWidget *pWidget = m_pBtnEditOneLyric.data();
                //QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
                pWidget->style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
            }
        }
}

bool FinishLrcButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    Q_UNUSED(model);

    bool isEmpty = index.data().toString().isEmpty();

    if (isEmpty || index.column() != LRC_FINISH_OPERATE_COLUMN)
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
                emit sig_rowClicked(index.row());
                m_nType = 1;
                break;
            }
            // 鼠标释放
            case QEvent::MouseButtonRelease:
            {
                emit sig_editLyric(index.row());
                break;
            }
            default:
                break;
            }
        }

        return bRepaint;
}
