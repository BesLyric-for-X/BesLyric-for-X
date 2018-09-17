#ifndef SuNavigator_H
#define SuNavigator_H

#include <QWidget>
#include "ISettingUnit.h"

class SuNavigator : public QWidget
{
    Q_OBJECT

public:
    SuNavigator(QVector<ISettingUnit*>& settingUnits, QWidget *parent = 0);
    ~SuNavigator();

    void initEntity();
    void initLayout();
    void initConnection();

    virtual void paintEvent(QPaintEvent* event);

public slots:
    void OnSettingUnitPanelPosChanged(int pos);         //响应设置面板的滚动，在单元改变时，重画项目高亮

signals:
    void sig_scrollToSettingUnit(int index);            //发出项被点击的信号，使设置面板滚动到对应设置单元

private:
    QVector<ISettingUnit*>* pSettings;
};

#endif // SuNavigator_H
