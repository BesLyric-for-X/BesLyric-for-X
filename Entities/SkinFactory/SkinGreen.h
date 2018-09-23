#ifndef SkinGreen_H
#define SkinGreen_H
#include "ISkin.h"
#include <QString>

class SkinGreen : public ISkin
{
public:
    SkinGreen()
    {
        QString colorStr = themColorGreen;
        initPureColor(colorStr);
    }

    virtual ~SkinGreen(){}

protected:
    virtual void SetParameter()
    {
        setWhiteThemeBaseParameter();
        topContainerBgImage = ":/resource/image/top_background_green.png";

        setPureColor();
    }

};

#endif // SkinGreen_H
