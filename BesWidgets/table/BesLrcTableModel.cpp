#include "BesLrcTableModel.h"

BesLrcTableModel::BesLrcTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

BesLrcTableModel::~BesLrcTableModel()
{

}

void BesLrcTableModel::deleteAllItems()
{
    m_data.clear();
}

void BesLrcTableModel::appendItems(const QVector<LyricInfo>& infos)
{
    m_data.append(infos);
}

int BesLrcTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.size();
}

int BesLrcTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 5;
}

QVariant BesLrcTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        int nCol = index.column();
        int nRow =  index.row();
        LyricInfo info = m_data.at(nRow);
        switch(nCol)
        {
        case 0:
            return QString().sprintf("%02d",(nRow+1));
        case 1:
            return info.strSong;
        case 2:
            return info.strArtist;
        case 3:
            return info.strLyricFrom;
        case 4:
            return "";
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

Qt::ItemFlags BesLrcTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    // flag|=Qt::ItemIsEditable // 设置单元格可编辑,此处注释,单元格无法被编辑
    return flag;
}

void BesLrcTableModel::setHorizontalHeader(const QStringList &headers)
{
    m_HorizontalHeader =  headers;
}


QVariant BesLrcTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if(m_HorizontalHeader.size()!=0)
            return m_HorizontalHeader.at(section);
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void BesLrcTableModel::setData(const QVector<LyricInfo> &data)
{
    m_data = data;
}

