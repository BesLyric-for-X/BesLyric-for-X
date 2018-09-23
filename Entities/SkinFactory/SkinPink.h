#ifndef SkinPink_H
#define SkinPink_H
#include "ISkin.h"
#include <QString>

class SkinPink : public ISkin
{
public:
    SkinPink()
    {
        QString colorStr = themColorPink;
        initPureColor(colorStr);
    }

    virtual ~SkinPink(){}

protected:
    virtual void SetParameter()
    {
        setWhiteThemeBaseParameter();
        topContainerBgImage = ":/resource/image/top_background_pink.png";
        topContainerBgColor = "";

        setPureColor();
    }

};

#endif // SkinPink_H
