#include <Define/Define.h>
#include "PageLyricList.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QDebug>

PageLyricList::PageLyricList(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    initLayout();
    initConnection();
}

PageLyricList::~PageLyricList()
{

}

void PageLyricList::initLayout()
{
    QGridLayout* mainLayout = new QGridLayout(this);
    pageLyricListContainer = new QWidget(this);
    pageLyricListContainer->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    pageLyricListContainer->setObjectName("pageLyricListContainer");
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(pageLyricListContainer);

    lyriclistLeftPanel = new QWidget(pageLyricListContainer);
    lyriclistRightPanel = new QWidget(pageLyricListContainer);
    lyriclistLeftPanel->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    lyriclistRightPanel->setMouseTracking(true);
    lyriclistLeftPanel->setObjectName("lyriclistLeftPanel");
    lyriclistRightPanel->setObjectName("lyriclistRightPanel");
    lyriclistLeftPanel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    lyriclistRightPanel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);


    //左侧列表

    QVBoxLayout* vListLayout = new QVBoxLayout(lyriclistLeftPanel);

    //制作历史歌词单
    lyricListHistory = new BesList(lyriclistLeftPanel);
//  lyricListHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉滚动条
//  lyricListHistory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lyricListHistory->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    lyricListHistory->setFocusPolicy(Qt::NoFocus);
    lyricListHistory->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    lyricListHistory->addItem("制作历史");


    //创建的歌单

    //表头
    headerListCreated = new BesListHeader("创建的歌词单",true,true,lyriclistLeftPanel);
    headerListCreated->setMaximumHeight(36);
    headerListCreated->setMinimumHeight(36);
    headerListCreated->setMinimumWidth(50);
    headerListCreated->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    //列表
    lyricListCreated = new BesList(lyriclistLeftPanel);
//  lyricListCreated->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉滚动条
//  lyricListCreated->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lyricListCreated->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
    lyricListCreated->setFocusPolicy(Qt::NoFocus);
    lyricListCreated->setDragDropMode(QListView::DragDropMode::InternalMove);
    lyricListCreated->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    //创建列表测试
    lyricListCreated->addItem("我喜欢的音乐");
    lyricListCreated->addItem("我的音乐");
    lyricListCreated->addItem("我创建的歌单1");
    lyricListCreated->addItem("我创建的歌单2");
    lyricListCreated->addItem("我创建的歌单3");
    lyricListCreated->addItem("我创建的歌单4");
    lyricListCreated->addItem("我创建的歌单1");
    lyricListCreated->addItem("我创建的歌单2");
    lyricListCreated->addItem("我创建的歌单3");
    lyricListCreated->addItem("我创建的歌单4");
    lyricListCreated->addItem("我创建的歌单1");
    lyricListCreated->addItem("我创建的歌单2");
    lyricListCreated->addItem("我创建的歌单3");
    lyricListCreated->addItem("我创建的歌单4");
    lyricListCreated->addItem("我创建的歌单1");
    lyricListCreated->addItem("我创建的歌单2");
    lyricListCreated->addItem("我创建的歌单3");
    lyricListCreated->addItem("我创建的歌单4");


    vListLayout->setMargin(0);
    vListLayout->addSpacerItem(new QSpacerItem(20,10,QSizePolicy::Fixed, QSizePolicy::Fixed));
    vListLayout->addWidget(lyricListHistory);
    vListLayout->addWidget(headerListCreated);
    vListLayout->addWidget(lyricListCreated);
    //vListLayout->addWidget(headerListTest);
    //vListLayout->addWidget(lyricListTest);
    vListLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));


    scrollAreaLeft = new QScrollArea(pageLyricListContainer);
    scrollAreaLeft->setMinimumWidth(250);
    scrollAreaLeft->setMaximumWidth(250);
    scrollAreaLeft->setWidgetResizable(true);
    scrollAreaLeft->setWidget(lyriclistLeftPanel);
    scrollAreaLeft->setObjectName("scrollAreaLeftList");
    scrollAreaLeft->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    scrollAreaLeft->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreaLeft->setFrameShape(QFrame::NoFrame);

    QHBoxLayout* hMainLayout = new QHBoxLayout(pageLyricListContainer);
    hMainLayout->addWidget(scrollAreaLeft);
    hMainLayout->addWidget(lyriclistRightPanel);
    hMainLayout->setMargin(0);
    hMainLayout->setSpacing(0);
}

void PageLyricList::initConnection()
{
    connect(headerListCreated, &BesListHeader::sig_toggleList,[=](bool show){lyricListCreated->setVisible(show);});

    connect(headerListCreated,SIGNAL(sig_addButtonClicked()),this,SLOT(addNewListItem()));

    //connect(headerListTest, &BesListHeader::sig_toggle_list,[=](bool show){lyricListTest->setVisible(show);});
    //connect(LyricListCreated,&QListWidget::itemClicked,this,&MainWindow::slot_my_Create_Music_List_itemClicked);

    QAbstractItemModel* model = lyricListCreated->model();
    connect(model,SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
                        this,SLOT(rowsMoved(QModelIndex,int,int,QModelIndex,int)));

}

void PageLyricList::rowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row, PageLyricList::QPrivateSignal)
{
    //(start,end)->row
}

void PageLyricList::addNewListItem()
{
    QString title = "new item";



}

