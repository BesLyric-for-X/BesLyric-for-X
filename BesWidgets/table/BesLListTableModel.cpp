#include "BesLListTableModel.h"

BesLListTableModel::BesLListTableModel(QObject *parent) :
    QAbstractTableModel(parent),m_pData(nullptr)
{
}

BesLListTableModel::~BesLListTableModel()
{
}

void BesLListTableModel::deleteAllItems()
{
    m_pData = nullptr;
}

void BesLListTableModel::setDataSource(LyricList *pData)
{
    m_pData = pData;
}

int BesLListTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if(m_pData == nullptr)
        return 0;

    return m_pData->items.size();
}

int BesLListTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant BesLListTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

//    if(m_pData == nullptr)
//        return QVariant();

    if (role == Qt::DisplayRole)
    {
        int nCol = index.column();
        int nRow =  index.row();
        LyricListItem item = m_pData->items.at(nRow);

        switch(nCol)
        {
        case 0:
            return QString().sprintf("%02d",(nRow+1));
        case 1:
            return "";
        case 2:
            return item.song.right(item.song.length() - item.song.lastIndexOf('/')-1);
        case 3:
            return item.lyric.right(item.lyric.length() - item.lyric.lastIndexOf('/')-1);
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
    else if(role == Qt::ToolTipRole)
    {
        LyricListItem item = m_pData->items.at(index.row());

        if(index.column()==2)
        {
            return item.song;
        }
        else if( index.column()==3)
        {
            return item.lyric;
        }
    }

    return QVariant();
}

Qt::ItemFlags BesLListTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    // flag|=Qt::ItemIsEditable // 设置单元格可编辑,此处注释,单元格无法被编辑
    return flag;
}

void BesLListTableModel::setHorizontalHeader(const QStringList &headers)
{
    m_HorizontalHeader =  headers;
}


QVariant BesLListTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if(m_HorizontalHeader.size()!=0)
            return m_HorizontalHeader.at(section);
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void BesLListTableModel::setData(LyricList* pData)
{
    m_pData = pData;
}

