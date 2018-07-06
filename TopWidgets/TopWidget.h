#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QWidget>

class TopWidget : public QWidget
{
    Q_OBJECT

public:
    TopWidget(QWidget *parent = 0);
    ~TopWidget();

    void initLayout();
    void connectAll();

private:

};

#endif // TOPWIDGET_H
