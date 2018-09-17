#ifndef SuScrollPanel_H
#define SuScrollPanel_H

#include <QVector>
#include <QScrollArea>
#include "ISettingUnit.h"
#include "SuDemo1.h"
#include "SuDemo2.h"

class SuScrollPanel : public QScrollArea
{
    Q_OBJECT

public:
    SuScrollPanel(QWidget *parent = 0);
    ~SuScrollPanel();

    //在未初始化时，初始化 QVector<ISettingUnit*> settings，并返回对应的引用
    QVector<ISettingUnit*>& getSettingUnits();

public slots:
    void OnScrollToIndex(int index);        //如果存在滚动条，滚动到指定目标设置单元的位置

signals:
    void sig_scrollPosChanged(int pos);            //在滚动条位置发生变化时，发出当前的位置

private:

    void initLayout();
    void initConnection();

public:
    QWidget * settingUnitContainer;
    QScrollArea * scrollAreaSetting;

private:

    QVector<ISettingUnit*> settings;

    int nScrollMin;
    int nScrollMax;
    int nPageStep;
};

#endif // SuScrollPanel_H
