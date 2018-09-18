#ifndef SuSoftware_H
#define SuSoftware_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include "BesButton.h"
#include "ISettingUnit.h"

class SuSoftware: public ISettingUnit
{
public:
    virtual QString getName() override;
    virtual int getUnitHeight() override;
    virtual QWidget* getUnitWidget(QWidget* parent) override;

public:
    QWidget* SettingUnitContainer;
    QLabel* labelSettingTitle;
};

#endif // SuSoftware_H
