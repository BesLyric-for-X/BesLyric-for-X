#ifndef BES_LLIST_TABLE_MODEL_H
#define BES_LLIST_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "LyricListManager.h"

class BesLListTableView;
class BesLListTableModel : public QAbstractTableModel
{
    friend class BesLListTableView;
    Q_OBJECT
public:
    explicit BesLListTableModel(QObject *parent = 0);
    ~BesLListTableModel(void);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setHorizontalHeader(const QStringList& headers);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setData(LyricList* pData);
     LyricList* getData() {return m_pData;}

    void deleteAllItems();
    void setDataSource(LyricList* pData);

signals:
    void sig_finishDragging();

protected:
    //拖动支持的实现参考:https://blog.csdn.net/gongjianbo1992/article/details/106932706
    Qt::DropActions supportedDropActions() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

private:
    QStringList m_HorizontalHeader;
    LyricList* m_pData;
};

#endif // BES_LLIST_TABLE_MODEL_H
