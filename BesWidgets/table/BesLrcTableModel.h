#ifndef BESLRCTABLEMODEL_H
#define BESLRCTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "Define/Define.h"

class BesLrcTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BesLrcTableModel(QObject *parent = 0);
    ~BesLrcTableModel(void);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setHorizontalHeader(const QStringList& headers);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setData(const QVector<LyricInfo>& data);
    QVector<LyricInfo>& DataVector() {return m_data;}

    void deleteAllItems();
    void appendItems(const QVector<LyricInfo>& infos);

signals:

public slots:


private:
    QStringList m_HorizontalHeader;
    QVector<LyricInfo> m_data;
};

#endif // BESLRCTABLEMODEL_H
