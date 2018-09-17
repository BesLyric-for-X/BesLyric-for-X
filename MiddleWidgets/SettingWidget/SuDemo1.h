#ifndef SuDemo1_H
#define SuDemo1_H
#include <QWidget>
#include <QObject>
#include "ISettingUnit.h"

class SuDemo1: public ISettingUnit
{
public:
    virtual QString getName() override;
    virtual int getUnitHeight() override;
    virtual QWidget* getUnitWidget(QWidget* parent) override;

public:
    QWidget* SettingUnitContainer;
};

#endif // SuDemo1_H
