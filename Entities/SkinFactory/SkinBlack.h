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
        //容器相关
        topContainerBgImage        =":/resource/image/top_background_black.png";
        topContainerBgColor        ="";
        leftboardBgColor           ="#191b1f                                     ";
        leftboardBorderColor       ="#23262c";
        bottomContainerColor       ="#222222                                     ";
        bottomContainerBorderColor ="rgba(ff, ff, ff, ff)                        ";
        subPageContainerColor      ="#16181c                                     ";

        //label、button 相关
        fontFamily                 ="Microsoft YaHei                             ";
        defautlLabelColor          ="#909090                                     ";
        DeepLabelColor             ="#ffffff";
        titleColor                 ="#ffffff                                     ";
        defaultButtonColorNormal    ="#e0e0e0";
        defaultButtonColorHover     ="#ffffff";
        defaultButtonBgColorNormal ="rgba(100, 100, 100, 100)";
        defaultButtonBgColorHover ="rgba(100, 100, 100, 150)";
        defaultButtonDisableColor  ="#454545";
        defaultButtonDisableBgColor="#222222";

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

        btnPackbackToBoxImg       =":/resource/image/btn_pack_back_to_box_black.png";
    }

};

#endif // SKIN_BLACK_H
