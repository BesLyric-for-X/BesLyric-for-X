#include <Define/Define.h>
#include "PageLyricList.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QDebug>
#include <QPixmap>
#include <QFileDialog>
#include <assert.h>
#include "BesMessageBox.h"

PageLyricList::PageLyricList(QWidget *parent)
    : QWidget(parent),pCurrentLyricList(nullptr)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    initEntity();
    initLayout();
    initConnection();
}

PageLyricList::~PageLyricList()
{

}

void PageLyricList::initEntity()
{
    listData = LyricListManager::GetInstance().getLyricListData();
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
    lyricListHistory->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    lyricListHistory->setFocusPolicy(Qt::NoFocus);
    lyricListHistory->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    lyricListHistory->setLyricLists(listData.listsHistory);

    //创建的歌单

    //表头
    headerListCreated = new BesListHeader("创建的歌词单",true,true,lyriclistLeftPanel);
    headerListCreated->setMaximumHeight(36);
    headerListCreated->setMinimumHeight(36);
    headerListCreated->setMinimumWidth(50);
    headerListCreated->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    //列表
    lyricListCreated = new BesList(lyriclistLeftPanel);
    lyricListCreated->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);
    lyricListCreated->setFocusPolicy(Qt::NoFocus);
    lyricListCreated->setDragDropMode(QListView::DragDropMode::InternalMove);
    lyricListCreated->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    lyricListCreated->setLyricLists(listData.listsCreated);

    //左侧控件布局
    vListLayout->setMargin(0);
    vListLayout->addSpacerItem(new QSpacerItem(20,10,QSizePolicy::Fixed, QSizePolicy::Fixed));
    vListLayout->addWidget(lyricListHistory);
    vListLayout->addWidget(headerListCreated);
    vListLayout->addWidget(lyricListCreated);
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

    //右侧控件
    labelListCoverRect = new QLabel(lyriclistRightPanel);
    labelListCoverRect->setObjectName("labelListCoverRect");
    labelListCoverRect->setPixmap(QPixmap(":/resource/image/default_list_cover.png"));
    labelListCoverRect->setMinimumSize(245,245);
    labelListCoverRect->setMaximumSize(245,245);
    labelListCoverRect->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    widgetListInfoRight = new QWidget(lyriclistRightPanel);

    labelLyricListRedMark= new QLabel(widgetListInfoRight);
    labelListInfoTitle= new QLabel(widgetListInfoRight);

    labelLyricListRedMark->setObjectName("labelLyricListRedMark");
    labelListInfoTitle->setObjectName("labelListInfoTitle");

    labelLyricListRedMark->setText("歌词单");
    labelListInfoTitle->setText(tr("歌词单标题"));
    labelLyricListRedMark->setAlignment(Qt::AlignCenter);
    labelLyricListRedMark->setMinimumSize(60,26);
    labelLyricListRedMark->setMaximumSize(60,26);
    labelLyricListRedMark->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelListInfoTitle->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);

    btnPackupLyricList = new BesButton(widgetListInfoRight);
    btnPackupLyricList->setObjectName("btnPackupLyricList");

    QHBoxLayout* hLayoutListInfo = new QHBoxLayout();
    hLayoutListInfo->addWidget(labelLyricListRedMark);
    hLayoutListInfo->addWidget(labelListInfoTitle);
    hLayoutListInfo->addWidget(btnPackupLyricList);
    hLayoutListInfo->setSpacing(2);

    QVBoxLayout* vLayoutListInfo = new QVBoxLayout();
    vLayoutListInfo->addLayout(hLayoutListInfo);
    vLayoutListInfo->addSpacerItem(new QSpacerItem(20,200,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    widgetListInfoRight->setMinimumHeight(250);
    widgetListInfoRight->setMaximumHeight(250);
    widgetListInfoRight->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    QHBoxLayout* hLayoutTop = new QHBoxLayout();
    hLayoutTop->addWidget(labelListCoverRect);
    hLayoutTop->addLayout(vLayoutListInfo);
    hLayoutTop->setSpacing(25);
    hLayoutTop->setMargin(30);

    tabpageLyricList = new  QTabWidget(pageLyricListContainer);

    tableLrcList = new BesLListTableView(tabpageLyricList);
    widgetEditLyricItem = new QWidget(tabpageLyricList);
    widgetEditListInfo = new QWidget(tabpageLyricList);

    tabpageLyricList->setObjectName("tabpageLyricList");
    tabpageLyricList->addTab(tableLrcList, tr("歌词单列表"));
    tabpageLyricList->addTab(widgetEditLyricItem,tr("创建/编辑项"));
    tabpageLyricList->addTab(widgetEditListInfo,tr("编辑歌词单"));

    tabpageLyricList->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    QVBoxLayout* vLayoutRight = new QVBoxLayout(lyriclistRightPanel);
    vLayoutRight->addLayout(hLayoutTop);
    vLayoutRight->addWidget(tabpageLyricList);


    //编辑歌单项
    labelLrcItemSongPath = new QLabel(widgetEditLyricItem);
    labelLrcItemLrcPath  = new QLabel(widgetEditLyricItem);
    editLrcItemSongPath = new QLineEdit(widgetEditLyricItem);
    editLrcItemLrcPath  = new QLineEdit(widgetEditLyricItem);
    btnSelectLrcItemSongPath = new BesButton(widgetEditLyricItem);
    btnSelectLrcItemLrcPath  = new BesButton(widgetEditLyricItem);
    btnSaveLrcItem     = new BesButton(widgetEditLyricItem);
    btnCreateLrcItem   = new BesButton(widgetEditLyricItem);
    btnSaveLrcItem->setObjectName("btnSaveLrcItem");
    btnCreateLrcItem->setObjectName("btnCreateLrcItem");

    labelLrcItemSongPath->setText(tr("歌曲路径"));
    labelLrcItemLrcPath->setText(tr("歌词路径"));
    editLrcItemSongPath->setFocusPolicy(Qt::NoFocus);
    editLrcItemLrcPath->setFocusPolicy(Qt::NoFocus);
    editLrcItemSongPath->setMinimumHeight(35);
    editLrcItemSongPath->setMaximumHeight(35);
    editLrcItemLrcPath->setMinimumHeight(35);
    editLrcItemLrcPath->setMaximumHeight(35);
    editLrcItemSongPath->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed );
    editLrcItemLrcPath->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed );

    btnSelectLrcItemSongPath->setText(tr("选择"));
    btnSelectLrcItemLrcPath->setText(tr("选择"));
    btnSaveLrcItem->setText("保存");
    btnCreateLrcItem->setText("新建");
    btnSelectLrcItemSongPath->setMinimumWidth(80);
    btnSelectLrcItemSongPath->setMaximumWidth(120);
    btnSelectLrcItemLrcPath->setMinimumWidth(80);
    btnSelectLrcItemLrcPath->setMaximumWidth(120);
    btnSaveLrcItem->setMinimumSize(80,35);
    btnSaveLrcItem->setMaximumSize(80,35);
    btnCreateLrcItem->setMinimumSize(80,35);
    btnCreateLrcItem->setMaximumSize(80,35);

    QHBoxLayout* hEditItemSong = new QHBoxLayout();
    hEditItemSong->addWidget(labelLrcItemSongPath);
    hEditItemSong->addWidget(editLrcItemSongPath);
    hEditItemSong->addWidget(btnSelectLrcItemSongPath);

    QHBoxLayout* hEditItemLrc = new QHBoxLayout();
    hEditItemLrc->addWidget(labelLrcItemLrcPath);
    hEditItemLrc->addWidget(editLrcItemLrcPath);
    hEditItemLrc->addWidget(btnSelectLrcItemLrcPath);

    QHBoxLayout* hEditItemSave = new QHBoxLayout();
    hEditItemSave->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    hEditItemSave->addWidget(btnSaveLrcItem);
    hEditItemSave->addWidget(btnCreateLrcItem);

    QVBoxLayout* hEditItem = new QVBoxLayout(widgetEditLyricItem);
    hEditItem->addLayout(hEditItemSong);
    hEditItem->addLayout(hEditItemLrc);
    hEditItem->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hEditItem->addLayout(hEditItemSave);
    hEditItem->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));

    //编辑歌词单信息
    labelModifyLrcListName = new QLabel(widgetEditListInfo);
    editModifyLrcListName= new QLineEdit(widgetEditListInfo);
    labelModifyListCoverRect= new QLabel(widgetEditListInfo);
    btnModifyListCover = new BesButton(widgetEditListInfo);

    labelModifyLrcListName->setText(tr("歌词单名:"));
    labelModifyLrcListName->setMinimumSize(80,30);
    labelModifyLrcListName->setMaximumSize(80,30);
    labelModifyLrcListName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    editModifyLrcListName->setMinimumHeight(35);
    editModifyLrcListName->setMaximumHeight(35);
    editModifyLrcListName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    labelModifyListCoverRect->setObjectName("labelModifyListCoverRect");
    labelModifyListCoverRect->setPixmap(QPixmap(":/resource/image/default_list_cover.png"));
    labelModifyListCoverRect->setMinimumSize(245,245);
    labelModifyListCoverRect->setMaximumSize(245,245);
    labelModifyListCoverRect->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    btnModifyListCover->setMinimumSize(120,35);
    btnModifyListCover->setMaximumSize(120,35);
    btnModifyListCover->setText(tr("编辑封面"));
    btnModifyListCover->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    btnDeleteLrcList  = new BesButton(widgetEditListInfo);
    btnSaveLrcListModified  = new BesButton(widgetEditListInfo);
    btnDeleteLrcList->setObjectName("btnDeleteLrcList");
    btnSaveLrcListModified->setObjectName("btnSaveLrcListModified");

    btnDeleteLrcList->setText(tr("删除整个歌词单"));
    btnSaveLrcListModified->setText(tr("保存"));
    btnDeleteLrcList->setMinimumSize(160,35);
    btnDeleteLrcList->setMaximumSize(160,35);
    btnSaveLrcListModified->setMinimumSize(100,35);
    btnSaveLrcListModified->setMaximumSize(100,35);
    btnDeleteLrcList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSaveLrcListModified->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout* hModifyListName = new QHBoxLayout();
    hModifyListName->addWidget(labelModifyLrcListName);
    hModifyListName->addWidget(editModifyLrcListName);

    QHBoxLayout* hModifyListButtons = new QHBoxLayout();
    hModifyListButtons->addWidget(btnDeleteLrcList);
    hModifyListButtons->addWidget(btnSaveLrcListModified);
    hModifyListButtons->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ));
    hModifyListButtons->addWidget(btnModifyListCover);

    QVBoxLayout* vModifyListInfoLeft = new QVBoxLayout();
    vModifyListInfoLeft->addLayout(hModifyListName);
    vModifyListInfoLeft->addSpacerItem(new QSpacerItem(20,100,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ));
    vModifyListInfoLeft->addLayout(hModifyListButtons);

    QHBoxLayout* hModifyListTop = new QHBoxLayout();
    hModifyListTop->addLayout(vModifyListInfoLeft);
    hModifyListTop->addWidget(labelModifyListCoverRect);
    hModifyListTop->setSpacing(15);

    QVBoxLayout* vModifyListInfo = new QVBoxLayout(widgetEditListInfo);
    vModifyListInfo->addLayout(hModifyListTop);
    vModifyListInfo->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ));

    //整体布局
    QHBoxLayout* hMainLayout = new QHBoxLayout(pageLyricListContainer);
    hMainLayout->addWidget(scrollAreaLeft);
    hMainLayout->addWidget(lyriclistRightPanel);
    hMainLayout->setMargin(0);
    hMainLayout->setSpacing(0);
}

