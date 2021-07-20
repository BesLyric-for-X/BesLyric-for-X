#ifndef PAGELYRICLIST_H
#define PAGELYRICLIST_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QScrollArea>
#include "list/BesListHeader.h"
#include "list/BesList.h"
#include "LyricListManager.h"
#include "table/BesLListTableView.h"
#include "BesFileLineEdit.h"
#include "BoxPagePreviewLyric.h"

class PageLyricList : public QWidget
{
    Q_OBJECT

public:
    PageLyricList(QWidget *parent = 0);
    ~PageLyricList();

    void initEntity();
    void initLayout();
    void initConnection();

public slots:

    //左侧歌词单操作
    void OnAddNewListItem(QString itemName);
    bool OnDeleteCurrentItem(bool bDeleteConformRequested);

    //右侧编辑歌词单相关
    void OnSelectSongPath();
    void OnSelectLrcPath();
    void OnSaveLrcListItem();
    void OnCreateLrcListItem();

    void OnDeleteListItem(int row);
    void OnEditListItem(int row);

    void OnSaveListInfo();
    void OnDeleteLrcList();

    //凡是歌词单数据发生改变，都调用此保存数据
    void OnSaveLyricListData();

    //用于外部添加到历史记录
    void OnAddToMakingHistory(QString musicPath, QString lrcPath);

private:
    void reloadLyricListData(LyricList* pLyricListData);

    void enableEditMode(bool bEnable, int indexWhenEnable = -1);


public:
    QWidget * pageLyricListContainer;
    QWidget * lyriclistLeftPanel;
    QWidget * lyriclistRightPanel;

    //左侧控件
    BesList *lyricListHistory;
    BesListHeader* headerListCreated;
    BesList *lyricListCreated;
    QWidget * lyricListContainer;

    QScrollArea* scrollAreaLeft;

    BoxPagePreviewLyric*    boxPagePreviewLyric;

    //右侧控件
    QLabel *labelListCoverRect;

    QLabel *labelLyricListRedMark;
    QLabel *labelListInfoTitle;
    BesButton*  btnPackupLyricList;      //将设置收起按钮
    QWidget* widgetListInfoRight;

    QTabWidget* tabpageLyricList;

    BesLListTableView* tableLrcList;
    QWidget* widgetEditLyricItem;
    QWidget* widgetEditListInfo;


    //编辑歌单项
    QLabel* labelLrcItemSongPath;
    QLabel* labelLrcItemLrcPath;
    BesFileLineEdit* editLrcItemSongPath;
    BesFileLineEdit* editLrcItemLrcPath;
    BesButton* btnSelectLrcItemSongPath;
    BesButton* btnSelectLrcItemLrcPath;

    BesButton* btnCreateLrcItem;
    BesButton* btnSaveLrcItem;

    //编辑歌词单信息
    QLabel* labelModifyLrcListName;
    QLineEdit* editModifyLrcListName;
    QLabel* labelModifyListCoverRect;
    BesButton* btnModifyListCover;

    BesButton* btnDeleteLrcList;
    BesButton* btnSaveLrcListModified;

public:

    LyricListData listData;

    LyricList* pCurrentLyricList;
    BesList *lyricListCurrent;          //当前选中的列

    bool bEnableEditMode;
    int currentEditItem;
};

#endif // PAGELYRICLIST_H
