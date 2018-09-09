#ifndef BES_NCM_SONG_TABLE_MODEL_H
#define BES_NCM_SONG_TABLE_MODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include "Define/Define.h"

class BesNcmSongTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BesNcmSongTableModel(QObject *parent = 0);
    ~BesNcmSongTableModel(void);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setHorizontalHeader(const QStringList& headers);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setData(const QVector<SONGINFO>& data);
    QVector<SONGINFO>& DataVector() {return m_data;}

    void deleteAllItems();
    void appendItems(const QVector<SONGINFO>& infos);

signals:

public slots:


private:
    QStringList m_HorizontalHeader;
    QVector<SONGINFO> m_data;
};

#endif // BES_NCM_SONG_TABLE_MODEL_H
