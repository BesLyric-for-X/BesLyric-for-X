#include "SuDemo2.h"

QString SuDemo2::getName()
{
   return tr("demo2");
}

int SuDemo2::getUnitHeight()
{
   return 300;
}

QWidget *SuDemo2::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer2");

    return SettingUnitContainer;
}
