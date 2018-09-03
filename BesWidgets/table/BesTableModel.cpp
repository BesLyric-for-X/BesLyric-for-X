#include "BesTableModel.h"

BesTableModel::BesTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

BesTableModel::~BesTableModel()
{

}


int BesTableModel::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

int BesTableModel::columnCount(const QModelIndex &parent) const
{
    return m_HorizontalHeader.count();
}

QVariant BesTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole) {
        int ncol = index.column();
        int nrow =  index.row();
        QStringList values = m_data.at(nrow);
        if (values.size() > ncol)
            return values.at(ncol);
        else
        return QVariant();
    }
    else if(role==Qt::TextAlignmentRole)
    {
        if(index.column() == 0)
            return Qt::AlignRight;
        else
            return Qt::AlignLeft;
    }

    return QVariant();
}

Qt::ItemFlags BesTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    // flag|=Qt::ItemIsEditable // 设置单元格可编辑,此处注释,单元格无法被编辑
    return flag;
}

void BesTableModel::setHorizontalHeader(const QStringList &headers)
{
    m_HorizontalHeader =  headers;
}


QVariant BesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_HorizontalHeader.at(section);
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void BesTableModel::setData(const QVector<QStringList> &data)
{
    m_data = data;
}
