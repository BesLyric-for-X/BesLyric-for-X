#include "global.h"
#include "BesTableView.h"
#include "BesTableModel.h"
#include "BesButtonDelegate.h"
#include <QHeaderView>

BesTableView::BesTableView(QWidget *parent) :
    QTableView(parent)
{
    this->setMouseTracking(true);
    iniData();
}

BesTableView::~BesTableView()
{
    delete m_model;
}

void BesTableView::deleteAllItems()
{
    m_model->deleteAllItems();
    emit m_model->layoutChanged();
    this->update();
}

void BesTableView::appendItems(QVector<LyricInfo> infos)
{
    m_model->appendItems(infos);
    emit m_model->layoutChanged();
    this->update();
}

void BesTableView::iniData()
{
    m_model = new BesTableModel();
    this->setModel(m_model);

    QStringList headers;
    headers << "" << tr("音乐标题")<< tr("歌手")<< tr("来源")<< tr("操作");
    m_model->setHorizontalHeader(headers);
    m_buttonDelegate = new BesButtonDelegate(this);
    this->setItemDelegateForColumn(4, m_buttonDelegate);

    connect(m_buttonDelegate, SIGNAL(rowClicked(int)),this,SLOT(selectRow(int)));

    BaseInit();
}

 //基础的初始化
void BesTableView::BaseInit()
{
    this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->setColumnWidth(0,50);
    this->setColumnWidth(4,200);
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setMinimumSectionSize(50);
    this->horizontalHeader()->setHighlightSections(false);
    this->setGridStyle(Qt::PenStyle::NoPen);

}

//void BesTableView::resizeEvent(QResizeEvent *event)
//{
//    QTableView::resizeEvent(event);
////    double widthLeft = this->width() - 50 - 80 - 5;
////    QModelIndex modelIndex;
////    int nColumn = m_model->columnCount(modelIndex);
////    double sigleWidth = widthLeft / (nColumn -2);
////    this->setColumnWidth(2,sigleWidth);
////    this->setColumnWidth(3,sigleWidth);
////    this->setColumnWidth(4,sigleWidth);
////    this->setColumnWidth(5,sigleWidth);
//}

