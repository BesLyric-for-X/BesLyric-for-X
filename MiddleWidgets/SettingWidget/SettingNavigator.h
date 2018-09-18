#ifndef SuNavigator_H
#define SuNavigator_H

#include <QWidget>
#include "ISettingUnit.h"
#include <QMouseEvent>
#include <QResizeEvent>
#include <QFontMetrics>

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

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

    void resizeEvent(QResizeEvent* event);

public slots:
    void OnSettingUnitPanelPosChanged(int pos, int pageStep, int nScrollMax);  //响应设置面板的滚动，在单元改变时，重画项目高亮

    int getPointInRect(QPoint& pt);                      //获得点在哪个矩形里

signals:
    void sig_scrollToSettingUnit(int index);            //发出项被点击的信号，使设置面板滚动到对应设置单元

private:
    QVector<ISettingUnit*>* pSettings;
    QVector<QRect> vecRect;

    int nCurrentIndex;


    int nRight;
    int nTop;
    int nStep;
    int nr;
    int nR;
    int textHeight;

    QFontMetrics* fontMetrics;
};

#endif // SuNavigator_H
