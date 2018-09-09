#include <Define/Define.h>
#include "PageLyricList.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>

PageLyricList::PageLyricList(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
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
    pageLyricListContainer->setObjectName("pageLyricListContainer");
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(pageLyricListContainer);

    lyriclistLeftPanel = new QWidget(pageLyricListContainer);
    lyriclistRightPanel = new QWidget(pageLyricListContainer);
    lyriclistLeftPanel->setObjectName("lyriclistLeftPanel");
    lyriclistRightPanel->setObjectName("lyriclistRightPanel");
    lyriclistLeftPanel->setMinimumWidth(250);
    lyriclistRightPanel->setMinimumWidth(250);
    lyriclistLeftPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    lyriclistRightPanel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QHBoxLayout* hMainLayout = new QHBoxLayout(pageLyricListContainer);
    hMainLayout->addWidget(lyriclistLeftPanel);
    hMainLayout->addWidget(lyriclistRightPanel);
    hMainLayout->setMargin(0);
    hMainLayout->setSpacing(0);

    //左侧列表

    QVBoxLayout* vListLayout = new QVBoxLayout(lyriclistLeftPanel);

    //创建的歌单
    LyricListCreated = new QListWidget(lyriclistLeftPanel);
    LyricListCreated->setStyleSheet("QListWidget{color:rgb(173,175,178); background:rgb(25,27,31);border:0px solid gray;}"
                                        "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}"
                                        "QListWidget::Item:hover{color:rgb(255,255,255);background:transparent;border:0px solid gray;}"
                                        "QListWidget::Item:selected{border-image:url(images/listwidget_h.png); color:rgb(255,255,255);border:0px solid gray;}"
                                        "QListWidget::Item:selected:active{background:#00FFFFFF;color:#FFFFFF;border-width:0;}");

    //去掉滚动条
    LyricListCreated->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    LyricListCreated->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    LyricListCreated->show();

    //创建列表测试
    QListWidgetItem *add_item_10 = new QListWidgetItem(LyricListCreated);
    add_item_10->setIcon(QIcon(":/resource/image/btn_skin_normal_white.png"));
    add_item_10->setText("我喜欢的音乐");
    QListWidgetItem *add_item_11 = new QListWidgetItem(LyricListCreated);
    add_item_11->setIcon(QIcon(":/resource/image/btn_skin_press_white.png"));
    add_item_11->setText("我的音乐");

    QListWidgetItem *add_item_12 = new QListWidgetItem(LyricListCreated);
    add_item_12->setIcon(QIcon(":/resource/image/btn_setting_normal_white.png"));
    add_item_12->setText("我创建的歌单1");

    QListWidgetItem *add_item_13 = new QListWidgetItem(LyricListCreated);
    add_item_13->setIcon(QIcon(":/resource/image/btn_setting_normal_white.png"));
    add_item_13->setText("我创建的歌单2");

    QListWidgetItem *add_item_14 = new QListWidgetItem(LyricListCreated);
    add_item_14->setIcon(QIcon(":/resource/image/btn_setting_normal_white.png"));
    add_item_14->setText("我创建的歌单3");

//    LyricListCreated->insertItem(-1,add_item_10);
//    LyricListCreated->insertItem(-1,add_item_11);
//    LyricListCreated->insertItem(-1,add_item_12);
//    LyricListCreated->insertItem(-1,add_item_13);
//    LyricListCreated->insertItem(-1,add_item_14);

    vListLayout->addWidget(LyricListCreated);



//    //收藏的歌单
//    my_Collect_Music_List = new QListWidget(ui->widget_list);
//    connect(my_Collect_Music_List,&QListWidget::itemClicked,this,&MainWindow::slot_my_my_Collect_Music_List_itemClicked);
//    my_Collect_Music_List->setStyleSheet("QListWidget{color:rgb(173,175,178); background:rgb(25,27,31);border:0px solid gray;}"
//                                         "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}"
//                                         "QListWidget::Item:hover{color:rgb(255,255,255);background:transparent;border:0px solid gray;}"
//                                         "QListWidget::Item:selected{border-image:url(images/listwidget_h.png); color:rgb(255,255,255);border:0px solid gray;}"
//                                         "QListWidget::Item:selected:active{background:#00FFFFFF;color:#FFFFFF;border-width:0;}");
//    //去掉滚动条
//    my_Collect_Music_List->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    my_Collect_Music_List->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    my_Collect_Music_List->hide();

//    //收藏歌单测试
//    QListWidgetItem *add_item_15 = new QListWidgetItem(my_Collect_Music_List);
//    add_item_15->setIcon(QIcon("./images/musiclist.png"));
//    add_item_15->setText("我收藏的歌单1");

//    QListWidgetItem *add_item_16 = new QListWidgetItem(my_Collect_Music_List);
//    add_item_16->setIcon(QIcon("./images/musiclist.png"));
//    add_item_16->setText("我收藏的歌单2");

//    QListWidgetItem *add_item_17 = new QListWidgetItem(my_Collect_Music_List);
//    add_item_17->setIcon(QIcon("./images/musiclist.png"));
//    add_item_17->setText("我收藏的歌单3");
}

void PageLyricList::initConnection()
{
    //connect(LyricListCreated,&QListWidget::itemClicked,this,&MainWindow::slot_my_Create_Music_List_itemClicked);

}
