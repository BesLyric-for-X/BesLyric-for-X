#ifndef PAGELYRICLIST_H
#define PAGELYRICLIST_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include "list/BesListHeader.h"

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

    QListWidget *lyricListHistory;
    BesListHeader* headerListCreated;
    QListWidget *lyricListCreated;
    BesListHeader* headerListTest;
    QListWidget *lyricListTest;
};

#endif // PAGELYRICLIST_H
