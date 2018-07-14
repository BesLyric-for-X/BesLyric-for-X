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

protected:
    virtual void SetParameter()
    {
        //容器相关
        topContainerBgImage        ="";
        topContainerBgColor        =colorString;
        leftboardBgColor           ="#f5f5f7                                     ";
        leftboardBorderColor       ="rgba(33, 33, 33, 33)                        ";
        bottomContainerColor       ="#f6f6f8";
        bottomContainerBorderColor ="rgba(33, 33, 33, 33)                        ";
        subPageContainerColor      ="#ffffff                                     ";

        //label、button 相关
        fontFamily                 ="Microsoft YaHei                             ";
        defautlLabelColor          ="#000000                                     ";
        titleColor                 ="#ffffff                                     ";
        defaultButtonColorNormal    ="#5c5c5c";
        defaultButtonColorHover     ="#000000";

        playBgColor                =colorLighter;
        playHoverColor             =colorString;
        playBorderColor            ="#666666                                     ";
        playBorderHoverColor       ="#888888                                     ";
        playBorderSize             =0;
        btnSoundImage              =":/resource/image/btn_close_normal_white.png";
        btnSoundImageHover         =":/resource/image/btn_close_press_white.png";
        btnPlayModeImage           =":/resource/image/btn_close_normal_white.png";
        btnPlayModeImageHover      =":/resource/image/btn_close_press_white.png";
        btnLyricImage              =":/resource/image/btn_close_normal_white.png";
        btnLyricImageHover         =":/resource/image/btn_close_press_white.png";
        btnTabLeftBoderColor       =colorString;
    }


private:
    QString colorString;
    QString colorLighter;
};

#endif // SKIN_PURE_COLOR_H
