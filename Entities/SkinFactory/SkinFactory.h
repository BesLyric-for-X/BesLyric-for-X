#ifndef SKINFACTORY_H
#define SKINFACTORY_H
#include <QString>
#include <QSharedPointer>
#include "SkinPureColor.h"
#include "SkinRed.h"
#include "SkinBlack.h"

class SkinFacgtory
{
public:
    static QSharedPointer<ISkin> getSkin(QString skinDescritption)
    {
        if(skinDescritption.contains("#"))
            return QSharedPointer<ISkin>(new SkinPureColor(skinDescritption));

        if(skinDescritption == "red")
            return QSharedPointer<ISkin>(new SkinRed);
        else if(skinDescritption =="black")
            return QSharedPointer<ISkin>(new SkinBlack);
        else
            return QSharedPointer<ISkin>(new SkinBlack);
    }
};

#endif // SKINFACTORY_H
