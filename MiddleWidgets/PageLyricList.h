#ifndef PAGELYRICLIST_H
#define PAGELYRICLIST_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>

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

    QListWidget *LyricListCreated;
};

#endif // PAGELYRICLIST_H
