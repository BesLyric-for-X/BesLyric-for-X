#ifndef SuLogin_H
#define SuLogin_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include "BesButton.h"
#include "ISettingUnit.h"

class SuLogin: public ISettingUnit
{
public:
    virtual QString getName() override;
    virtual int getUnitHeight() override;
    virtual QWidget* getUnitWidget(QWidget* parent) override;

public:
    QWidget* SettingUnitContainer;

    QLabel*  labelSettingTitle;
    QLabel*  labelShiftBackwardTime;
    QLineEdit*  editShiftBackwardTime;
    BesButton*  btnEditShiftTime;
};

#endif // SuLogin_H
