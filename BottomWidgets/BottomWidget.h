#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include <QWidget>

class BottomWidget : public QWidget
{
    Q_OBJECT

public:
    BottomWidget(QWidget *parent = 0);
    ~BottomWidget();

    void initLayout();
    void connectAll();

private:

};

#endif // BOTTOMWIDGET_H
