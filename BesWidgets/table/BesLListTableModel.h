#ifndef BES_LLIST_TABLE_MODEL_H
#define BES_LLIST_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "LyricListManager.h"

class BesLListTableModel : public QAbstractTableModel
{
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

public slots:


private:
    QStringList m_HorizontalHeader;
    LyricList* m_pData;
};

#endif // BES_LLIST_TABLE_MODEL_H
