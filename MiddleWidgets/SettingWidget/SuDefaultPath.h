#ifndef SuDefaultPath_H
#define SuDefaultPath_H
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QLineEdit>
#include "BesButton.h"
#include "ISettingUnit.h"

class SuDefaultPath: public ISettingUnit
{
    Q_OBJECT
public:
    virtual QString getName() override;
    virtual int getUnitHeight() override;
    virtual QWidget* getUnitWidget(QWidget* parent) override;

signals:
    void sig_defaultPathLyricChanged(QString path);
    void sig_defaultPathOutputChanged(QString path);

public:
    QWidget* SettingUnitContainer;

    QLabel*  labelSettingTitle;

    BesButton*  btnSelectDefaultPathMusic;
    BesButton*  btnSelectDefaultPathLyric;
    BesButton*  btnSelectDefaultPathOutput;
    QLabel*  labelTipPathMusic;
    QLabel*  labelTipPathLyric;
    QLabel*  labelTipPathOutput;
    QLabel*  labelDefaultPathMusic;
    QLabel*  labelDefaultPathLyric;
    QLabel*  labelDefaultPathOutput;
};

#endif // SuDefaultPath_H
