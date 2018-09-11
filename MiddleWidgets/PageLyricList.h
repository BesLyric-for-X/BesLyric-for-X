#ifndef PAGELYRICLIST_H
#define PAGELYRICLIST_H

#include <QWidget>
#include <QListWidgetItem>
#include <QScrollArea>
#include "list/BesListHeader.h"
#include "list/BesList.h"

class PageLyricList : public QWidget
{
    Q_OBJECT

public:
    PageLyricList(QWidget *parent = 0);
    ~PageLyricList();

    void initLayout();
    void initConnection();

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
};

#endif // PAGELYRICLIST_H
