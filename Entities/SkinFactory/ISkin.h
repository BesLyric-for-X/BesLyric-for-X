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
        cssContent += GetQEditCss();
        cssContent += GetQMenuCss();
        cssContent += GetQScrollBarCss();
        cssContent += GetQTabWidgetCss();
        cssContent += GetQTableWidgetCss();

        return cssContent;
    }

    virtual ~ISkin(){}

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
        "QWidget#leftBoardMainPage, QWidget#lyriclistLeftPanel"
        "{"
            "background-color:"+ leftboardBgColor+";"
            "border-right: 1px solid "+ leftboardBorderColor +";"
        "}"
        "QWidget#bottomWidgetContainer"
        "{"
            "background-color:"+ bottomContainerColor +";"
            "border-top: 1px solid "+bottomContainerBorderColor +";"
        "}"

        "QWidget#subPageContainer, QWidget#pageSettingContainer, QWidget#pageLyricListContainer"
        "{"
           " background-color:"+subPageContainerColor+ ";"
        "}"

        "QWidget#widgetMainPreviewLyric, QWidget#widgetMainLyricList"
        "{"
            "border:1px solid "+leftboardBorderColor+";"
                "border-bottom:0px;"
        "}"
        "QWidget#scrollLyricPanel"
        "{"
             "margin-right:10px;"
             "border-right: 1px solid rgba(100,100,100,25%);"
        "}"
        "QWidget#widgetLyricBoard"
        "{"
            "background-color:rgba(150,150,150,2%);"
             "border: 1px solid rgba(150,150,150,15%);"
        "}"

        "QScrollArea#ScrollLyricPanelScrollArea"
        "{"
            "background-color:rgba(0,0,0,0%);"
        "}"

        "QWidget#widgetMessageBoxContainer"
        "{"
           "border: 1px solid #3b3a3d;"
        "}"

        "QWidget#widgetMessageTop"
        "{"
           " background-color:"+MessageBoxBgColor+ ";"
           "border-bottom: 1px solid #3b3a3d;"
        "}"

        "QWidget#widgetMessageMiddle"
        "{"
           " background-color:"+MessageBoxBgColor+ ";"
           "border-bottom: 1px solid #3b3a3d;"
        "}"

        "QWidget#widgetMessageBottom"
        "{"
           " background-color:"+MessageBoxBottomColor+ ";"
        "}"

        "QWidget#settingTopPanel{"
           "border-bottom: 1px solid rgba(150,150,150,15%);"
        "}"
        "QWidget#settingLeftPanel{"
           "border-right: 1px solid rgba(150,150,150,15%);"
         "}"

