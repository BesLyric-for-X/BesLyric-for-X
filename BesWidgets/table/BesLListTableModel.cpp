#include "BesLListTableModel.h"
#include <QMimeData>

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
    //当选项被拖动到在列表项最后时，index.isValid() 为false，这里定义这种情况为“拖放到列表最后”，所以返回 ItemIsDropEnabled
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;
    else
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
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

Qt::DropActions BesLListTableModel::supportedDropActions() const
{
    return Qt::MoveAction;
}
QMimeData *BesLListTableModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *data=QAbstractItemModel::mimeData(indexes);
    if(data){
        //如果认为解析返回的 QMimeData 数据中的 MIME 类型为 "application/x-qabstractitemmodeldatalist" 的数据比较麻烦
        //可以考虑通过 QMimeData::setData(const QString &mimetype, const QByteArray &data) 设置数据，
        //而后通过 QMimeData::data(const QString &mimetype) 取回数据
    }
    return data;
}

bool BesLListTableModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if(!data || action != Qt::MoveAction)
        return false;

    //解析逻辑参考：https://stackoverflow.com/questions/1723989/how-to-decode-application-x-qabstractitemmodeldatalist-in-qt-for-drag-and-drop
    QByteArray encoded = data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encoded, QIODevice::ReadOnly);

    QSet<int> rowsFrom;
    while (!stream.atEnd())
    {
        int row, col;
        QMap<int,  QVariant> roleDataMap;
        stream >> row >> col >> roleDataMap;

        /* do something with the data */
        //qDebug()<<"data in QMimeData: Row->" << row << " Col->" << col << " Map<int,  QVariant>->" <<roleDataMap;
        rowsFrom.insert(row);
    }

    qDebug()<<"From row"<< rowsFrom;

    //参考文档：QAbstractItemModel::dropMimeData
    int beforeIndex = m_pData->items.size(); //默认拖动到最后
    if(row==-1 && column==-1)
    {
        int rowParent = parent.row();
        int colParent = parent.column();
        if(rowParent ==-1 && colParent ==-1)
            qDebug()<< "Dropped directly to the empty area below list items";
        else
        {
            qDebug()<< "Dropped directly to parent(QModelIndex)->(R:"<<parent.row() <<",C"<<parent.column() <<")";
            beforeIndex = parent.row();
        }
    }
    else
    {
        qDebug()<< "Dropped before: (Row:" << row << " Column:" << column <<")";
        beforeIndex = row;
    }

    //将 rowsFrom 中下标对应的项目，取出放置到 beforeIndex 前

    //标记需要取出的数据和目标数据
    QVector<LyricListItem> items = m_pData->items;
    for(int i = 0; i < items.size(); ++i)
    {
        items[i].selected = (rowsFrom.find(i)!=rowsFrom.end());
        items[i].target = i == beforeIndex;
    }

    //如果 target 和 selected 在同一个上面，那么需要修改 beforeIndex 为下一个没被 selected 的项上
    for(int i = 0; i < items.size(); ++i)
    {
        if(items[i].selected == true && items[i].target == true )
        {
            ++beforeIndex;
            items[i].target = false;
            if(i < items.size()-1)
                items[i+1].target = true;
        }
    }

    //取出
    QVector<LyricListItem> itemsFetched;
    for(auto iter = items.begin(); iter != items.end(); )
    {
        if(iter->selected)
        {
            itemsFetched.push_back(*iter);
            iter = items.erase(iter);
        }
        else
            ++iter;
    }

    //构建最终结果
    QVector<LyricListItem> itemsNew;
    if(beforeIndex != m_pData->items.size())
    {
        //不是落在最后，在目标下标对应的项前加入 itemsFetched
        for(LyricListItem& item : items)
        {
            if(item.target)
                itemsNew << itemsFetched;

            itemsNew << item;
        }
    }
    else
    {
        //直接拖动 itemsFetched 到最后
        itemsNew << items;
        itemsNew << itemsFetched;
    }

    //如果顺序发生了改变，则保存数据，并刷新布局
    bool bChanged = false;
    for(int i = 0; i< itemsNew.size(); ++i)
    {
        auto& item1 = itemsNew[i];
        auto& item2 = m_pData->items[i];
        if(item1.song != item2.song || item1.lyric != item2.lyric)
        {
            bChanged = true;
            break;
        }
    }

    if(bChanged)
    {
        m_pData->items = itemsNew;
        layoutChanged();
        emit sig_finishDragging();
    }

    return true;
}