void PageLyricList::initConnection()
{
    connect(headerListCreated, &BesListHeader::sig_toggleList,[=](bool show){lyricListCreated->setVisible(show);});
    //connect(headerListCreated,SIGNAL(sig_addButtonClicked()),this,SLOT(addNewListItem()));

    //列表互斥选中项
    connect(lyricListCreated,&QListWidget::itemClicked,this,[=](){
        if(lyricListHistory->getCurrentIndex() != -1)lyricListHistory->setCurrentRow(-1);
    });
    connect(lyricListHistory,&QListWidget::itemClicked,this,[=](){
        if(lyricListCreated->getCurrentIndex() != -1)lyricListCreated->setCurrentRow(-1);
    });

    //选中项，则将对应的数据的指针传出来用于显示、编辑等
    connect(lyricListCreated,&QListWidget::currentRowChanged,[=](int currentRow){
        if(currentRow != -1)
        {
            lyricListCurrent = lyricListCreated;
            reloadLyricListData(lyricListCreated->getCurrentItemData());
        }
    });

    connect(lyricListHistory,&QListWidget::currentRowChanged,[=](int currentRow){
        if(currentRow != -1)
        {
            lyricListCurrent = lyricListHistory;
            reloadLyricListData(lyricListHistory->getCurrentItemData());
        }
    });

    connect(lyricListCreated,SIGNAL(sig_listDataChanged()),
            this, SLOT(OnSaveLyricListData()));


    QAbstractItemModel* model = lyricListCreated->model();
    connect(model,SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
                        this,SLOT(OnRowsMoved(QModelIndex,int,int,QModelIndex,int)));


    //连接按钮动作
    connect(btnSelectLrcItemSongPath,SIGNAL(clicked(bool)),SLOT(OnSelectSongPath()));
    connect(btnSelectLrcItemLrcPath,SIGNAL(clicked(bool)),SLOT(OnSelectLrcPath()));
    connect(btnSaveLrcItem,SIGNAL(clicked(bool)),SLOT(OnSaveLrcListItem()));
    connect(btnCreateLrcItem,SIGNAL(clicked(bool)),SLOT(OnCreateLrcListItem()));

    connect(tableLrcList,SIGNAL(sig_deleteItem(int)),this,SLOT(OnDeleteListItem(int)));
    connect(tableLrcList,SIGNAL(sig_editItem(int)),this,SLOT(OnEditListItem(int)));

    connect(btnSaveLrcListModified,SIGNAL(clicked(bool)), this, SLOT(OnSaveListInfo()));
    connect(btnDeleteLrcList,SIGNAL(clicked(bool)), this, SLOT(OnDeleteLrcList()));

    //连接完后，默认选中第一项
    lyricListHistory->setCurrentRow(0); //必有一项，默认选中

    enableEditMode(false);              //默认不在编辑模式

    headerListCreated->OnMakeSureHeaderChecking();  //默认列表是展开的
}

