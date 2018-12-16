#ifndef FinishLrcTableModel_H
#define FinishLrcTableModel_H

#include <QAbstractTableModel>
#include <QVector>
#include "Define/Define.h"

class FinishLrcTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit FinishLrcTableModel(QObject *parent = 0);
    ~FinishLrcTableModel(void);
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

    void setLyricLines(QVector<QPair<quint64, QString>>* _pLrcLines){pLrcLines = _pLrcLines;}
    QVector<QPair<quint64, QString>>* getLyricLines(){return pLrcLines;}
signals:

public slots:


private:
    QStringList m_HorizontalHeader;
    QVector<LyricInfo> m_data;

    QVector<QPair<quint64, QString>>* pLrcLines;
};

#endif // FinishLrcTableModel_H
