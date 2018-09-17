#ifndef PAGESETTING_H
#define PAGESETTING_H

#include <QWidget>
#include <QLabel>
#include "BesButton.h"
#include "SettingWidget/SettingWidget.h"

class PageSetting : public QWidget
{
    Q_OBJECT

public:
    PageSetting(QWidget *parent = 0);
    ~PageSetting();

    void initLayout();
    void initConnection();

public:
    QWidget* pageSettingContainer;
    QWidget * settingTopPanel;
    QWidget * settingLeftPanel;
    QWidget * settingRightPanel;

    SettingWidget* settingWidget;

    QLabel*   settingTitle;            //设置标题
    BesButton*  btnPackupSetting;      //将设置收起按钮

};

#endif // PAGESETTING_H
