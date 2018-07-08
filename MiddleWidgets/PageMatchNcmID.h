#ifndef PAGEMATCHNCMID_H
#define PAGEMATCHNCMID_H

#include <QWidget>

class PageMatchNcmID : public QWidget
{
    Q_OBJECT

public:
    PageMatchNcmID(QWidget *parent = 0);
    ~PageMatchNcmID();

    void initLayout();
    void connectAll();

public:

};

#endif // PAGEMATCHNCMID_H
