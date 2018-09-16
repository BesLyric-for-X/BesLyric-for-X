#ifndef PAGELYRICLIST_H
#define PAGELYRICLIST_H

#include <QWidget>
#include <QListWidgetItem>
#include <QScrollArea>
#include "list/BesListHeader.h"
#include "list/BesList.h"
#include "LyricListManager.h"

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
    void OnRowsMoved( const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row, QPrivateSignal);

    void OnAddNewListItem(QString itemName);
    void OnDeleteCurrentItem();

    void OnSaveLyricListData();

public:
    QWidget* pageLyricListContainer;
    QWidget * lyriclistLeftPanel;
    QWidget * lyriclistRightPanel;

    BesList *lyricListHistory;
    BesListHeader* headerListCreated;
    BesList *lyricListCreated;

    BesListHeader* headerListTest;
    BesList *lyricListTest;

    QScrollArea* scrollAreaLeft;

public:
    LyricListData listData;

    QList<QString> strList;
};

#endif // PAGELYRICLIST_H
