#ifndef SuDonation_H
#define SuDonation_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include "BesButton.h"
#include "ISettingUnit.h"

class SuDonation: public ISettingUnit
{
    Q_OBJECT
public:
    virtual QString getName() override;
    virtual int getUnitHeight() override;
    virtual QWidget* getUnitWidget(QWidget* parent) override;

public:
    QWidget* SettingUnitContainer;
    QLabel* labelSettingTitle;
};

#endif // SuDonation_H
