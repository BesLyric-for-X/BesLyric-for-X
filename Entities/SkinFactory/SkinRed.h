#ifndef SKIN_RED_H
#define SKIN_RED_H
#include "ISkin.h"
#include <QString>

class SkinRed : public ISkin
{
public:
    virtual ~SkinRed(){}

protected:
    virtual void SetParameter()
    {
        setWhiteThemeBaseParameter();
    }

};

#endif // SKIN_RED_H
