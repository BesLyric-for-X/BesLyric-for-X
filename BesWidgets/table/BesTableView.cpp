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


void BesTableView::iniData()
{

    m_model = new BesTableModel();
    this->setModel(m_model);


    QStringList headers;
    headers << "" << tr("音乐标题")<< tr("歌手")<< tr("来源")<< tr("操作");
    m_model->setHorizontalHeader(headers);

    QVector<QStringList> data;
    data.append(QStringList() << "01" << "You Dont't Know Me"<< "Ofenbach/Brodie Barcaly"<< "You Dont't Know Me"<< "");
    data.append(QStringList() << "02" << "Top Of The World"<< "Carpenters"<< "Yesteray"<< "");
    data.append(QStringList() << "03" << "You Dont't Know Me"<< "Ofenbach/Brodie Barcaly"<< "You Dont't Know Me"<< "");
    data.append(QStringList() << "04" << "Top Of The World"<< "Carpenters"<< "Yesteray"<< "");
    data.append(QStringList() << "05" << "You Dont't Know Me"<< "Ofenbach/Brodie Barcaly"<< "You Dont't Know Me"<< "");
    m_model->setData(data);

    m_buttonDelegate = new BesButtonDelegate(this);
    this->setItemDelegateForColumn(4, m_buttonDelegate);
    emit m_model->layoutChanged();

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

