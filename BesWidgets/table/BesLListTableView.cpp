#include "global.h"
#include "BesLListTableView.h"
#include "BesLListTableModel.h"
#include "BesLListButtonDelegate.h"
#include <QHeaderView>

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

void BesLListTableView::deleteAllItems()
{
    m_model->deleteAllItems();
    emit m_model->layoutChanged();
    this->update();
}

//void BesLListTableView::appendItems(const QVector<LyricListItem>& infos)
//{
//    m_model->appendItems(infos);
//    emit m_model->layoutChanged();
//    this->update();
//}

void BesLListTableView::setDataSource(LyricList *pData)
{
    m_model->deleteAllItems();
    m_model->setDataSource(pData);
//    m_model->appendItems(infos);
    emit m_model->layoutChanged();
    this->update();
}

void BesLListTableView::reloadTableFromData()
{
    this->update();
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
