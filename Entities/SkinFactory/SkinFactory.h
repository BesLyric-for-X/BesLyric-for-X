#ifndef SKINFACTORY_H
#define SKINFACTORY_H
#include <QString>
#include <QSharedPointer>
#include "SkinRed.h"
#include "SkinBlack.h"

class SkinFacgtory
{
public:
    static QSharedPointer<ISkin> getSkin(QString skinDescritption)
    {
        if(skinDescritption == "red")
            return QSharedPointer<ISkin>(new SkinRed);
        else
            return QSharedPointer<ISkin>(new SkinBlack);

    }
};

#endif // SKINFACTORY_H
