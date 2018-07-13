#ifndef SKIN_RED_H
#define SKIN_RED_H
#include "ISkin.h"
#include <QString>

class SkinRed : public ISkin
{
protected:
    virtual void SetParameter()
    {
        fontFamily                 ="Microsoft YaHei                             ";
        topContainerBgImage        ="";
        topContainerBgColor        ="#c62f2f";
        leftboardBgColor           ="#f5f5f7                                     ";
        leftboardBorderColor       ="rgba(33, 33, 33, 33)                        ";
        bottomContainerColor       ="#f6f6f8                                     ";
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

};

#endif // SKIN_RED_H
