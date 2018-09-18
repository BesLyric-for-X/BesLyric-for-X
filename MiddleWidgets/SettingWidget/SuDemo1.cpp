#include <global.h>
#include "SuDemo1.h"

QString SuDemo1::getName()
{
   return tr("歌词制作");
}

int SuDemo1::getUnitHeight()
{
   return 200;
}

QWidget *SuDemo1::getUnitWidget(QWidget *parent)
{
    SettingUnitContainer = new QWidget(parent);
    SettingUnitContainer->setObjectName("SettingUnitContainer");

    return SettingUnitContainer;
}


