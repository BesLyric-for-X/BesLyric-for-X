#ifndef SuUpgrade_H
#define SuUpgrade_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QCheckBox>
#include "BesButton.h"
#include "ISettingUnit.h"
#include "ThreadCheckUpdate.h"

class SuUpgrade: public ISettingUnit
{
    Q_OBJECT
public:
    virtual QString getName() override;
    virtual int getUnitHeight() override;
    virtual QWidget* getUnitWidget(QWidget* parent) override;

public slots:
    void onCheckUpgrade();
    void onShowCheckResult(CheckUpgradeResult result);

public:
    QWidget* SettingUnitContainer;

    QLabel*  labelSettingTitle;

    QCheckBox* checkboxAutoUpgrade;
    BesButton* btnCheckUpgrade;

    QLabel*  labelCurrentVersionTip;
    QLabel*  labelCurrentVersion;

    QLabel*  labelNcmBeslyricAccount;

private:
    ThreadCheckUpdate threadCheck;
};

#endif // SuUpgrade_H
