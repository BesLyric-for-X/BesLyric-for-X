#include "global.h"
#include "BesLListTableView.h"
#include "BesLListTableModel.h"
#include "BesLListButtonDelegate.h"
#include <QHeaderView>
#include <QDrag>
#include <QPainterPath>

#include <QDesktopServices>
#include <QUrl>
#include "NetAccess.h"
#include "BesMessageBox.h"

BesLListTableView::BesLListTableView(QWidget *parent) :
    QTableView(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    initEntity();
    initConnection();
    BaseInit();
}

BesLListTableView::~BesLListTableView()
{
    delete m_model;
}

void BesLListTableView::setDataSource(LyricList *pData)
{
    m_model->deleteAllItems();
    m_model->setDataSource(pData);
    emit m_model->layoutChanged();

    //重置数据时，需要清空选中项，一是符合逻辑；二是在测试过程中发现，在某些情况会触发不必要的调用，甚至崩溃（由于下标不符合预期导致）
    //比如，当存在2个歌词单A和B，A 中有1个项，B 中有3个项目。同时选中B中的第1项和其他任意项，切换到A，A中的第1项显示为选中状态。
    //     此时，如果拖动 A 中第1项，会首先触发如下调用链：
    //...
    //BesLListTableModel::mimeData(const QModelIndexList &indexes)
    //...
    //QAbstractItemModel::mimeData(indexes)
    //...
    //BesLListTableModel::data(const QModelIndex &index, int role)
    //
    //由于多选状态下，此时 indexes（QModelIndexList） 中包含了多个项下标(包含了第1项和其他已经失效的项)，
    //BesLListTableModel::data 又没有做下标越界检测，即会导致越界崩溃，故在此需要在重置数据后清空选择
    this->clearSelection();
}

void BesLListTableView::reloadTableFromData()
{
    emit m_model->layoutChanged();
}

void  BesLListTableView::baseColorChanged(QColor color)
{
    if(m_listViewStyle)
        m_listViewStyle->setLineIndicatorColor(color);
}

void BesLListTableView::finishDragging()
{
    //拖动后，被拖动后的项需要保持选中状态
    LyricList* lyricList = m_model->getData();
    QList<int> rowsSelected;
    for(int i = 0; i < lyricList->items.size(); ++i)
    {
        LyricListItem& item = lyricList->items[i];
        if(item.selected)
            rowsSelected.append(i);
    }
    for(int i = 0; i< rowsSelected.size(); ++i)
    {
        QModelIndex index = m_model->createIndex(rowsSelected[i],0);
        QItemSelectionModel::SelectionFlags flags = (i==0?QItemSelectionModel::ClearAndSelect : QItemSelectionModel::Select) | QItemSelectionModel::Rows;

        this->selectionModel()->select(index,flags);
    }

    //拖动后立刻保持数据
    emit sig_saveLyricListData();
}

void BesLListTableView::initEntity()
{
    m_model = new BesLListTableModel();
    this->setModel(m_model);

    QStringList headers;
    headers << "" << tr("操作")<< tr("音乐文件名")<< tr("歌词文件名");
    m_model->setHorizontalHeader(headers);
    m_buttonDelegate = new BesLListButtonDelegate(this);
    this->setItemDelegateForColumn(1, m_buttonDelegate);
}

void BesLListTableView::initConnection()
{
    connect(m_buttonDelegate, &BesLListButtonDelegate::sig_rowClicked, this, &BesLListTableView::selectRow);
    connect(m_buttonDelegate,&BesLListButtonDelegate::sig_deleteItem, [=](int row){
        emit sig_deleteItem(row);
    });
    connect(m_buttonDelegate,&BesLListButtonDelegate::sig_editItem, [=](int row){
        emit sig_editItem(row);
    });

    connect(this,&QTableView::doubleClicked,[=](QModelIndex index){
        LyricListItem& listItem = m_model->getData()->items[index.row()];
        emit sig_playSongAndLyric(listItem.song, listItem.lyric);
    });

    connect(m_model, &BesLListTableModel::sig_finishDragging, this, &BesLListTableView::finishDragging);
}

 //基础的初始化
void BesLListTableView::BaseInit()
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->setColumnWidth(0,50);
    this->setColumnWidth(1,100);
    this->verticalHeader()->setVisible(true);
    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setMinimumSectionSize(50);
    this->horizontalHeader()->setHighlightSections(false);
    this->setGridStyle(Qt::PenStyle::NoPen);
    this->setShowGrid(false);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setDragDropMode(InternalMove);
    //为了能够识别拖动是落在目标item内的正中还是上下方，设置为不是覆盖而是插入
    //同时此时会出现 lineIndicator: https://blog.csdn.net/weixin_49601095/article/details/117956551
    this->setDragDropOverwriteMode(false);

    m_listViewStyle = new BesLListViewStyle(style());
    this->setStyle(m_listViewStyle); //为了显示自定义的拖动 line indicator
}

//自动调整大小
void BesLListTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    double widthLeft = this->width() - 50- 100 - 15;
    if(widthLeft<=0)
        return;

    this->setColumnWidth(2,widthLeft / 2);
    this->setColumnWidth(3,widthLeft / 2);
}

void BesLListTableView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();
    if (indexes.count() > 0) {
        QMimeData *data = m_model->mimeData(indexes);
        if (!data)
            return;

        //绘制拖动时显示的图像
        QPixmap pixmap;
        {
            //获得实际拖动的数据
            QVector<QString> items;
            {
                LyricList* lyricList = m_model->getData();

                QSet<int> rowsSet;
                for(QModelIndex index:indexes)
                    rowsSet.insert(index.row());

                QList<int> rows(rowsSet.begin(),rowsSet.end());
                std::sort(rows.begin(),rows.end());

                for(int row:rows)
                {
                    QString song = lyricList->items.at(row).song;
                    items.push_back( QFileInfo(song).fileName());
                }
            }

            //开始绘制
            int margin = 6;    //距边
            int interval = 4;  //行间隔
            int totalLine = items.size()>1 ? items.size()+1 : items.size(); //多于一行时，额外显示一个统计

            QFont font;
            QFontMetrics fmText = QFontMetrics(font);
            int heightOfLine = fmText.boundingRect("文本").height();   //一行高度

            int maxWidthOfText = 0;     //文字最大宽度
            for(QString fileName:items)
            {
                QRect rect = fmText.boundingRect(fileName);
                maxWidthOfText = qMax(maxWidthOfText,rect.width());
            }

            int HeightOfAllText = margin * 2 + totalLine * heightOfLine + (totalLine-1)*interval;
            int WidthOfAllText = margin * 2 + maxWidthOfText;

            pixmap = QPixmap(WidthOfAllText+1,HeightOfAllText+1);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            painter.setOpacity(0.6);
            painter.setFont(font);

            //绘制圆角矩形背景
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addRoundedRect(QRect(0, 0, WidthOfAllText, HeightOfAllText), margin, margin);
            painter.fillPath(path, Qt::white);
            painter.drawPath(path);

            //绘制文字
            QString extraHeader = items.size() >1?QString("共 %1 首歌").arg(items.size()):"";
            int currentY = margin + heightOfLine;
            if(!extraHeader.isEmpty())
            {
                painter.drawText(margin,currentY,extraHeader);
                currentY += heightOfLine + interval;
            }
            for(QString fileName:items)
            {
                painter.drawText(margin,currentY,fileName);
                currentY += heightOfLine + interval;
            }
        }

        QDrag *drag = new QDrag(this);
        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->exec(supportedActions);
    }
}
