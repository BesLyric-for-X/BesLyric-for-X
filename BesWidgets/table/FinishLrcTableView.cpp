#include "global.h"
#include "FinishLrcTableView.h"
#include "FinishLrcTableModel.h"
#include "FinishLrcButtonDelegate.h"
#include <QHeaderView>

FinishLrcTableView::FinishLrcTableView(QWidget *parent) :
    QTableView(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    iniData();
}

FinishLrcTableView::~FinishLrcTableView()
{
    delete m_model;
}

void FinishLrcTableView::deleteAllItems()
{
    m_model->deleteAllItems();
    emit m_model->layoutChanged();
    this->update();
}

void FinishLrcTableView::appendItems(const QVector<LyricInfo>& infos)
{
    m_model->appendItems(infos);
    emit m_model->layoutChanged();
    this->update();
}


void FinishLrcTableView::setLyricData(QVector<QPair<quint64, QString>>* pLrcLines)
{
    m_model->setLyricLines(pLrcLines);
    emit m_model->layoutChanged();
    this->update();
}

QVector<QString> FinishLrcTableView::getSelectedLrc()
{
    QModelIndexList indexList = selectedIndexes();
    QVector<QString> vecLines;
    if(indexList.size() != 0)
    {
        QVector<QPair<quint64, QString>>* pLrc = m_model->getLyricLines();

        if(pLrc)
        {
            int indexSize = indexList.size();
            for(int i=0;i<indexSize; i+=3)
            {
                int index = indexList.at(i).row();
                QString line = pLrc->at(index).second.trimmed();
                if(!line.isEmpty())
                    vecLines.push_back(line);
            }
        }
    }
    return vecLines;
}

void FinishLrcTableView::iniData()
{
    m_model = new FinishLrcTableModel();
    this->setModel(m_model);

    QStringList headers;
    headers << "时间" << tr("歌词")<< tr("操作");
    m_model->setHorizontalHeader(headers);
    m_buttonDelegate = new FinishLrcButtonDelegate(this);
    this->setItemDelegateForColumn(2, m_buttonDelegate);

    connect(m_buttonDelegate, SIGNAL(sig_rowClicked(int)),this,SLOT(selectRow(int)));
    connect(m_buttonDelegate,&FinishLrcButtonDelegate::sig_showLyric, [=](int row, bool rawLyric){
        if(rawLyric)emit(sig_showRawLyric(m_model->DataVector().at(row)));
        else emit (sig_showLrcLyric(m_model->DataVector().at(row)));
    });

    BaseInit();
}

 //基础的初始化
void FinishLrcTableView::BaseInit()
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->setColumnWidth(0,120);
    this->setColumnWidth(2,120);
    this->verticalHeader()->setVisible(true);
    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setMinimumSectionSize(50);
    this->horizontalHeader()->setHighlightSections(false);
    this->setGridStyle(Qt::PenStyle::NoPen);
    this->setShowGrid(false);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//自动调整大小
void FinishLrcTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    double widthLeft = this->width() - 120 - 120 - 15;
    if(widthLeft<=0)
        return;

    this->setColumnWidth(1,widthLeft);
//    this->setColumnWidth(2,widthLeft * 2/5);
//    this->setColumnWidth(3,widthLeft * 1/5);
}

