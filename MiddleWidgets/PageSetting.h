#ifndef PAGESETTING_H
#define PAGESETTING_H

#include <QWidget>

class PageSetting : public QWidget
{
    Q_OBJECT

public:
    PageSetting(QWidget *parent = 0);
    ~PageSetting();

    void initLayout();
    void connectAll();

public:

};

#endif // PAGESETTING_H
