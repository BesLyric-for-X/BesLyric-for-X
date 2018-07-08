#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "TopWidget.h"
#include "MiddleWidget.h"
#include "BottomWidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void initLayout();
    void connectAll();

public:
    TopWidget*      topWidget;
    MiddleWidget*   middleWidget;
    BottomWidget*   bottomWidget;
};

#endif // MAINWIDGET_H
