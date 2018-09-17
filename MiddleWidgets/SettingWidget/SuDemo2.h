#ifndef SuDemo2_H
#define SuDemo2_H
#include <QWidget>
#include <QObject>
#include "ISettingUnit.h"

class SuDemo2: public ISettingUnit
{
public:
    virtual QString getName() override;
    virtual int getUnitHeight() override;
    virtual QWidget* getUnitWidget(QWidget* parent) override;

public:
    QWidget* SettingUnitContainer;
};

#endif // SuDemo2_H
