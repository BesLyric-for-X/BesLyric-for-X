#ifndef SkinBlue_H
#define SkinBlue_H
#include "ISkin.h"
#include <QString>

class SkinBlue : public ISkin
{
public:
    SkinBlue()
    {
        QString colorStr = themColorBlue;
        initPureColor(colorStr);
    }

    virtual ~SkinBlue(){}

protected:
    virtual void SetParameter()
    {
        setWhiteThemeBaseParameter();
        topContainerBgImage = ":/resource/image/top_background_blue.png";

        setPureColor();
    }

};

#endif // SkinBlue_H
