#include "BesNcmSongTableModel.h"

BesNcmSongTableModel::BesNcmSongTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

BesNcmSongTableModel::~BesNcmSongTableModel()
{
}

void BesNcmSongTableModel::deleteAllItems()
{
    m_data.clear();
}

void BesNcmSongTableModel::appendItems(const QVector<SONGINFO>& infos)
{
    m_data.append(infos);
}

int BesNcmSongTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.size();
}

int BesNcmSongTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 6;
}

QVariant BesNcmSongTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        int nCol = index.column();
        int nRow =  index.row();
        SONGINFO info = m_data.at(nRow);

        int nMinutes = info.nTime/1000/60;
        int nSecond = info.nTime/1000%60;

        switch(nCol)
        {
        case 0:
            return QString().sprintf("%02d",(nRow+1));
        case 1:
            return info.nPercentage;
        case 2:
            return info.strSong;
        case 3:
            return info.strArtists;
        case 4:
            return info.strAlbum;
        case 5:
            return QString().sprintf("%02d:%02d",nMinutes, nSecond);
        default:
            return QVariant();
        }

    }
    else if(role==Qt::TextAlignmentRole)
    {
        if(index.column() == 0)
            return Qt::AlignCenter;
        else
            return Qt::AlignVCenter;
    }

    return QVariant();
}

Qt::ItemFlags BesNcmSongTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    // flag|=Qt::ItemIsEditable // 设置单元格可编辑,此处注释,单元格无法被编辑
    return flag;
}

void BesNcmSongTableModel::setHorizontalHeader(const QStringList &headers)
{
    m_HorizontalHeader =  headers;
}


QVariant BesNcmSongTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if(m_HorizontalHeader.size()!=0)
            return m_HorizontalHeader.at(section);
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void BesNcmSongTableModel::setData(const QVector<SONGINFO> &data)
{
    m_data = data;
}

