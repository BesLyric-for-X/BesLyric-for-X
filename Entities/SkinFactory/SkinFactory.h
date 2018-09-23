#ifndef SKINFACTORY_H
#define SKINFACTORY_H
#include <QString>
#include <QSharedPointer>
#include "SkinPureColor.h"
#include "SkinRed.h"
#include "SkinBlack.h"
#include "SkinGreen.h"
#include "SkinPink.h"
#include "SkinGold.h"
#include "SkinBlue.h"

class SkinFacgtory
{
public:
    static QSharedPointer<ISkin> getSkin(QString skinDescritption)
    {
        if(skinDescritption.contains("#"))
            return QSharedPointer<ISkin>(new SkinPureColor(skinDescritption));

        if(skinDescritption == "black")
            return QSharedPointer<ISkin>(new SkinBlack);
        else if(skinDescritption =="red")
            return QSharedPointer<ISkin>(new SkinRed);
        else if(skinDescritption =="pink")
            return QSharedPointer<ISkin>(new SkinPink);
        else if(skinDescritption =="blue")
            return QSharedPointer<ISkin>(new SkinBlue);
        else if(skinDescritption =="green")
            return QSharedPointer<ISkin>(new SkinGreen);
        else if(skinDescritption =="gold")
            return QSharedPointer<ISkin>(new SkinGold);
        else
            return QSharedPointer<ISkin>(new SkinBlack);
    }
};

#endif // SKINFACTORY_H
