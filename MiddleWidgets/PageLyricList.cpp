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
//    lyricListHistory->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉滚动条
//    lyricListHistory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lyricListHistory->setMaximumHeight(36*1);
    lyricListHistory->setMinimumHeight(36*1);
    lyricListHistory->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    lyricListHistory->setFocusPolicy(Qt::NoFocus);
    lyricListHistory->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    QListWidgetItem *add_item_0 = new QListWidgetItem(lyricListHistory);
    add_item_0->setIcon(QIcon(":/resource/image/btn_skin_normal_white.png"));
    add_item_0->setText("制作历史");

    //创建的歌单

    //表头
    headerListCreated = new BesListHeader("创建的歌词单",true,true,lyriclistLeftPanel);
    headerListCreated->setMaximumHeight(36);
    headerListCreated->setMinimumHeight(36);
    headerListCreated->setMinimumWidth(50);
    headerListCreated->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    //列表
    lyricListCreated = new BesList(lyriclistLeftPanel);
    lyricListCreated->setMaximumHeight(36*9);
    lyricListCreated->setMinimumHeight(36*9);
//    lyricListCreated->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉滚动条
//    lyricListCreated->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lyricListCreated->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
    lyricListCreated->setFocusPolicy(Qt::NoFocus);
    lyricListCreated->setDragDropMode(QListView::DragDropMode::InternalMove);
    lyricListCreated->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    //创建列表测试
    QListWidgetItem *add_item_10 = new QListWidgetItem(lyricListCreated);
    add_item_10->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_10->setText("我喜欢的音乐");
    QListWidgetItem *add_item_11 = new QListWidgetItem(lyricListCreated);
    add_item_11->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_11->setText("我的音乐");

    QListWidgetItem *add_item_12 = new QListWidgetItem(lyricListCreated);
    add_item_12->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_12->setText("我创建的歌单1");

    QListWidgetItem *add_item_13 = new QListWidgetItem(lyricListCreated);
    add_item_13->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_13->setText("我创建的歌单2");

    QListWidgetItem *add_item_14 = new QListWidgetItem(lyricListCreated);
    add_item_14->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_14->setText("我创建的歌单3");

    QListWidgetItem *add_item_15 = new QListWidgetItem(lyricListCreated);
    add_item_15->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_15->setText("我创建的歌单2");

    QListWidgetItem *add_item_16 = new QListWidgetItem(lyricListCreated);
    add_item_16->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_16->setText("我创建的歌单3");

    QListWidgetItem *add_item_17 = new QListWidgetItem(lyricListCreated);
    add_item_17->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_17->setText("我创建的歌单2");

    QListWidgetItem *add_item_18 = new QListWidgetItem(lyricListCreated);
    add_item_18->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_18->setText("我创建的歌单3");


    //表头
    headerListTest = new BesListHeader("收藏的歌词单",false ,true,lyriclistLeftPanel);
    headerListTest->setMaximumHeight(36);
    headerListTest->setMinimumHeight(36);
    headerListTest->setMinimumWidth(50);
    headerListTest->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    //列表
    lyricListTest = new BesList(lyriclistLeftPanel);
    lyricListTest->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
//    lyricListTest->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//去掉滚动条
//    lyricListTest->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lyricListTest->setFocusPolicy(Qt::NoFocus);
    lyricListTest->setDragDropMode(QListView::DragDropMode::InternalMove);

    lyricListTest->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
    lyricListTest->setMaximumHeight(36*5);
    lyricListTest->setMinimumHeight(36*5);

    //创建列表测试
    QListWidgetItem *add_item_20 = new QListWidgetItem(lyricListTest);
    add_item_20->setIcon(QIcon(":/resource/image/btn_setting_press_white.png"));
    add_item_20->setText("我收藏喜欢的音乐");
    QListWidgetItem *add_item_21 = new QListWidgetItem(lyricListTest);
    add_item_21->setIcon(QIcon(":/resource/image/btn_setting_press_white.png"));
    add_item_21->setText("我收藏的音乐");

    QListWidgetItem *add_item_22 = new QListWidgetItem(lyricListTest);
    add_item_22->setIcon(QIcon(":/resource/image/btn_setting_press_white.png"));
    add_item_22->setText("我收藏的歌单1");

    QListWidgetItem *add_item_23 = new QListWidgetItem(lyricListTest);
    add_item_23->setIcon(QIcon(":/resource/image/btn_setting_press_white.png"));
    add_item_23->setText("我收藏的歌单2");

    QListWidgetItem *add_item_24 = new QListWidgetItem(lyricListTest);
    add_item_24->setIcon(QIcon(":/resource/image/btn_setting_press_white.png"));
    add_item_24->setText("我收藏的歌单3");


    vListLayout->setMargin(0);
    vListLayout->addSpacerItem(new QSpacerItem(20,10,QSizePolicy::Fixed, QSizePolicy::Fixed));
    vListLayout->addWidget(lyricListHistory);
    vListLayout->addWidget(headerListCreated);
    vListLayout->addWidget(lyricListCreated);
    vListLayout->addWidget(headerListTest);
    vListLayout->addWidget(lyricListTest);
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
    connect(headerListCreated, &BesListHeader::sig_toggle_list,[=](bool show){lyricListCreated->setVisible(show);});
    connect(headerListTest, &BesListHeader::sig_toggle_list,[=](bool show){lyricListTest->setVisible(show);});
    //connect(LyricListCreated,&QListWidget::itemClicked,this,&MainWindow::slot_my_Create_Music_List_itemClicked);

    QAbstractItemModel* model = lyricListCreated->model();
    connect(model,SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this,SLOT(rowsMoved(QModelIndex,int,int,QModelIndex,int)));
}

void PageLyricList::rowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row, PageLyricList::QPrivateSignal)
{
    //(start,end)->row
}

