#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QStyledItemDelegate>
#include <QPushButton>
#include <BesButton.h>

class BesButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BesButtonDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void sig_rowClicked(int row);
    void sig_showLyric(int row, bool rawLyric);

public slots:


private:
    QPoint m_mousePoint;  // 鼠标位置
    QScopedPointer<BesButton> m_pBtnLyricRaw;
    QScopedPointer<BesButton> m_pBtnLyricLrc;
    QStringList m_list;
    int m_nSpacing;  // 按钮之间的间距
     int m_nWidth;  // 按钮宽度
     int m_nHeight;  // 按钮高度
     int m_nType;  // 按钮状态-1：划过 2：按下

};

#endif // BUTTONDELEGATE_H
