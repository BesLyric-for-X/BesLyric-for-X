#ifndef SKIN_BLACK_H
#define SKIN_BLACK_H
#include "ISkin.h"
#include <QString>

class SkinBlack : public ISkin
{
protected:
    virtual void SetParameter()
    {
        fontFamily                 ="Microsoft YaHei                             ";
        topContainerBgImage        =":/resource/image/top_background_black.png";
        topContainerBgColor        ="";
        leftboardBgColor           ="#191b1f                                     ";
        leftboardBorderColor       ="#23262c";
        bottomContainerColor       ="#222222                                     ";
        bottomContainerBorderColor ="rgba(ff, ff, ff, ff)                        ";
        subPageContainerColor      ="#16181c                                     ";

        defautlLabelColor          ="#909090                                     ";
        titleColor                 ="#ffffff                                     ";
        playBgColor                ="#222222                                     ";
        playHoverColor             ="#222222                                     ";
        playBorderColor            ="#666666                                     ";
        playBorderHoverColor       ="#888888                                     ";
        playBorderSize             =1;
        btnSoundImage              =":/resource/image/btn_close_normal_white.png";
        btnSoundImageHover         =":/resource/image/btn_close_press_white.png";
        btnPlayModeImage           =":/resource/image/btn_close_normal_white.png";
        btnPlayModeImageHover      =":/resource/image/btn_close_press_white.png";
        btnLyricImage              =":/resource/image/btn_close_normal_white.png";
        btnLyricImageHover         =":/resource/image/btn_close_press_white.png";
        btnTabLeftBoderColor       ="#bb0033                                     ";

        defaultButtonColorNormal    ="#909090";
        defaultButtonColorHover     ="#f0f0f0";
    }

};

#endif // SKIN_BLACK_H
