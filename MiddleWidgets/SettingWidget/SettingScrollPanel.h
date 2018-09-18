#ifndef SuScrollPanel_H
#define SuScrollPanel_H

#include <QVector>
#include <QScrollArea>
#include <QTimer>
#include "ISettingUnit.h"
#include "SuLyricMaker.h"
#include "SuDefaultPath.h"
#include "SuMusicDownload.h"
#include "SuLogin.h"
#include "SuUpgrade.h"
#include "SuSoftware.h"
#include "SuDonation.h"

class SuScrollPanel : public QWidget
{
    Q_OBJECT

public:
    SuScrollPanel(QWidget *parent = 0);
    ~SuScrollPanel();

    //在未初始化时，初始化 QVector<ISettingUnit*> settings，并返回对应的引用
    QVector<ISettingUnit*>& getSettingUnits();

public slots:
    void OnScrollToIndex(int index);        //如果存在滚动条，滚动到指定目标设置单元的位置
    void OnSrcollTimerTimeout();

signals:
    void sig_scrollPosChanged(int pos, int pageStep, int nScrollMax);//在滚动条位置发生变化时，发出当前的位置


private:
    void initEntity();
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

    QTimer*             scrollTimer;        //滚动用的定时器
    int                 nTargetPos;         //目标位置
};

#endif // SuScrollPanel_H
