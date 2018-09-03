#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>

class BesTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BesTableModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setHorizontalHeader(const QStringList& headers);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setData(const QVector<QStringList>& data);
    QVector<QStringList>& DataVector() {return m_data;}
    ~BesTableModel(void);

signals:

public slots:


private:
    QStringList m_HorizontalHeader;
    QVector<QStringList> m_data;
};

#endif // TABLEMODEL_H