void PageLyricList::OnRowsMoved(const QModelIndex &parent, int start, int end,
                                const QModelIndex &destination, int row, PageLyricList::QPrivateSignal)
{
    Q_UNUSED(parent)
    Q_UNUSED(end)
    Q_UNUSED(destination)
	//(start,end)->row
	lyricListCreated->moveRow(start, row);
}

void PageLyricList::OnAddNewListItem(QString itemName)
{
    lyricListCreated->addItem(itemName);

    headerListCreated->OnMakeSureHeaderChecking();
}

bool PageLyricList::OnDeleteCurrentItem(bool bDeleteConformRequested)
{
    if(bDeleteConformRequested)
    {
        if(QMessageBox::StandardButton::Ok ==
                BesMessageBox::question(tr("温馨提示"), tr("该歌词单中包含有 %1 个歌词项\n\n是否确定删除 [%2]？").arg(
                    pCurrentLyricList->items.size()).arg(pCurrentLyricList->name)))
        {
            lyricListCreated->deleteCurrentItem();
            return true;
        }
    }
    else
    {
        lyricListCreated->deleteCurrentItem();
        return true;
    }

    return false;
}

void PageLyricList::OnSelectSongPath()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择歌曲"),
                                                      "/home",
                                                      tr("音频 (*.mp3 *.wav);;视频 (*.mp4)"));

    if(fileName.size() !=0)
    {
        editLrcItemSongPath->setText(fileName);
    }
}

