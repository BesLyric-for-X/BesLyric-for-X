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

void BesTableView::appendItems(const QVector<LyricInfo>& infos)
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

//自动调整大小
void BesTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    double widthLeft = this->width() - 50 - 200 - 15;
    this->setColumnWidth(1,widthLeft * 2/5);
    this->setColumnWidth(2,widthLeft * 2/5);
    this->setColumnWidth(3,widthLeft * 1/5);
}

