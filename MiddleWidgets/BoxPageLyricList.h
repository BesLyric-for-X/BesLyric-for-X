#ifndef BOXPAGELYRICLIST_H
#define BOXPAGELYRICLIST_H

#include <QWidget>

class BoxPageLyricList : public QWidget
{
    Q_OBJECT

public:
    BoxPageLyricList(QWidget *parent = 0);
    ~BoxPageLyricList();

    void initLayout();
    void initConnection();

public:
    QWidget* widgetMainLyricList;

};

#endif // BOXPAGELYRICLIST_H
