#ifndef ISettingUnit_H
#define ISettingUnit_H
#include <QWidget>
#include <QObject>

#include <SettingManager.h>  //用于本程序每个SettingUnit都可以访问设置

//每个设置单元的实现接口
//继承该接口的类，可以直接加入 SuScrollPanel 中 getSettingUnits 中，即可实现设置项的拓展
class ISettingUnit: public QObject
{
    Q_OBJECT
public:
    virtual QString getName() = 0;
    virtual int getUnitHeight() = 0;
    virtual QWidget* getUnitWidget(QWidget* parent) = 0;
};

#endif // ISettingUnit
