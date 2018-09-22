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

        btnSoundImage              =":/resource/image/sound_normal_black.png";
        btnSoundImageHover         =":/resource/image/sound_hover_black.png";
        btnSoundMuteImage          =":/resource/image/sound_mute_normal_black.png";
        btnSoundMuteImageHover     =":/resource/image/sound_mute_hover_black.png";
        btnPlayModeSingleNormal     =":/resource/image/play_mode_single_normal_white.png";
        btnPlayModeSingleHover      =":/resource/image/play_mode_single_hover_white.png";
        btnPlayModeSingleCycleNormal=":/resource/image/play_mode_single_cycle_normal_black.png";
        btnPlayModeSingleCycleHover =":/resource/image/play_mode_single_cycle_hover_black.png";

        btnLyricImage              =":/resource/image/btn_close_normal_white.png";
        btnLyricImageHover         =":/resource/image/btn_close_press_white.png";
        btnTabLeftBoderColor       ="#bb0033                                     ";

        btnPackbackToBoxImg       =":/resource/image/btn_pack_back_to_box_black.png";

        //QList
        listItemNormalColor        ="#c0c0c0";
    }

};

#endif // SKIN_BLACK_H
