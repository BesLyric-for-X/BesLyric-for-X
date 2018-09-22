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

        colorLighter = colorString;
        for(auto& ch: colorLighter)   //将所有16进制位在可以自增1的时候自增1，变亮一些
        {
            if(ch == '#')
                continue;

            if(ch == '9')
                ch = 'a';

            if(ch != 'f')
            {
                ch = char(ch.unicode()+1);
            }
        }
    }

    virtual ~SkinPureColor(){}
protected:
    virtual void SetParameter()
    {
        //容器相关
        topContainerBgImage        ="";
        topContainerBgColor        =colorString;
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

        playBgColor                =colorLighter;
        playHoverColor             =colorString;
        playBorderColor            ="#666666                                     ";
        playBorderHoverColor       ="#888888                                     ";
        playBorderSize             =0;
        btnSoundImage              =":/resource/image/btn_close_normal_white.png";
        btnSoundImageHover         =":/resource/image/btn_close_press_white.png";
        btnPlayModeSingleNormal     =":/resource/image/play_mode_single_normal_white.png";
        btnPlayModeSingleHover      =":/resource/image/play_mode_single_hover_white.png";
        btnPlayModeSingleCycleNormal=":/resource/image/play_mode_single_cycle_normal_black.png";
        btnPlayModeSingleCycleHover =":/resource/image/play_mode_single_cycle_hover_black.png";
        btnLyricImage              =":/resource/image/btn_close_normal_white.png";
        btnLyricImageHover         =":/resource/image/btn_close_press_white.png";
        btnTabLeftBoderColor       =colorString;

        btnPackbackToBoxImg       =":/resource/image/btn_pack_back_to_box_white.png";

        //QSlider
        baseColor                 =colorString;
        baseColorLighter          =colorLighter;

        //其他
        editBackground            = "white";
    }


private:
    QString colorString;
    QString colorLighter;
};

#endif // SKIN_PURE_COLOR_H
