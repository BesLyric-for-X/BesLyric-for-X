#ifndef ISKIN_H
#define ISKIN_H
#include <QString>

class ISkin
{
public:
    QString GetCssString()
    {
        setDefaultParameter();

        SetParameter();

        QString cssContent;

        cssContent += GetQWidgetCss();
        cssContent += GetQLabelCss();
        cssContent += GetQPushButtonCss();
        cssContent += GetQSliderCss();

        return cssContent;
    }

protected:
    virtual void SetParameter() = 0;


private:

    QString GetQWidgetCss()
   {
        QString str =
        "QWidget{"
            "font-family:"+fontFamily +";"
             "padding:0px;"
             "margin: 0px;"
        "}"
        "QWidget#topWidgetContainer"
        "{"
           " border-image:url(\""+topContainerBgImage+"\");"
            "background-color:"+topContainerBgColor+";"
        "}"
        "QWidget#leftBoardMainPage"
        "{"
            "background-color:"+ leftboardBgColor+";"
            "border-right: 1px solid "+ leftboardBorderColor +";"
        "}"
        "QWidget#bottomWidgetContainer"
        "{"
            "background-color:"+ bottomContainerColor +";"
            "border-top: 1px solid "+bottomContainerBorderColor +";"
        "}"

        "QWidget#subPageContainer"
        "{"
           " background-color:"+subPageContainerColor+ ";"
        "}"

//        "QWidget#skinBoxMainLayer"
//        "{"
//           " border-radius:5px;"
//           " background-color:"+PopWindowBgColor+ ";"
//        "}"

        ;

        return str;
   }

    QString GetQLabelCss()
    {
        QString str =
        "QLabel{"
           " color:"+defautlLabelColor+";"
        "}"

        "QLabel#labelTitle{"
            "color:"+titleColor+";"
            "font-size:16px;"
        "}"
        "QLabel#labelSepHeader{"
            "color:rgba(88, 88, 88, 88);"
            "font-size:20px;"
        "}"
         ;
        return str;
    }


