#ifndef SKIN_BLACK_H
#define SKIN_BLACK_H
#include "ISkin.h"
#include <QString>

class SkinBlack : public ISkin
{
public:
    virtual ~SkinBlack(){}

protected:
    virtual void SetParameter()
    {
       setBlackThemeBaseParameter();
    }

};

#endif // SKIN_BLACK_H
