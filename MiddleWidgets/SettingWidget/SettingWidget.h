#ifndef SettingWidget_H
#define SettingWidget_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QScrollArea>
#include "BesButton.h"
#include "ISettingUnit.h"
#include "SuLyricMaker.h"
#include "SuDemo2.h"
#include "SettingNavigator.h"
#include "SettingScrollPanel.h"

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    SettingWidget(QWidget *parent = 0);
    ~SettingWidget();

    void initLayout();
    void initConnection();

public:
    QWidget* settingWidgetContainer;

    SuNavigator * settingLeftNavigator;
    SuScrollPanel * settingScrollPanel;
};

#endif // SettingWidget_H
