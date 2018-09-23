#ifndef SKIN_PURE_COLOR_H
#define SKIN_PURE_COLOR_H
#include "ISkin.h"
#include <QString>

class SkinPureColor : public ISkin
{
public:
    SkinPureColor(QString colorStr)
    {
        initPureColor(colorStr);
    }

    virtual ~SkinPureColor(){}
protected:
    virtual void SetParameter()
    {
        setWhiteThemeBaseParameter();

        setPureColor();
    }


private:
    QString colorString;
    QString colorLighter;
};

#endif // SKIN_PURE_COLOR_H