void PageLyricList::OnSelectLrcPath()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择歌词"), "/home",
                                                      tr("文本 (*.lrc);;其他 (*.*)"));
    if(fileName.size() !=0)
    {
        editLrcItemLrcPath->setText(fileName);
    }
}

void PageLyricList::OnSaveLrcListItem()
{
    QString song = editLrcItemSongPath->text();
    QString lrc = editLrcItemLrcPath->text();

    assert(bEnableEditMode == true);

    if(pCurrentLyricList->items[currentEditItem].song == song
            && pCurrentLyricList->items[currentEditItem].lyric ==lrc)
    {
        BesMessageBox::information(tr("提示"),tr("没有发生任何更改 ：）"));
        return;
    }

    pCurrentLyricList->items[currentEditItem].song = song;//这里的更改直接对  listData 生效
    pCurrentLyricList->items[currentEditItem].lyric = lrc;

    OnSaveLyricListData();                 //触发保存

    enableEditMode(false);                  //退出编辑模式

    //切换到列表页面
    tabpageLyricList->setCurrentIndex(0);
}

void PageLyricList::OnCreateLrcListItem()
{
    QString song = editLrcItemSongPath->text();
    QString lrc = editLrcItemLrcPath->text();

    if(song.size() == 0)
    {
        BesMessageBox::information(tr("提示"),tr("请先选择歌曲"));
        return;
    }
    if(lrc.size() == 0)
    {
        BesMessageBox::information(tr("提示"),tr("请先选择歌词"));
        return;
    }

    for(LyricListItem& item: pCurrentLyricList->items)
    {
        if(item.song == song && item.lyric ==lrc)
        {
            BesMessageBox::information(tr("提示"),tr("当前歌曲和歌词组合已存在 ：）"));
            return;
        }
    }

    LyricListItem item;
    item.song = song;
    item.lyric = lrc;
    pCurrentLyricList->items.push_back(item);  //这里的更改直接对  listData 生效
    OnSaveLyricListData();                     //触发保存

    tableLrcList->setDataSource(pCurrentLyricList); //重置使表重载数据

    enableEditMode(false);                  //确保退出编辑模式（由于编辑时可能保持也可能新建）

    //切换到列表页面
    tabpageLyricList->setCurrentIndex(0);
}