//        "QWidget#lyriclistLeftPanel{"
//           " background-color:#ff0000;"
//        "}"
//        "QWidget#lyriclistRightPanel{"
//           " background-color:#00ff00;"
//         "}"


        ;

        return str;
   }

    QString GetQLabelCss()
    {
        QString str =
        "QLabel{"
           " color:"+defautlLabelColor+";"
           "font-size:16px;"
        "}"

        "QLabel#labelTitle{"
            "color:"+titleColor+";"
            "font-size:16px;"
        "}"
        "QLabel#labelSepHeader{"
            "color:rgba(88, 88, 88, 88);"
            "font-size:20px;"
        "}"

        "QLabel#labelLine2{"
         "color:"+DeepLabelColor+";"
            "font-size:18px;"
        "}"
        "QLabel#labelCurrenLineEmptyTip{"
            "background:rgba(77, 77, 77, 10);"
           " border:1px solid "+defautlLabelColor+";"
        "}"

        "QLabel#labelPreviewTitle{"
            "color:"+DeepLabelColor+";"
            "font-size:24px;"
        "}"

        "QLabel#labelPreviewSongTip, QLabel#labelPreviewLyricTip{"
            "color:"+defautlLabelColor+";"
        "}"

        "QLabel#labelPreviewSongPath, QLabel#labelPreviewLyricPath{"
            "color:"+DeepLabelColor+";"
        "}"


        "QLabel#labelMessageTitle{"
            "color:"+DeepLabelColor+";"
            "font-size:18px;"
            "font-weight:bold;"
            "margin:5px;"
        "}"

         "QLabel#labelMessageContent{"
             "color:"+DeepLabelColor+";"
             "font-size:17px;"
             "margin:10px;"
         "}"

         "QLabel#labelTitleSearchLyric,QLabel#labelTitleSearchSong, QLabel#settingTitle{"
             "color:"+DeepLabelColor+";"
             "font-size:23px;"
             "margin:10px;"
         "}"

         "QLabel#labelLyricResultTip2, QLabel#labelLyricResultTip4,QLabel#labelNcmSongResultTip2, QLabel#labelNcmSongResultTip4{"
             "color:#427cae;"
             "font-size:17px;"
         "}"
         ;
        return str;
    }


    QString GetQPushButtonCss()
    {
        QString str =

"               QPushButton{"
        "font-size:16px;"
        "border:1px solid "+leftboardBorderColor+";"
"                   border-radius:5px;"
"                   color:"+defaultButtonColorNormal+";"
"                   padding: 3px;"
"                   max-height: 120px;"
"                   background:"+defaultButtonBgColorNormal+";"
"               }"
"               QPushButton:hover{"
"                   color: "+defaultButtonColorHover+";"
"                   background:"+defaultButtonBgColorHover+";"
"               }"
"               QPushButton:checked{"
"                   color: "+defaultButtonColorHover+";"
"               }"

"               QPushButton:pressed{"
"                   color: "+defaultButtonColorHover+";"
"               }"

"               QPushButton:disabled{"
"                   color:"+defaultButtonDisableColor+";"
"                   background: "+defaultButtonDisableBgColor+";"
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
"               QPushButton#btnClose, QPushButton#btnMessageClose{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 22px; height:22px;"
"                   background-image:url(\":/resource/image/btn_close_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnClose:hover, QPushButton#btnMessageClose:hover{"
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
"                   background-image:url(\":/resource/image/btn_play.png\");"
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
"                   background-color:"+itemSelectBackground+";"
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

                "QPushButton#btnCheckMark{"
"                border-style: solid;"
"                border: 2px;"
"                padding: 3px;"
                "background-color: "+subPageContainerColor+";"
                "width: 30px;"
                "height:30px;"
                "margin:0px;"
                "border-radius:17px;"
                "padding:0px;"
                "border-image:url(\":/resource/image/mark_skin_select.png\");"
                 "}"

                "QPushButton#btnPackupLyricBox, QPushButton#btnPackupSetting{"
                 "image:url(\""+btnPackbackToBoxImg +"\");"
                "border:1px solid "+leftboardBorderColor+";"
                "width: 42px;"
                "height:32px;"
                "padding:0px;"
                "}"

                /* 消息框蓝色按钮 */
                "QPushButton#btnOK{"
                "border:1px solid "+leftboardBorderColor+";"
                "    border-radius:5px;"
                "    color:"+defaultButtonColorNormal+";"
                "    padding: 3px;"
                "    max-height: 120px;"
                "    background:"+blueButtonColorNormal+";"
                "}"
                "QPushButton#btnOK:hover{"
                "    min-height: 30px;"
                "    color: "+defaultButtonColorHover+";"
                "    background:"+blueButtonColorHover+";"
                "}"

                //搜索歌曲列表按钮
                "QPushButton#m_pBtnPreviewSong{"
                      "border: none; "
                      "background-color: transparent; "
                      "image:url(:/resource/image/btn_download_normal);"
                "}"
                 "QPushButton#m_pBtnPreviewSong:hover{"
                      "image:url(:/resource/image/btn_download_hover);"
                  "} "
                 "QPushButton#m_pBtnPreviewSong:pressed {"
                      "image:url(:/resource/image/btn_download_hover);"
                 "}"


                "QPushButton#m_pBtnListenSong{"
                      "border: none; "
                      "background-color: transparent; "
                      "image:url(:/resource/image/btn_download_normal);"
                "}"
                 "QPushButton#m_pBtnListenSong:hover{"
                      "image:url(:/resource/image/btn_download_hover);"
                  "} "
                 "QPushButton#m_pBtnListenSong:pressed {"
                      "image:url(:/resource/image/btn_download_hover);"
                 "}"


                "QPushButton#m_pBtnDownloadSong{"
                      "border: none; "
                      "background-color: transparent; "
                      "image:url(:/resource/image/btn_download_normal);"
                "}"
                 "QPushButton#m_pBtnDownloadSong:hover{"
                      "image:url(:/resource/image/btn_download_hover);"
                  "} "
                 "QPushButton#m_pBtnDownloadSong:pressed {"
                      "image:url(:/resource/image/btn_download_hover);"
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
"                    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 "+baseColorLighter+", stop:1 "+baseColor+");"
"                }"
                /* 拖动块部分 */
"                QSlider#sliderSong::handle:horizontal {"
"                    width: 14px;"
"                    margin-top: -5px;"
"                    margin-bottom: -5px;"
"                    border-radius: 7px;"
"                    border: 1px solid #cccccc;"
"                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.25 "+baseColor+", stop:0.4 #fefefe);"
"                }"
"                QSlider#sliderSong::handle:horizontal:hover {"
"                    border: 1px solid #aaaaaa;"
"                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.3 "+baseColor+", stop:0.45 #fefefe);"
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
"                    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 "+baseColorLighter+", stop:1 "+baseColor+");"
"                }"
                /* 拖动块部分 */
"                QSlider#sliderSound::handle:horizontal {"
"                    width: 10px;"
"                    margin-top: -3px;"
"                    margin-bottom: -3px;"
"                    border-radius: 5px;"
"                    border: 1px solid #cccccc;"
"                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.25 "+baseColor+", stop:0.4 #fefefe);"
        "                }"
"                QSlider#sliderSound::handle:horizontal:hover {"
"                    border: 1px solid #aaaaaa;"
"                    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5,stop:0.3 "+baseColor+", stop:0.45 #fefefe);"
"                }"

"                QSlider#SliderHue::groove:horizontal,QSlider#SliderLightness::groove:horizontal {"
                "height: 6px;"
                "border-radius: 2px;"
                "}"
"                QSlider#SliderHue::handle:horizontal,QSlider#SliderLightness::handle:horizontal {"
"                    width: 20px;"
"                    margin-top: -7px;"
"                    margin-bottom: -7px;"
"                    border-radius: 10px;"
"                    border: 1px solid #aaaaaa;"
"                    background: #fefefe;"
        "                }"
"                QSlider#SliderHue::handle:horizontal:hover,QSlider#SliderLightness::handle:horizontal:hover {"
"                    background: #eeeeee;"
"                }"




               ;

        return str;
    }


    QString GetQEditCss()
    {
        QString str =
        "QLineEdit{"
               "color:"+DeepLabelColor+";"
               " border: 1px solid rgba(40, 20, 20,50); "
                "   background: "+editBackground+"; "
                "   selection-background-color: green;"
        "}"

        "QTextEdit{"
               "color:"+DeepLabelColor+";"
               " border: 1px solid rgba(40, 20, 20,50); "
                "   background:"+editBackground+"; "
                "   selection-background-color: green;"
        "}"

        ;

        return str;
    }

    QString GetQMenuCss()
    {
        QString str =
                "QMenu::separator {"
//                "height: 2px;"
//                "background: lightblue;"
//                "margin-left: 10px;"
//                "margin-right: 5px;"
            "}";

        return str;
    }

    QString GetQScrollBarCss()
    {

        QString str =

"QScrollBar:vertical"
"{"
"    width:10px;"
"    background:rgba(0,0,0,0%);"
"    margin:0px,0px,0px,0px;"
"    padding-top:0px;"   // 留出0px给上面和下面的箭头
"    padding-bottom:0px;"
"}"
"QScrollBar::handle:vertical"
"{"
"    width:10px;"
"    background:rgba(100,100,100,25%);"
"    border-radius:5px;"   // 滚动条两端变成椭圆
"    min-height:20;"
"}"
"QScrollBar::handle:vertical:hover"
"{"
"    width:10px;"
"    background:rgba(100,100,100,50%);"   // 鼠标放到滚动条上的时候，颜色变深
"    border-radius:5px;"
"    min-height:20;"
"}"

"QScrollBar::add-line:vertical "  // 这个应该是设置下箭头的，3.png就是箭头
"{"
"    height:11px;width:10px;"
"    border-image:url(:/images/a/3.png);"
"    subcontrol-position:bottom;"
"}"

"QScrollBar::sub-line:vertical"   // 设置上箭头
"{"
"    height:11px;width:10px;"
"    border-image:url(:/images/a/1.png);"
"    subcontrol-position:top;"
"}"
"QScrollBar::add-line:vertical:hover"   // 当鼠标放到下箭头上的时候
"{"
"    height:11px;width:10px;"
"    border-image:url(:/images/a/4.png);"
"    subcontrol-position:bottom;"
"}"
"QScrollBar::sub-line:vertical:hover"  // 当鼠标放到下箭头上的时候
"{"
"    height:11px;width:10px;"
"    border-image:url(:/images/a/2.png);"
"    subcontrol-position:top;"
"}"
"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"   // 当滚动条滚动的时候，上面的部分和下面的部分
"{"
"    background:rgba(0,0,0,0%);"
"    border-radius:5px;"
"}";

    return str;
    }

    QString GetQTabWidgetCss()
    {
        QString str = ""
        "QTabWidget:pane {"
        "   border-top: 1px solid "+leftboardBorderColor+";"
        "}"
        "QTabWidget::tab-bar {"
        "   left: 72px;"
        "}"
        "QTabBar:tab {"
        "   margin-right:30px;"
        "   font:18px;"
        "   height: 40px;"
        "   border-bottom:5px solid;"
        "   color:"+ DeepLabelColor+";"
        "   border-bottom-color: rgba(0, 0, 0, 0);"
        "   min-width: 80px;"
        "}"

        "QTabBar:tab:selected {"
        "   color:"+ btnTabLeftBoderColor+";"
        "   border-bottom-color: "+btnTabLeftBoderColor+";"
        "}";

        return str;
    }

    QString GetQTableWidgetCss()
    {
        QString str = ""
        " QTableView"
        " {"
        "     background-color: rgba(255, 255, 255, 0); "
        //"     alternate-background-color:#e3edf9;"
        "     font:14px;"
        "     color:"+DeepLabelColor+";"
        "     selection-background-color:"+itemSelectBackground+";"
        "     gridline-color: #00000000;"
        "     border:1px solid "+leftboardBorderColor+";"
        " }"
        " QTableView::item"
        " {	"
        "     font:14px;"
        "     color:"+DeepLabelColor+";"
        "     gridline-color: #00000000; "
        "     text-align:middle;"
        " }"
        "  "
        " QTableView::item:selected"
        " {	"
        "     color:"+DeepLabelColor+";"
        "     background:"+itemSelectBackground+";"    //比较奇怪的是，selection-background-color 已经设置了背景了
                                                       //这里设置选中字体颜色color 偏偏得再加上background 才生效
        " }"
        " QTableView"
        "{"
        "     border-top:none;"
        "}"
        " QHeaderView{"
        "     border:none;"
        " margin:0px; padding:0px;"
        "     background-color: #00000000;"
        "} "
        " QHeaderView::section {	"
        "     color:"+defautlLabelColor+";"
        "     font:bold 14px;"
        "     text-align:center;"
        "     height:32px;"
        "     background-color: #00000000;"
        "     border:1px solid "+leftboardBorderColor+";"
        "     border-left:none;"
        "     border-top:none;"
        " }"
        "  "
        " QHeaderView::section:first "
        " {"
        //"     border-left:1px solid #8faac9;"
        " }"

                ;
        return str;
    }

    void setDefaultParameter()
    {
        //容器相关
        topContainerBgImage        =":/resource/image/top_background_black.png";
        topContainerBgColor        ="";
        leftboardBgColor           ="#191b1f                                     ";
        leftboardBorderColor       ="#23262c";
        bottomContainerColor       ="#222222                                     ";
        bottomContainerBorderColor ="rgba(ff, ff, ff, ff)                        ";
        subPageContainerColor      ="#16181c                                     ";
        PopWindowBgColor           ="#2d2f33                                     ";

        MessageBoxBgColor           ="#2d2f33";
        MessageBoxBottomColor       ="#292b2f";

        //label、button 相关
        fontFamily                 ="Microsoft YaHei                             ";
        defautlLabelColor          ="#909090                                     ";
        DeepLabelColor             ="#ffffff";
        defaultButtonColorNormal    ="#909090";
        defaultButtonColorHover     ="#f0f0f0";
        defaultButtonBgColorNormal ="rgba(100, 100, 100, 100)";
        defaultButtonBgColorHover ="rgba(100, 100, 100, 150)";
        defaultButtonDisableColor  ="#454545";
        defaultButtonDisableBgColor="#222222";

        blueButtonColorNormal       ="#2e4e7e";
        blueButtonColorHover        ="#3a6098";


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

        btnPackbackToBoxImg       =":/resource/image/btn_pack_back_to_box_black.png";

        //QSlider
        baseColor                 ="#CC0033";
        baseColorLighter          ="#ff3344";

        //其他
        itemSelectBackground       ="rgba(175, 175, 175, 55)";
        editBackground             =" rgba(222, 222, 222,35)";
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
    QString MessageBoxBgColor;          //#444648
    QString MessageBoxBottomColor;      //#242628

    //label、button 相关
    QString defautlLabelColor;          //#909090
    QString DeepLabelColor;             //#ffffff
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
    QString defaultButtonBgColorNormal;
    QString defaultButtonBgColorHover;
    QString defaultButtonDisableColor;
    QString defaultButtonDisableBgColor;

    QString blueButtonColorNormal;  //#2e4e7e
    QString blueButtonColorHover;   //#3a6098

    QString btnPackbackToBoxImg;        //":/resource/image/btn_pack_back_to_box_black.png"

    //QSlider
    QString baseColor;                  //#CC0033
    QString baseColorLighter;           //#ff3344

    //其他
    QString itemSelectBackground;
    QString editBackground;
};

#endif // ISKIN_H
