#include "FinishLrcTableModel.h"

FinishLrcTableModel::FinishLrcTableModel(QObject *parent) :
    QAbstractTableModel(parent),pLrcLines(nullptr)
{
}

FinishLrcTableModel::~FinishLrcTableModel()
{

}

void FinishLrcTableModel::deleteAllItems()
{
    m_data.clear();

    pLrcLines = nullptr;
}

void FinishLrcTableModel::appendItems(const QVector<LyricInfo>& infos)
{
    m_data.append(infos);
}

int FinishLrcTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if(pLrcLines)
        return pLrcLines->size();
    else
        return 0;
}

int FinishLrcTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}

QVariant FinishLrcTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        int nCol = index.column();
        int nRow =  index.row();
        switch(nCol)
        {
        case 0:
        {
            int nTime = pLrcLines->at(nRow).first;
            int nMiliSecond = nTime%1000;
            nTime /= 1000;
            int nSecond = nTime%60;
            nTime /= 60;
            int nMinute = nTime;

            return QString().sprintf("%02d:%02d.%03d",nMinute, nSecond, nMiliSecond);
        }
        case 1:
            return pLrcLines->at(nRow).second;
        case 2:
            return pLrcLines->at(nRow).second.isEmpty()? "": " ";
            //存储是否为空的信息,为空输出空串，否则输出空格，为了不显示出来
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

Qt::ItemFlags FinishLrcTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    // flag|=Qt::ItemIsEditable // 设置单元格可编辑,此处注释,单元格无法被编辑
    return flag;
}

void FinishLrcTableModel::setHorizontalHeader(const QStringList &headers)
{
    m_HorizontalHeader =  headers;
}


QVariant FinishLrcTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if(m_HorizontalHeader.size()!=0)
            return m_HorizontalHeader.at(section);
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void FinishLrcTableModel::setData(const QVector<LyricInfo> &data)
{
    m_data = data;
}

