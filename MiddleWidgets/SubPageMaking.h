#ifndef SUBPAGEMAKING_H
#define SUBPAGEMAKING_H

#include <QWidget>

class SubPageMaking : public QWidget
{
    Q_OBJECT

public:
    SubPageMaking(QWidget *parent = 0);
    ~SubPageMaking();

    void initLayout();
    void connectAll();

private:

};

#endif // SUBPAGEMAKING_H
