#ifndef SkinGold_H
#define SkinGold_H
#include "ISkin.h"
#include <QString>

class SkinGold : public ISkin
{
public:
    SkinGold()
    {
        QString colorStr = themColorGold;
        initPureColor(colorStr);
    }

    virtual ~SkinGold(){}

protected:
    virtual void SetParameter()
    {
        setWhiteThemeBaseParameter();
        topContainerBgImage = ":/resource/image/top_background_gold.png";

        setPureColor();
    }

};

#endif // SkinGold_H
