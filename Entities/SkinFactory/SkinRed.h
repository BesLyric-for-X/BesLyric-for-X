#ifndef SKIN_RED_H
#define SKIN_RED_H
#include "ISkin.h"
#include <QString>

class SkinRed : public ISkin
{
public:
    virtual ~SkinRed(){}

protected:
    virtual void SetParameter()
    {
        //容器相关
        topContainerBgImage        ="";
        topContainerBgColor        ="#c62f2f";
        leftboardBgColor           ="#f5f5f7                                     ";
        leftboardBorderColor       ="rgba(33, 33, 33, 33)                        ";
        bottomContainerColor       ="#f6f6f8                                     ";
        bottomContainerBorderColor ="rgba(33, 33, 33, 33)                        ";
        subPageContainerColor      ="rgb(250, 250, 250)        ";

        //label、button 相关
        fontFamily                 ="Microsoft YaHei                             ";
        defautlLabelColor          ="#111111                                     ";
        DeepLabelColor             ="#000000";
        titleColor                 ="#ffffff                                     ";
        defaultButtonColorNormal    ="#202020";
        defaultButtonColorHover     ="#000000";
        defaultButtonBgColorNormal ="rgba(255, 255, 255, 200)";
        defaultButtonBgColorHover ="rgba(240, 240, 240, 200)";
        defaultButtonDisableColor  ="#bbbbbb";
        defaultButtonDisableBgColor="rgba(250, 250, 250, 220)";

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

        btnPackbackToBoxImg       =":/resource/image/btn_pack_back_to_box_white.png";

        //QSlider
        baseColor                 ="#CC0033";
        baseColorLighter          ="#ff3344";
    }

};

#endif // SKIN_RED_H
