#ifndef SKIN_PURE_COLOR_H
#define SKIN_PURE_COLOR_H
#include "ISkin.h"
#include <QString>

class SkinPureColor : public ISkin
{
public:
    SkinPureColor(QString colorStr)
    {
        colorString = colorStr;
    }

protected:
    virtual void SetParameter()
    {
        fontFamily                 ="Microsoft YaHei                             ";
        topContainerBgImage        ="";
        topContainerBgColor        =colorString;
        leftboardBgColor           ="#f5f5f7                                     ";
        leftboardBorderColor       ="rgba(33, 33, 33, 33)                        ";
        bottomContainerColor       =colorString;
        bottomContainerBorderColor ="rgba(33, 33, 33, 33)                        ";
        subPageContainerColor      ="#ffffff                                     ";
        defautlLabelColor          ="#000000                                     ";
        titleColor                 ="#ffffff                                     ";
        playBgColor                ="#e83c3c                                     ";
        playHoverColor             ="#c62f2f                                     ";
        playBorderColor            ="#666666                                     ";
        playBorderHoverColor       ="#888888                                     ";
        playBorderSize             =0;
        btnSoundImage              =":/resource/image/btn_close_normal_white.png";
        btnSoundImageHover         =":/resource/image/btn_close_press_white.png";
        btnPlayModeImage           =":/resource/image/btn_close_normal_white.png";
        btnPlayModeImageHover      =":/resource/image/btn_close_press_white.png";
        btnLyricImage              =":/resource/image/btn_close_normal_white.png";
        btnLyricImageHover         =":/resource/image/btn_close_press_white.png";
        btnTabLeftBoderColor       ="#bb0033                                     ";
    }


private:
    QString colorString;
    QString colorLighter;
};

#endif // SKIN_PURE_COLOR_H
