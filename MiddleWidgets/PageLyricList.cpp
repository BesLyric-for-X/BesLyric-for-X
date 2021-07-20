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
#include "SettingManager.h"
#include "BesScaleUtil.h"

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

    lyriclistLeftPanel->setMinimumWidth(250* BesScaleUtil::scale());
    lyriclistLeftPanel->setMaximumWidth(250* BesScaleUtil::scale());
    lyriclistLeftPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);

    //左侧列表
    lyricListContainer = new QWidget(lyriclistLeftPanel);
    lyricListContainer->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    lyricListContainer->setObjectName("lyricListContainer");
    lyricListContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

    QVBoxLayout* vListLayout = new QVBoxLayout(lyricListContainer);

    //制作历史歌词单
    lyricListHistory = new BesList(lyricListContainer);
    lyricListHistory->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    lyricListHistory->setFocusPolicy(Qt::NoFocus);
    lyricListHistory->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    lyricListHistory->setLyricLists(listData.listsHistory);

    //创建的歌单

    //表头
    headerListCreated = new BesListHeader("创建的歌词单",true,true,lyricListContainer);
    headerListCreated->setMaximumHeight(36 * BesScaleUtil::scale());
    headerListCreated->setMinimumHeight(36 * BesScaleUtil::scale());
    headerListCreated->setMinimumWidth(250* BesScaleUtil::scale());
    headerListCreated->setMaximumWidth(250* BesScaleUtil::scale());
    headerListCreated->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //列表
    lyricListCreated = new BesList(lyricListContainer);
    lyricListCreated->setMinimumWidth(250* BesScaleUtil::scale());
    lyricListCreated->setMaximumWidth(250* BesScaleUtil::scale());
    lyricListCreated->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lyricListCreated->setFocusPolicy(Qt::NoFocus);
    lyricListCreated->setDragDropMode(QListView::DragDropMode::InternalMove);
    lyricListCreated->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    lyricListCreated->setLyricLists(listData.listsCreated);

    //左侧控件布局
    vListLayout->setMargin(0);
    vListLayout->addSpacerItem(new QSpacerItem(20,10* BesScaleUtil::scale(),QSizePolicy::Fixed, QSizePolicy::Fixed));
    vListLayout->addWidget(lyricListHistory);
    vListLayout->addWidget(headerListCreated);
    vListLayout->addWidget(lyricListCreated);
    vListLayout->addSpacerItem(new QSpacerItem(20,20* BesScaleUtil::scale(),QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));

    scrollAreaLeft = new QScrollArea(pageLyricListContainer);
    scrollAreaLeft->setMinimumWidth(250* BesScaleUtil::scale());
    scrollAreaLeft->setMaximumWidth(250* BesScaleUtil::scale());
    scrollAreaLeft->setWidgetResizable(true);
    scrollAreaLeft->setWidget(lyricListContainer);
    scrollAreaLeft->setObjectName("scrollAreaLeftList");
    scrollAreaLeft->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    scrollAreaLeft->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollAreaLeft->setFrameShape(QFrame::NoFrame);

    //box预览歌词
    boxPagePreviewLyric = new BoxPagePreviewLyric(lyriclistLeftPanel);
    boxPagePreviewLyric->setObjectName("boxPagePreviewLyric");
    boxPagePreviewLyric->setMinimumSize(100* BesScaleUtil::scale(),(55+10)* BesScaleUtil::scale());
    boxPagePreviewLyric->setMaximumSize(300* BesScaleUtil::scale(),(55+10)* BesScaleUtil::scale());
    boxPagePreviewLyric->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    QVBoxLayout* vLeftPanelLayout = new QVBoxLayout(lyriclistLeftPanel);
    vLeftPanelLayout->setMargin(0);
    vLeftPanelLayout->setSpacing(0);
    vLeftPanelLayout->addWidget(scrollAreaLeft);
    vLeftPanelLayout->addWidget(boxPagePreviewLyric);

    //右侧控件
    labelListCoverRect = new QLabel(lyriclistRightPanel);
    labelListCoverRect->setObjectName("labelListCoverRect");
    labelListCoverRect->setMinimumSize(245* BesScaleUtil::scale(),245* BesScaleUtil::scale());
    labelListCoverRect->setMaximumSize(245* BesScaleUtil::scale(),245* BesScaleUtil::scale());
    labelListCoverRect->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QPixmap pixmapCover(":/resource/image/default_list_cover.png");
    QPixmap pixmapCoverScaled = pixmapCover.scaled(245* BesScaleUtil::scale(), 245* BesScaleUtil::scale(),
                                           Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    labelListCoverRect->setPixmap(pixmapCoverScaled);

    widgetListInfoRight = new QWidget(lyriclistRightPanel);

    labelLyricListRedMark= new QLabel(widgetListInfoRight);
    labelListInfoTitle= new QLabel(widgetListInfoRight);

    labelLyricListRedMark->setObjectName("labelLyricListRedMark");
    labelListInfoTitle->setObjectName("labelListInfoTitle");

    labelLyricListRedMark->setText("歌词单");
    labelListInfoTitle->setText(tr("歌词单标题"));
    labelLyricListRedMark->setAlignment(Qt::AlignCenter);
    labelLyricListRedMark->setMinimumSize(60 * BesScaleUtil::scale(), 26 * BesScaleUtil::scale());
    labelLyricListRedMark->setMaximumSize(60 * BesScaleUtil::scale(), 26 * BesScaleUtil::scale());
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
    vLayoutListInfo->addSpacerItem(new QSpacerItem(20,200* BesScaleUtil::scale(),QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

    widgetListInfoRight->setMinimumHeight(250* BesScaleUtil::scale());
    widgetListInfoRight->setMaximumHeight(250* BesScaleUtil::scale());
    widgetListInfoRight->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    QHBoxLayout* hLayoutTop = new QHBoxLayout();
    hLayoutTop->addWidget(labelListCoverRect);
    hLayoutTop->addLayout(vLayoutListInfo);
    hLayoutTop->setSpacing(25* BesScaleUtil::scale());
    hLayoutTop->setMargin(30* BesScaleUtil::scale());

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
    editLrcItemSongPath = new BesFileLineEdit(BesFileTypeMusic, widgetEditLyricItem);
    editLrcItemLrcPath  = new BesFileLineEdit(BesFileTypeLrc ,widgetEditLyricItem);
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
    editLrcItemSongPath->setMinimumHeight(35* BesScaleUtil::scale());
    editLrcItemSongPath->setMaximumHeight(35* BesScaleUtil::scale());
    editLrcItemLrcPath->setMinimumHeight(35* BesScaleUtil::scale());
    editLrcItemLrcPath->setMaximumHeight(35* BesScaleUtil::scale());
    editLrcItemSongPath->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed );
    editLrcItemLrcPath->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed );
    editLrcItemSongPath->setPlaceholderText(tr("点击选择文件 或 拖放文件到这里"));
    editLrcItemLrcPath->setPlaceholderText(tr("点击选择文件 或 拖放文件到这里"));

    btnSelectLrcItemSongPath->setText(tr("选择"));
    btnSelectLrcItemLrcPath->setText(tr("选择"));
    btnSaveLrcItem->setText("保存");
    btnCreateLrcItem->setText("新建");
    btnSelectLrcItemSongPath->setMinimumWidth(80* BesScaleUtil::scale());
    btnSelectLrcItemSongPath->setMaximumWidth(120* BesScaleUtil::scale());
    btnSelectLrcItemLrcPath->setMinimumWidth(80* BesScaleUtil::scale());
    btnSelectLrcItemLrcPath->setMaximumWidth(120* BesScaleUtil::scale());
    btnSaveLrcItem->setMinimumSize(80* BesScaleUtil::scale(),35* BesScaleUtil::scale());
    btnSaveLrcItem->setMaximumSize(80* BesScaleUtil::scale(),35* BesScaleUtil::scale());
    btnCreateLrcItem->setMinimumSize(80* BesScaleUtil::scale(),35* BesScaleUtil::scale());
    btnCreateLrcItem->setMaximumSize(80* BesScaleUtil::scale(),35* BesScaleUtil::scale());

    QHBoxLayout* hEditItemSong = new QHBoxLayout();
    hEditItemSong->addWidget(labelLrcItemSongPath);
    hEditItemSong->addWidget(editLrcItemSongPath);
    hEditItemSong->addWidget(btnSelectLrcItemSongPath);

    QHBoxLayout* hEditItemLrc = new QHBoxLayout();
    hEditItemLrc->addWidget(labelLrcItemLrcPath);
    hEditItemLrc->addWidget(editLrcItemLrcPath);
    hEditItemLrc->addWidget(btnSelectLrcItemLrcPath);

    QHBoxLayout* hEditItemSave = new QHBoxLayout();
    hEditItemSave->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    hEditItemSave->addWidget(btnSaveLrcItem);
    hEditItemSave->addWidget(btnCreateLrcItem);

    QVBoxLayout* hEditItem = new QVBoxLayout(widgetEditLyricItem);
    hEditItem->addLayout(hEditItemSong);
    hEditItem->addLayout(hEditItemLrc);
    hEditItem->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hEditItem->addLayout(hEditItemSave);
    hEditItem->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));

    //编辑歌词单信息
    labelModifyLrcListName = new QLabel(widgetEditListInfo);
    editModifyLrcListName= new QLineEdit(widgetEditListInfo);
    labelModifyListCoverRect= new QLabel(widgetEditListInfo);
    btnModifyListCover = new BesButton(widgetEditListInfo);

    labelModifyLrcListName->setText(tr("歌词单名:"));
    labelModifyLrcListName->setMinimumSize(80* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    labelModifyLrcListName->setMaximumSize(80* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    labelModifyLrcListName->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    editModifyLrcListName->setMinimumHeight(35* BesScaleUtil::scale());
    editModifyLrcListName->setMaximumHeight(35* BesScaleUtil::scale());
    editModifyLrcListName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    labelModifyListCoverRect->setObjectName("labelModifyListCoverRect");
    QPixmap pixmapCover2(":/resource/image/default_list_cover.png");
    QPixmap pixmapCoverScaled2 = pixmapCover2.scaled(245* BesScaleUtil::scale(), 245* BesScaleUtil::scale(),
                                           Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    labelModifyListCoverRect->setPixmap(pixmapCoverScaled2);
    labelModifyListCoverRect->setMinimumSize(245* BesScaleUtil::scale(),245* BesScaleUtil::scale());
    labelModifyListCoverRect->setMaximumSize(245* BesScaleUtil::scale(),245* BesScaleUtil::scale());
    labelModifyListCoverRect->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelModifyListCoverRect->setVisible(false);

    btnModifyListCover->setMinimumSize(120* BesScaleUtil::scale(),35* BesScaleUtil::scale());
    btnModifyListCover->setMaximumSize(120* BesScaleUtil::scale(),35* BesScaleUtil::scale());
    btnModifyListCover->setText(tr("编辑封面"));
    btnModifyListCover->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnModifyListCover->setVisible(false);

    btnDeleteLrcList  = new BesButton(widgetEditListInfo);
    btnSaveLrcListModified  = new BesButton(widgetEditListInfo);
    btnDeleteLrcList->setObjectName("btnDeleteLrcList");
    btnSaveLrcListModified->setObjectName("btnSaveLrcListModified");

    btnDeleteLrcList->setText(tr("删除整个歌词单"));
    btnSaveLrcListModified->setText(tr("保存"));
    btnDeleteLrcList->setMinimumSize(160* BesScaleUtil::scale(),35* BesScaleUtil::scale());
    btnDeleteLrcList->setMaximumSize(160* BesScaleUtil::scale(),35* BesScaleUtil::scale());
    btnSaveLrcListModified->setMinimumSize(100* BesScaleUtil::scale(),35* BesScaleUtil::scale());
    btnSaveLrcListModified->setMaximumSize(100* BesScaleUtil::scale(),35* BesScaleUtil::scale());
    btnDeleteLrcList->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSaveLrcListModified->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout* hModifyListName = new QHBoxLayout();
    hModifyListName->addWidget(labelModifyLrcListName);
    hModifyListName->addWidget(editModifyLrcListName);

    QHBoxLayout* hModifyListButtons = new QHBoxLayout();
    hModifyListButtons->addWidget(btnDeleteLrcList);
    hModifyListButtons->addWidget(btnSaveLrcListModified);
    hModifyListButtons->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ));
    hModifyListButtons->addWidget(btnModifyListCover);

    QVBoxLayout* vModifyListInfoLeft = new QVBoxLayout();
    vModifyListInfoLeft->addLayout(hModifyListName);
    vModifyListInfoLeft->addSpacerItem(new QSpacerItem(20,100* BesScaleUtil::scale(),QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ));
    vModifyListInfoLeft->addLayout(hModifyListButtons);

    QHBoxLayout* hModifyListTop = new QHBoxLayout();
    hModifyListTop->addLayout(vModifyListInfoLeft);
    hModifyListTop->addWidget(labelModifyListCoverRect);
    hModifyListTop->setSpacing(15* BesScaleUtil::scale());

    QVBoxLayout* vModifyListInfo = new QVBoxLayout(widgetEditListInfo);
    vModifyListInfo->addLayout(hModifyListTop);
    vModifyListInfo->addSpacerItem(new QSpacerItem(20,20* BesScaleUtil::scale(),QSizePolicy::Fixed, QSizePolicy::MinimumExpanding ));

    //整体布局
    QHBoxLayout* hMainLayout = new QHBoxLayout(pageLyricListContainer);
    hMainLayout->addWidget(lyriclistLeftPanel);
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

    connect(lyricListCreated, &BesList::sig_listDataChanged, this, &PageLyricList::OnSaveLyricListData);

    //连接按钮动作
    connect(btnSelectLrcItemSongPath, &BesButton::clicked, this, &PageLyricList::OnSelectSongPath);
    connect(btnSelectLrcItemLrcPath, &BesButton::clicked, this, &PageLyricList::OnSelectLrcPath);
    connect(btnSaveLrcItem, &BesButton::clicked, this, &PageLyricList::OnSaveLrcListItem);
    connect(btnCreateLrcItem, &BesButton::clicked, this, &PageLyricList::OnCreateLrcListItem);

    connect(tableLrcList, &BesLListTableView::sig_deleteItem, this, &PageLyricList::OnDeleteListItem);
    connect(tableLrcList, &BesLListTableView::sig_editItem, this, &PageLyricList::OnEditListItem);
    connect(tableLrcList, &BesLListTableView::sig_saveLyricListData, this, &PageLyricList::OnSaveLyricListData);

    connect(btnSaveLrcListModified, &BesButton::clicked, this, &PageLyricList::OnSaveListInfo);
    connect(btnDeleteLrcList, &BesButton::clicked, this, &PageLyricList::OnDeleteLrcList);

    connect(editLrcItemSongPath, &BesFileLineEdit::sig_filesHaveBeenDrop,
            [=](QList<QString> list){editLrcItemSongPath->setText(list.at(0));});
    connect(editLrcItemLrcPath, &BesFileLineEdit::sig_filesHaveBeenDrop,
            [=](QList<QString> list){editLrcItemLrcPath->setText(list.at(0));});

    //连接完后，默认选中第一项
    lyricListHistory->setCurrentRow(0); //必有一项，默认选中

    enableEditMode(false);              //默认不在编辑模式

    headerListCreated->OnMakeSureHeaderChecking();  //默认列表是展开的
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
                                      SettingManager::GetInstance().data().defaultMusicPath,
                                                      tr("音频 (*.mp3 *.wav);;视频 (*.mp4)"));

    if(fileName.size() !=0)
    {
        editLrcItemSongPath->setText(fileName);
    }
}

void PageLyricList::OnSelectLrcPath()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择歌词"),
                                     SettingManager::GetInstance().data().defaultOutputPath,
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
    historyList.items.insert(0,item);          //这里的更改直接对  listData 生效
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