void PageLyricList::OnDeleteListItem(int row)
{
    pCurrentLyricList->items.removeAt(row);//这里的更改直接对  listData 生效
    OnSaveLyricListData();                 //触发保存

    tableLrcList->reloadTableFromData(); //表格数据改变，让其重显示数据

    //删除项将自动退出编辑模式
    enableEditMode(false);
}

void PageLyricList::OnEditListItem(int row)
{
    LyricListItem item = pCurrentLyricList->items.at(row);

    editLrcItemSongPath->setText(item.song);
    editLrcItemLrcPath->setText(item.lyric);

    enableEditMode(true, row);

    //切换到编辑页面
    tabpageLyricList->setCurrentIndex(1);
}

void PageLyricList::OnSaveListInfo()
{
    QString text = editModifyLrcListName->text().trimmed();

    bool bModified = false;
    if(pCurrentLyricList->name != text)
    {
        pCurrentLyricList->name = text;  //这里的更改直接对  listData 生效

        labelListInfoTitle->setText(text);          //页面显示改变
        lyricListCreated->reloadAllItemText();      //左侧列表显示改变

        bModified = true;
    }

    if(bModified)
    {
        OnSaveLyricListData();                 //触发保存
    }

    //无论是否真正保存，都切换到列表页面
    tabpageLyricList->setCurrentIndex(0);
}

void PageLyricList::OnDeleteLrcList()
{
    if(OnDeleteCurrentItem(pCurrentLyricList->items.size() != 0))
    {
        lyricListCreated->setCurrentRow(-1);
        lyricListHistory->setCurrentRow(0); //删除后，默认选中制作历史歌词单;必有一项，默认选中
    }
}

void PageLyricList::OnSaveLyricListData()
{
    LyricListManager::GetInstance().saveLyricListData(listData);
}

void PageLyricList::OnAddToMakingHistory(QString song, QString lrc)
{
    LyricList& historyList = listData.listsHistory[0];
    for(LyricListItem& item: historyList.items)
    {
        if(item.song == song && item.lyric ==lrc)
        {
            return;  //已存在
        }
    }

    LyricListItem item;
    item.song = song;
    item.lyric = lrc;
    historyList.items.push_back(item);        //这里的更改直接对  listData 生效
    OnSaveLyricListData();                     //触发保存

    if(lyricListCurrent == lyricListHistory) //如果当前在历史列表页面，需要重载入界面数据
    {
        tableLrcList->setDataSource(pCurrentLyricList); //重置使表重载数据

        enableEditMode(false);                  //确保退出编辑模式（由于编辑时可能保持也可能新建）

        //切换到列表页面
        tabpageLyricList->setCurrentIndex(0);
    }
    else
    {
        //如果当前不在历史表，不需要做重载处理 ,从别处点击历史列表，会自动重载
    }
}

void PageLyricList::reloadLyricListData(LyricList *pLyricListData)
{
    pCurrentLyricList = pLyricListData;

    labelListInfoTitle->setText( pCurrentLyricList->name);
    editModifyLrcListName->setText(pCurrentLyricList->name);
    tableLrcList->setDataSource(pCurrentLyricList);

    //重载将自动退出编辑模式
    enableEditMode(false);

    //历史记录列表不让编辑名字和删除
    bool bIsNotHistory = lyricListCurrent != lyricListHistory;
    editModifyLrcListName->setEnabled(bIsNotHistory);
    btnDeleteLrcList->setEnabled(bIsNotHistory);


    //切换到列表页面
    tabpageLyricList->setCurrentIndex(0);
}

void PageLyricList::enableEditMode(bool bEnable, int indexWhenEnable)
{
    bEnableEditMode = bEnable;

    if(bEnableEditMode)
    {
        currentEditItem = indexWhenEnable;
        btnSaveLrcItem->setVisible(true);
    }
    else
    {
        btnSaveLrcItem->setVisible(false);
    }
}


