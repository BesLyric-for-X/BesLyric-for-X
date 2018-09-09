#include "global.h"
#include "BesLrcTableView.h"
#include "BesLrcTableModel.h"
#include "BesLrcButtonDelegate.h"
#include <QHeaderView>

BesLrcTableView::BesLrcTableView(QWidget *parent) :
    QTableView(parent)
{
    this->setMouseTracking(true);
    iniData();
}

BesLrcTableView::~BesLrcTableView()
{
    delete m_model;
}

void BesLrcTableView::deleteAllItems()
{
    m_model->deleteAllItems();
    emit m_model->layoutChanged();
    this->update();
}

void BesLrcTableView::appendItems(const QVector<LyricInfo>& infos)
{
    m_model->appendItems(infos);
    emit m_model->layoutChanged();
    this->update();
}

void BesLrcTableView::iniData()
{
    m_model = new BesLrcTableModel();
    this->setModel(m_model);

    QStringList headers;
    headers << "" << tr("音乐标题")<< tr("歌手")<< tr("来源")<< tr("操作");
    m_model->setHorizontalHeader(headers);
    m_buttonDelegate = new BesLrcButtonDelegate(this);
    this->setItemDelegateForColumn(4, m_buttonDelegate);

    connect(m_buttonDelegate, SIGNAL(sig_rowClicked(int)),this,SLOT(selectRow(int)));
    connect(m_buttonDelegate,&BesLrcButtonDelegate::sig_showLyric, [=](int row, bool rawLyric){
        if(rawLyric)emit(sig_showRawLyric(m_model->DataVector().at(row)));
        else emit (sig_showLrcLyric(m_model->DataVector().at(row)));
    });

    BaseInit();
}

 //基础的初始化
void BesLrcTableView::BaseInit()
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->setColumnWidth(0,50);
    this->setColumnWidth(4,240);
    this->verticalHeader()->setVisible(true);
    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setMinimumSectionSize(50);
    this->horizontalHeader()->setHighlightSections(false);
    this->setGridStyle(Qt::PenStyle::NoPen);
    this->setShowGrid(false);
}

//自动调整大小
void BesLrcTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    double widthLeft = this->width() - 50 - 240 - 15;
    this->setColumnWidth(1,widthLeft * 2/5);
    this->setColumnWidth(2,widthLeft * 2/5);
    this->setColumnWidth(3,widthLeft * 1/5);
}

