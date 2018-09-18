#ifndef SuLogin_H
#define SuLogin_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QCheckBox>
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

    QCheckBox* checkboxLoginAnonymously;
    QLabel*  linkWhatIsLoginAnonymously;
};

#endif // SuLogin_H