    QString GetQPushButtonCss()
    {
        QString str =

"               QPushButton{"
"                   border-style: solid;"
"                   border: 2px;"
"                   color:"+defaultButtonColorNormal+";"
"                   padding: 3px;"
"                   max-height: 120px;"
"                   border-radius:5px;"
"                   background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #ff3344, stop:1 #CC0033);"
"               }"

"               QPushButton:hover{"
"                   color: "+defaultButtonColorHover+";"
"                   background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #bb0033, stop:1 #aa0033);"
"               }"

"               QPushButton:checked{"
"                   color: "+defaultButtonColorHover+";"
"                   background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #bb0033, stop:1 #aa0033);"
"               }"

"               QPushButton:pressed{"
"                   color: "+defaultButtonColorHover+";"
"                   background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #ff3344, stop:1 #CC0033);"
"               }"

               /* 标题栏上的按钮样式 */
"               QPushButton#btnIcon"
"               {"
"                   border: 0px; border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px; margin: 0px;"
"                   width: 120px; height:30px;"
"                   background-image:url(\":/resource/image/logo.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSkinBox{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_skin_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSkinBox:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_skin_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSetting{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_setting_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSetting:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_setting_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnMini{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_mini_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnMini:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_mini_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnMax{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_max_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnMax:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_max_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnRestore{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_restore_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnRestore:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_restore_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnClose{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_close_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnClose:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_close_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
               /*3个播放按钮*/
"               QPushButton#btnPreSong{"
"                   width: 32px;"
"                   height:32px;"
"                   border-radius: "+QString::number(playBorderSize + 16)+"px;"
"                   border:"+QString::number(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playBgColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderColor+";"
"                   background-image:url(\":/resource/image/btn_pre_song.png\");"
"                   background-repeat:no-repeat;"
"               }"
"                QPushButton#btnNextSong{"
"                   width: 32px;"
"                   height:32px;"
"                   border-radius: "+QString::number(playBorderSize + 16)+"px;"
"                   border:"+QString::number(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playBgColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderColor+";"
"                   background-image:url(\":/resource/image/btn_next_song.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPlayAndPause{"
"                   width: 40px;"
"                   height:40px;"
"                   border-radius: "+QString::number(playBorderSize + 20)+"px;"
"                   border:"+QString::number(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playBgColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderColor+";"
"                   background-image:url(\":/resource/image/btn_pause.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPreSong:hover{"
"                   width: 32px;"
"                   height:32px;"
"                   border-radius: "+QString::number(playBorderSize + 16)+"px;"
"                   border:"+QString::number(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playHoverColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderHoverColor+";"
"                   background-image:url(\":/resource/image/btn_pre_song.png\");"
"                   background-repeat:no-repeat;"
"               }"
"                QPushButton#btnNextSong:hover{"
"                   width: 32px;"
"                   height:32px;"
"                   border-radius: "+QString::number(playBorderSize + 16)+"px;"
"                   border:"+QString::number(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playHoverColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderHoverColor+";"
"                   background-image:url(\":/resource/image/btn_next_song.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPlayAndPause:hover{"
"                   width: 40px;"
"                   height:40px;"
"                   border-radius: "+QString::number(playBorderSize + 20)+"px;"
"                   border:"+QString::number(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playHoverColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderHoverColor+";"
"                   background-image:url(\":/resource/image/btn_pause.png\");"
"                   background-repeat:no-repeat;"
"               }"
               /* 播放栏其他按钮 */
"               QPushButton#btnSound{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\""+btnSoundImage+"\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSound:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\""+btnSoundImageHover+"\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPlayMode{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\""+btnPlayModeImage+"\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPlayMode:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\""+btnPlayModeImageHover+"\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnDesktopLyric{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\""+btnLyricImage+"\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnDesktopLyric:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\""+btnLyricImageHover+"\");"
"                   background-repeat:no-repeat;"
"               }"
               /* 页面左侧按钮 */
"               QPushButton#btnMakingLyric, QPushButton#btnDownloadSong, QPushButton#btnDownloadLyric{"
"                   background-color:rgba(0, 0, 0, 0);"
"                   border:0px;"
"                   width: 180px;"
"                   height:40px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   border-radius: 0px;"
"                   border-left: 5px solid rgba(0, 0, 0, 0);"
"                   border-style:solid;"
"               }"
"               QPushButton#btnMakingLyric:hover, QPushButton#btnDownloadSong:hover, QPushButton#btnDownloadLyric:hover{"
"                   background-color:rgba(0, 0, 0, 0);"
"                   border:0px;"
"                   width: 180px;"
"                   height:40px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   border-radius: 0px;"
"                   border-left: 5px solid rgba(0, 0, 0, 0);"
"                   border-style:solid;"
"               }"
"               QPushButton#btnMakingLyric:checked, QPushButton#btnDownloadSong:checked, QPushButton#btnDownloadLyric:checked{"
"                   background-color:rgba(255, 255, 255, 22);"
"                   border:0px;"
"                   width: 180px;"
"                   height:40px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   border-radius: 0px;"
"                   border-left: 5px solid "+btnTabLeftBoderColor+";"
"                   border-style:solid;"
"               }"

                /* 皮肤盒中的按钮 */
"               QPushButton#btnTheme, QPushButton#btnPureColor{"
"                   background-color:rgba(0, 0, 0, 0);"
"                   border:0px;"
                    "border-bottom:2px;"
                    "border-color:rgba(0, 0, 0, 0);"
"                   padding:0px;"
"                   padding-bottom:2px;"
"                   margin-top: 15px;"
"                   margin-left: 15px;"
"                   border-radius: 0px;"
"                   border-style:solid;"
"               }"
"                   QPushButton#btnTheme:hover, QPushButton#btnPureColor:hover{"
"                   border:0px;"
                    "border-bottom:2px;"
                    "border-color:rgba(0, 0, 0, 0);"
"                   padding:0px;"
"                   padding-bottom:2px;"
"                   margin-top: 15px;"
"                   margin-left: 15px;"
"                   border-radius: 0px;"
"                   border-style:solid;"
"               }"
"               QPushButton#btnTheme:checked, QPushButton#btnPureColor:checked{"
"                   border:0px;"
                    "border-bottom:2px;"
                    "border-color:"+defaultButtonColorNormal+";"
"                   padding:0px;"
"                   padding-bottom:2px;"
"                   margin-top: 15px;"
"                   margin-left: 15px;"
"                   border-radius: 0px;"
"                   border-style:solid;"
"               }"
                "QPushButton#btnCustomizeColor{"
                "border-radius:0px;"
                "margin:2px;"
                "border-image:url(\":/resource/image/btn_customize_color.png\");"
                 "}"
              ;

        return str;
    }

    QString GetQSliderCss()
    {
        QString str =
                /* 滑块器样式 */

                /* groove 滑块器槽 */
"                QSlider#sliderSong::groove:horizontal,QSlider#sliderSong::add-page:horizontal {"
"                    background: #cccccc;"
"                    height: 6px;"
"                    border-radius: 2px;"
"                }"
                /* sub-page 滑块器有值部分 sliderSong */
"                QSlider#sliderSong::sub-page:horizontal {"
"                    height: 6px;"
"                    border-radius: 2px;"
"                    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #ff3344, stop:1 #CC0033);"
"                }"
                /* 拖动块部分 */
"                QSlider#sliderSong::handle:horizontal {"
"                    width: 14px;"
"                    margin-top: -5px;"
"                    margin-bottom: -5px;"
"                    border-radius: 7px;"
"                    border: 1px solid #cccccc;"
"                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.25 #CC0033, stop:0.4 #fefefe);"
"                }"
"                QSlider#sliderSong::handle:horizontal:hover {"
"                    border: 1px solid #aaaaaa;"
"                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.3 #CC0033, stop:0.45 #fefefe);"
"                }"
                /* 滑块器样式 */
                /* groove 滑块器槽 */
"                QSlider#sliderSound::groove:horizontal,QSlider#sliderSound::add-page:horizontal {"
"                    background: #cccccc;"
"                    height: 4px;"
"                    border-radius: 1px;"
"                }"
                /* sub-page 滑块器有值部分 */
"                QSlider#sliderSound::sub-page:horizontal {"
"                    height: 4px;"
"                    border-radius: 1px;"
"                    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #ff3344, stop:1 #CC0033);"
"                }"
                /* 拖动块部分 */
"                QSlider#sliderSound::handle:horizontal {"
"                    width: 10px;"
"                    margin-top: -3px;"
"                    margin-bottom: -3px;"
"                    border-radius: 5px;"
"                    border: 1px solid #cccccc;"
"                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.25 #CC0033, stop:0.4 #fefefe);"
"                }"
"                QSlider#sliderSound::handle:horizontal:hover {"
"                    border: 1px solid #aaaaaa;"
"                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.3 #CC0033, stop:0.45 #fefefe);"
"                }"
               ;

        return str;
    }


    void setDefaultParameter()
    {
        fontFamily                 ="Microsoft YaHei                             ";
        topContainerBgImage        =":/resource/image/top_background_black.png";
        topContainerBgColor        ="";
        leftboardBgColor           ="#191b1f                                     ";
        leftboardBorderColor       ="#23262c";
        bottomContainerColor       ="#222222                                     ";
        bottomContainerBorderColor ="rgba(ff, ff, ff, ff)                        ";
        subPageContainerColor      ="#16181c                                     ";
        PopWindowBgColor           ="#2d2f33                                     ";

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

protected:
                                        //默认值参考
    QString fontFamily;                 //Microsoft YaHei
    QString topContainerBgImage;        //:/resource/image/top_background_black.png
    QString topContainerBgColor;        //#c62f2f
    QString leftboardBgColor;           //#191b1f
    QString leftboardBorderColor;       //rgba(ff, ff, ff, 33)
    QString bottomContainerColor;       //#222222
    QString bottomContainerBorderColor; //rgba(ff, ff, ff, ff)
    QString subPageContainerColor;      //#16181c
    QString PopWindowBgColor;           //#343638

    QString defautlLabelColor;          //#909090
    QString titleColor;                 //#ffffff
    QString playBgColor;                //#222222
    QString playHoverColor;             //#222222
    QString playBorderColor;            //#666666
    QString playBorderHoverColor;       //#888888
    int     playBorderSize;             //1px;
    QString btnSoundImage;              //:/resource/image/btn_close_normal_white.png
    QString btnSoundImageHover;         //:/resource/image/btn_close_press_white.png
    QString btnPlayModeImage;           //:/resource/image/btn_close_normal_white.png
    QString btnPlayModeImageHover;      //:/resource/image/btn_close_press_white.png
    QString btnLyricImage;              //:/resource/image/btn_close_normal_white.png
    QString btnLyricImageHover;         //:/resource/image/btn_close_press_white.png
    QString btnTabLeftBoderColor;       //#bb0033

    QString defaultButtonColorNormal;   //#909090
    QString defaultButtonColorHover;    //#f0f0f0

};

#endif // ISKIN_H
