#ifndef SuUpgrade_H
#define SuUpgrade_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QCheckBox>
#include "BesButton.h"
#include "ISettingUnit.h"

class SuUpgrade: public ISettingUnit
{
public:
    virtual QString getName() override;
    virtual int getUnitHeight() override;
    virtual QWidget* getUnitWidget(QWidget* parent) override;

public:
    QWidget* SettingUnitContainer;

    QLabel*  labelSettingTitle;

    QCheckBox* checkboxAutoUpgrade;
    BesButton* btnCheckUpgrade;

    QLabel*  labelCurrentVersionTip;
    QLabel*  labelCurrentVersion;
};

#endif // SuUpgrade_H
