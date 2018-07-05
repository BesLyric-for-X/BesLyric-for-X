#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BesFramelessWidget.h"

class MainWindow : public BesFramelessWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
