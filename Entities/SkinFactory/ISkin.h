#ifndef ISKIN_H
#define ISKIN_H
#include <QString>
#include "SkinBoxWidget.h"
#include "BesScaleUtil.h"

class ISkin
{
public:
    //按和最大宽的比例缩放,得到字符串表示
    inline QString scaleNum(int num)
    {
        int numAfterScale = num * BesScaleUtil::scale();
        return QString::number(numAfterScale);
    }

    //按和最大宽的比例缩放，但是又不要缩放太小，取缩放前后均值,得到字符串表示
    inline QString mscaleNum(int num)
    {
        int numAfterScale = num * BesScaleUtil::mscale();
        return QString::number(numAfterScale);
    }

    //缩放字体大小,得到字符串表示
    inline QString scaleFontSize(int size)
    {
        int sizeAfterScale = BesScaleUtil::fontSizeScale(size);
        return QString::number(sizeAfterScale);
    }

    //获得 CSS 样式字符串
    QString GetCssString()
    {
        setDefaultParameter();

        SetParameter();

        QString cssContent;

        cssContent += GetQWidgetCss();
        cssContent += GetQScrollArea();
        cssContent += GetQLabelCss();
        cssContent += GetQPushButtonCss();
        cssContent += GetQSliderCss();
        cssContent += GetQEditCss();
        cssContent += GetQMenuCss();
        cssContent += GetQScrollBarCss();
        cssContent += GetQTabWidgetCss();
        cssContent += GetQTableWidgetCss();
        cssContent += GetQListWidgetCss();
        cssContent += GetQCheckBoxCss();

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

        "QWidget#widgetMessageBoxContainer"
        "{"
           "border: 5px solid #3b3a3d;"
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

        "QWidget#lyriclistLeftPanel{"
             "background-color:"+ leftboardBgColor+";"
         "}"

         "QWidget#settingUnitContainer"
         "{"
           "background-color:rgba(0,0,0,0%);"
           "padding:"+scaleNum(20)+"px;"
         "}"

        "QWidget#lyriclistLeftPanel{"
//           "border: 1px solid #ff0000;"
//           " background-color:#ff0000;"
        "}"
        "QWidget#SettingUnitContainer{"
        "border-bottom: 1px solid rgba(150,150,150,15%);"
        "margin-right:"+scaleNum(20)+"px;"
         "}"
        "QWidget#SettingUnitContainer2{"
             " border-bottom: 1px solid rgba(150,150,150,15%);"
//           " background-color:#000044;"
         "}"
        ;

        return str;
   }

    QString GetQScrollArea()
    {
          QString str =

          "QScrollArea#ScrollLyricPanelScrollArea, QScrollArea#scrollAreaLeftList,QScrollArea#scrollAreaSetting"
          "{"
              "background-color:rgba(0,0,0,0%);"
          "}"

          "QScrollArea#scrollAreaLeftList"
          "{"
              "background-color:"+ leftboardBgColor+";"
              "border-right: 1px solid "+ leftboardBorderColor +";"
          "}"
          ;
          return str;
    }

    QString GetQLabelCss()
    {
        QString str =
        "QLabel{"
           " color:"+defautlLabelColor+";"
           "font-size:"+scaleFontSize(16)+"px;"
        "}"

        "QLabel#labelTitle{"
            "color:"+titleColor+";"
            "font-size:"+scaleFontSize(16)+"px;"
        "}"
        "QLabel#labelSepHeader{"
            "color:rgba(88, 88, 88, 88);"
            "font-size:"+scaleFontSize(20)+"px;"
        "}"

        "QLabel#labelLine2{"
         "color:"+DeepLabelColor+";"
            "font-size:"+scaleFontSize(18)+"px;"
        "}"

        "QLabel#labelSettingHighColor{"
         "color:"+DeepLabelColor+";"
            "font-size:"+scaleFontSize(17)+"px;"
        "}"


        "QLabel#labelCurrenLineEmptyTip{"
            "background:rgba(77, 77, 77, 10);"
           " border:1px solid "+defautlLabelColor+";"
        "}"

        "QLabel#labelPreviewTitle{"
            "color:"+DeepLabelColor+";"
            "font-size:"+scaleFontSize(24)+"px;"
        "}"

        "QLabel#labelPreviewSongTip, QLabel#labelPreviewLyricTip{"
            "color:"+defautlLabelColor+";"
        "}"

        "QLabel#labelPreviewSongPath, QLabel#labelPreviewLyricPath{"
            "color:"+DeepLabelColor+";"
        "}"


        "QLabel#labelMessageTitle{"
            "color:"+DeepLabelColor+";"
            "font-size:"+scaleFontSize(18)+"px;"
            "font-weight:bold;"
            "margin:0px;"
        "}"

         "QLabel#labelMessageContent{"
             "color:"+DeepLabelColor+";"
             "font-size:"+scaleFontSize(17)+"px;"
             "margin:"+scaleNum(10)+"px;"
         "}"

         "QLabel#labelTitleSearchLyric,QLabel#labelTitleSearchSong, "
         "QLabel#settingTitle, QLabel#labelListInfoTitle{"
             "color:"+DeepLabelColor+";"
             "font-size:"+scaleFontSize(23)+"px;"
             "margin:"+scaleNum(10)+"px;"
         "}"

         "QLabel#labelLyricResultTip2, QLabel#labelLyricResultTip4,QLabel#labelNcmSongResultTip2, QLabel#labelNcmSongResultTip4{"
             "color:#427cae;"
             "font-size:"+scaleFontSize(17)+"px;"
         "}"

         "QLabel#labelAddItemNameTip{"
             "color:"+DeepLabelColor+";"
             "font-weight:bold;"
             "font-size:"+scaleFontSize(19)+"px;"
             "margin:1px;"
         "}"

         "QLabel#labelLyricListRedMark{"
             "color:#e03f40;"
             "font:"+scaleFontSize(15)+"px;"
             "border:1px solid #e03f40;"
             "border-radius:2px;"
         "}"

      "QLabel#settingUnitTitle{"
          "color:"+DeepLabelColor+";"
          "font-size:"+scaleFontSize(23)+"px;"
          "margin-top:"+scaleNum(15)+"px;"
          "margin-bottom:"+scaleNum(15)+"px;"
      "}"
         ;
        return str;
    }


    QString GetQPushButtonCss()
    {
        QString str =

"               QPushButton{"
"                   font-size:"+scaleNum(16)+"px;"
"                   border:1px solid "+leftboardBorderColor+";"
"                   border-radius:5px;"
"                   color:"+defaultButtonColorNormal+";"
"                   padding: "+scaleNum(3)+"px;"
"                   max-height: "+scaleNum(120)+"px;"
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
"                   width: "+scaleNum(120)+"px; height:"+scaleNum(30)+"px;"
"                   border-image:url(\":/resource/image/logo.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSkinBox{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_skin_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSkinBox:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_skin_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSetting{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_setting_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSetting:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_setting_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnMini{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_mini_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnMini:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_mini_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnMax{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_max_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnMax:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_max_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnRestore{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_restore_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnRestore:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_restore_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnClose, QPushButton#btnMessageClose{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_close_normal_white.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnClose:hover, QPushButton#btnMessageClose:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: "+mscaleNum(22)+"px; height:"+mscaleNum(22)+"px;"
"                   border-image:url(\":/resource/image/btn_close_press_white.png\");"
"                   background-repeat:no-repeat;"
"               }"

               /*3个播放按钮*/
"               QPushButton#btnPreSong{"
"                   width: "+scaleNum(32)+"px;"
"                   height:"+scaleNum(32)+"px;"
"                   border-radius: "+scaleNum(playBorderSize + 16)+"px;"
"                   border:"+scaleNum(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playBgColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderColor+";"
"                   border-image:url(\":/resource/image/btn_pre_song.png\");"
"                   background-repeat:no-repeat;"
"               }"
"                QPushButton#btnNextSong{"
"                   width: "+scaleNum(32)+"px;"
"                   height:"+scaleNum(32)+"px;"
"                   border-radius: "+scaleNum(playBorderSize + 16)+"px;"
"                   border:"+scaleNum(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playBgColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderColor+";"
"                   border-image:url(\":/resource/image/btn_next_song.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPlayAndPause{"
"                   width: "+scaleNum(40)+"px;"
"                   height:"+scaleNum(40)+"px;"
"                   border-radius: "+scaleNum(playBorderSize + 20)+"px;"
"                   border:"+scaleNum(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playBgColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderColor+";"
"                   border-image:url(\":/resource/image/btn_play.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPreSong:hover{"
"                   width: "+scaleNum(32)+"px;"
"                   height:"+scaleNum(32)+"px;"
"                   border-radius: "+scaleNum(playBorderSize + 16)+"px;"
"                   border:"+scaleNum(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playHoverColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderHoverColor+";"
"                   border-image:url(\":/resource/image/btn_pre_song.png\");"
"                   background-repeat:no-repeat;"
"               }"
"                QPushButton#btnNextSong:hover{"
"                   width: "+scaleNum(32)+"px;"
"                   height:"+scaleNum(32)+"px;"
"                   border-radius: "+scaleNum(playBorderSize + 16)+"px;"
"                   border:"+scaleNum(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playHoverColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderHoverColor+";"
"                   border-image:url(\":/resource/image/btn_next_song.png\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPlayAndPause:hover{"
"                   width: "+scaleNum(40)+"px;"
"                   height:"+scaleNum(40)+"px;"
"                   border-radius: "+scaleNum(playBorderSize + 20)+"px;"
"                   border:"+scaleNum(playBorderSize)+"px;"
"                   padding:0px;"
"                   margin: 0px;"
"                   color: #F0F0F0;"
"                   background-color:"+playHoverColor+";"
"                   border-style:solid;"
"                   border-color:"+playBorderHoverColor+";"
"                   border-image:url(\":/resource/image/btn_pause.png\");"
"                   background-repeat:no-repeat;"
"               }"

               /* 播放栏其他按钮 */
"               QPushButton#btnSound:!checked{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 26px; height:26;"
"                   background-image:url(\""+btnSoundImage+"\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSound:!checked:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 26px; height:26px;"
"                   background-image:url(\""+btnSoundImageHover+"\");"
                "                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSound:checked{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 26px; height:26px;"
"                   background-image:url(\""+btnSoundMuteImage+"\");"
                "                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnSound:checked:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 26px; height:26px;"
"                   background-image:url(\""+btnSoundMuteImageHover+"\");"
                "                   background-repeat:no-repeat;"
"               }"



"               QPushButton#btnPlayModeSingle{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 25px; height:25px;"
"                   background-image:url(\""+btnPlayModeSingleNormal+"\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPlayModeSingle:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 25px; height:25px;"
"                   background-image:url(\""+btnPlayModeSingleHover+"\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPlayModeSingleCycle{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 25px; height:25px;"
"                   background-image:url(\""+btnPlayModeSingleCycleNormal+"\");"
"                   background-repeat:no-repeat;"
"               }"
"               QPushButton#btnPlayModeSingleCycle:hover{"
"                   border: 0px;border-radius:0px;"
"                   background-color:rgba(0, 0, 0, 0);"
"                   padding: 0px;margin: 0px;"
"                   width: 25px; height:25px;"
"                   background-image:url(\""+btnPlayModeSingleCycleHover+"\");"
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
"                   width: "+scaleNum(180)+"px;"
"                   height:"+scaleNum(40)+"px;"
"                   padding-left:"+scaleNum(18)+"px;"
"                   margin: 0px;"
                    "text-align:left;"
"                   border-radius: 0px;"
"                   border-left: "+scaleNum(5)+"px solid rgba(0, 0, 0, 0);"
"                   border-style:solid;"
"               }"
"               QPushButton#btnMakingLyric:hover, QPushButton#btnDownloadSong:hover, QPushButton#btnDownloadLyric:hover{"
"                   background-color:rgba(0, 0, 0, 0);"
"                   border-left: "+scaleNum(5)+"px solid rgba(0, 0, 0, 0);"
"               }"
"               QPushButton#btnMakingLyric:checked, QPushButton#btnDownloadSong:checked, QPushButton#btnDownloadLyric:checked{"
"                   background-color:"+itemSelectBackground+";"
"                   border-left: "+scaleNum(5)+"px solid "+btnTabLeftBoderColor+";"
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

                "QPushButton#btnPackupLyricBox, QPushButton#btnPackupSetting,  QPushButton#btnPackupLyricList{"
                 "image:url(\""+btnPackbackToBoxImg +"\");"
                "border:1px solid "+leftboardBorderColor+";"
                "width: "+scaleNum(42)+"px;"
                "height:"+scaleNum(32)+"px;"
                "padding:0px;"
                "}"

                /* 蓝色按钮(消息框等) */
                "QPushButton#btnOK, QPushButton#btnAddListItem"
                ",QPushButton#btnSaveLrcItem, QPushButton#btnCreateLrcItem"
                ",QPushButton#btnDeleteLrcList, QPushButton#btnSaveLrcListModified{"
                "border:1px solid "+leftboardBorderColor+";"
                "    border-radius:"+scaleNum(5)+"px;"
                "    color:#f0f0f0;"
                "    padding: "+scaleNum(3)+"px;"
                "    max-height: "+scaleNum(120)+"px;"
                "    background:"+blueButtonColorNormal+";"
                "}"
                "QPushButton#btnOK:hover, QPushButton#btnAddListItem:hover"
                ",QPushButton#btnSaveLrcItem:hover,QPushButton#btnCreateLrcItem:hover"
                ",QPushButton#btnDeleteLrcList:hover,QPushButton#btnSaveLrcListModified:hover{"
                "    min-height: "+mscaleNum(30)+"px;"
                "    color:#f0f0f0;"
                "    background:"+blueButtonColorHover+";"
                "}"
                "QPushButton#btnOK:disabled, QPushButton#btnAddListItem:disabled"
                ",QPushButton#btnSaveLrcItem:disabled, QPushButton#btnCreateLrcItem:disabled"
                ",QPushButton#btnDeleteLrcList:disabled, QPushButton#btnSaveLrcListModified:disabled{"
                "    min-height: "+mscaleNum(30)+"px;"
                "    background:"+blueButtonColorDisabled+";"
                "}"


                "QPushButton#btnDownloadWindow, QPushButton#btnDownloadLinux"
                ",QPushButton#btnDownloadMac{"
                "border:1px solid "+leftboardBorderColor+";"
                "    border-radius:"+scaleNum(5)+"px;"
                "    color:#f0f0f0;"
                "    padding: "+scaleNum(3)+"px;"
                "    max-height: "+scaleNum(120)+"px;"
                "    background:"+blueButtonColorNormal+";"
                "}"
                "QPushButton#btnDownloadWindow:hover, QPushButton#btnDownloadLinux:hover"
                ",QPushButton#btnDownloadMac:hover{"
                "    min-height: "+mscaleNum(30)+"px;"
                "    color:#f0f0f0;"
                "    background:"+blueButtonColorHover+";"
                "}"

                //搜索歌曲列表按钮
                "QPushButton#m_pBtnPreviewSong{"
                      "border: none; "
                      "background-color: transparent; "
                      "image:url("+ imgViewMusicNormal +");"
                "}"
                 "QPushButton#m_pBtnPreviewSong:hover{"
                      "image:url("+ imgViewMusicHover +");"
                  "} "
                 "QPushButton#m_pBtnPreviewSong:pressed {"
                      "image:url("+ imgViewMusicHover +");"
                 "}"


                "QPushButton#m_pBtnListenSong{"
                      "border: none; "
                      "background-color: transparent; "
                      "image:url("+ imgListenMusicNormal +");"
                "}"
                 "QPushButton#m_pBtnListenSong:hover{"
                      "image:url("+ imgListenMusicHover +");"
                  "} "
                 "QPushButton#m_pBtnListenSong:pressed {"
                      "image:url("+ imgListenMusicHover +");"
                 "}"

                //歌词单列表按钮
                "QPushButton#m_pBtnDeleteListItem{"
                      "border: none; "
                      "background-color: transparent; "
                      "image:url("+ imgDeleteListItemNormal +");"
                "}"
                 "QPushButton#m_pBtnDeleteListItem:hover{"
                      "image:url("+ imgDeleteListItemHover +");"
                  "} "
                 "QPushButton#m_pBtnDeleteListItem:pressed {"
                      "image:url("+ imgDeleteListItemHover +");"
                 "}"


                "QPushButton#m_pBtnEditListItem{"
                      "border: none; "
                      "background-color: transparent; "
                      "image:url("+ imgEditListItemNormal +");"
                "}"
                 "QPushButton#m_pBtnEditListItem:hover{"
                      "image:url("+ imgEditListItemHover +");"
                  "} "
                 "QPushButton#m_pBtnEditListItem:pressed {"
                      "image:url("+ imgEditListItemHover +");"
                 "}"



                "QPushButton#m_pBtnDownloadSong{"
                      "border: none; "
                      "background-color: transparent; "
                      "image:url("+ imgDownloadMusicNormal +");"
                "}"
                 "QPushButton#m_pBtnDownloadSong:hover{"
                      "image:url("+ imgDownloadMusicHover +");"
                  "} "
                 "QPushButton#m_pBtnDownloadSong:pressed {"
                      "image:url("+ imgDownloadMusicHover +");"
                 "}"


                //歌词列表按钮

                "QPushButton#btnAddItem, QPushButton#btnToggleList{"
                    "border: none; "
                    "background-color: transparent; "
                     "width:24px; height:24px;"
                "}"

                "QPushButton#btnAddItem{"
                      "image:url("+imgAddListNormal+");"
                "}"
                "QPushButton#btnAddItem:hover{"
                      "image:url("+imgAddListHover+");"
                "}"

                "QPushButton#btnToggleList{"
                      "image:url("+imgToggleListNormal+");"
                "}"
                "QPushButton#btnToggleList:hover{"
                      "image:url("+imgToggleListHover+");"
                "}"
                "QPushButton#btnToggleList:checked{"
                      "image:url("+imgToggleListCheckedNormal+");"
                "}"
                "QPushButton#btnToggleList:checked:hover{"
                      "image:url("+imgToggleListCheckedHover+");"
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
               "padding:"+scaleNum(5)+"px;"
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
                        "background: green;"
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
        "   left: "+scaleNum(72)+"px;"
        "}"
        "QTabBar:tab {"
        "   margin-right:"+scaleNum(30)+"px;"
        "   font:"+scaleFontSize(18)+"px;"
        "   height: "+scaleNum(40)+"px;"
        "   border-bottom:"+scaleNum(5)+"px solid;"
        "   color:"+ DeepLabelColor+";"
        "   border-bottom-color: rgba(0, 0, 0, 0);"
        "   min-width: "+mscaleNum(96)+"px;"
        "}"

        "QTabBar:tab:selected {"
        "   color:"+ btnTabLeftBoderColor+";"
        "   border-bottom-color: "+btnTabLeftBoderColor+";"
        "}"

;
        return str;
    }

    QString GetQTableWidgetCss()
    {
        QString str = ""
        " QTableView"
        " {"
        "     background-color: rgba(255, 255, 255, 0); "
        //"     alternate-background-color:#e3edf9;"
        "     font:"+scaleFontSize(14)+"px;"
        "     color:"+DeepLabelColor+";"
        "     selection-background-color:"+itemSelectBackground+";"
        "     gridline-color: #00000000;"
        "     border:1px solid "+leftboardBorderColor+";"
        " }"
        " QTableView::item"
        " {	"
        "     font:"+scaleFontSize(14)+"px;"
        "     color:"+DeepLabelColor+";"
        "     gridline-color: #00000000; "
        "     text-align:center;"
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
        "     margin:0px; padding:0px;"
        "     background-color: #00000000;"
        "} "
        " QHeaderView::section {	"
        "     color:"+defautlLabelColor+";"
        "     font:bold "+scaleFontSize(14)+"px;"
        "     text-align:center;"
        "     height:"+scaleNum(32)+"px;"
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

    QString GetQListWidgetCss()
    {
        QString str = ""
         "QListWidget{"
              "color:" + listItemNormalColor + ";"
              "background: transparent;border:0px solid gray;"
         "}"
         "QListWidget::Item{"
             "border-left: 5px solid transparent;"
             "font:14px;"
             "height:35px;"
             "padding-left:10;"
             "background:transparent;"
         "}"
         "QListWidget::Item:hover{"
            " color:" + DeepLabelColor + ";"
         "}"
         "QListWidget::Item:selected,QListWidget::Item:selected:active{"
            " background-color:"+itemSelectBackground+";"
            " border-left: 5px solid "+btnTabLeftBoderColor+";"
            " color:" + DeepLabelColor + ";"
          "}"
                ;



        return str;
    }

    QString GetQCheckBoxCss()
    {
        QString str = ""
          "  QCheckBox::indicator:unchecked {"
          "    image: url("+checkbox_unchecked+");"
          "}"
          "QCheckBox::indicator:checked {"
          "    image: url("+checkbox_checked+");"
          "}"

        ;
        return str;
    }

    ///////////////////////////////////////////////////////////
protected:

    void setDefaultParameter()
    {
        setBlackThemeBaseParameter();
    }

    //设置黑色主题基本参数
    void setBlackThemeBaseParameter()
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
        titleColor                 ="#ffffff                                     ";
        defaultButtonColorNormal    ="#909090";
        defaultButtonColorHover     ="#f0f0f0";
        defaultButtonBgColorNormal ="rgba(100, 100, 100, 100)";
        defaultButtonBgColorHover ="rgba(100, 100, 100, 150)";
        defaultButtonDisableColor  ="#454545";
        defaultButtonDisableBgColor="#222222";

        blueButtonColorNormal       ="#2e4e7e";
        blueButtonColorHover        ="#3a6098";
        blueButtonColorDisabled      ="#2e4e7eee";

        blueButtonColorNormal       ="#0c73c2";
        blueButtonColorHover        ="#1167a8";
        blueButtonColorDisabled      ="#2e4e7eee";


        titleColor                 ="#ffffff                                     ";
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

        btnPackbackToBoxImg         =":/resource/image/btn_pack_back_to_box_black.png";


        //特殊按钮样式
        imgViewMusicNormal          =":/resource/image/view_music_normal_black";
        imgViewMusicHover           =":/resource/image/view_music_hover_black";
        imgListenMusicNormal        =":/resource/image/listen_music_normal_black";
        imgListenMusicHover         =":/resource/image/listen_music_hover_black";
        imgDownloadMusicNormal      =":/resource/image/download_music_item_normal_black";
        imgDownloadMusicHover       =":/resource/image/download_music_item_hover_black";
        imgDeleteListItemNormal     =":/resource/image/delete_list_item_normal_black";
        imgDeleteListItemHover      =":/resource/image/delete_list_item_hover_black";
        imgEditListItemNormal       =":/resource/image/edit_list_info_normal_black";
        imgEditListItemHover        =":/resource/image/edit_list_info_hover_black";

        imgAddListNormal      =":/resource/image/add_list_normal_black";
        imgAddListHover       =":/resource/image/add_list_hover_black";
        imgToggleListNormal         =":/resource/image/btn_expand_normal_black";
        imgToggleListHover          =":/resource/image/btn_expand_hover_black";
        imgToggleListCheckedNormal  =":/resource/image/btn_collaps_normal_black";
        imgToggleListCheckedHover   =":/resource/image/btn_collaps_hover_black";

        //QList
        listItemNormalColor        ="#c0c0c0";

        //QSlider
        baseColor                 ="#CC0033";
        baseColorLighter          ="#ff3344";

        //QCheckBox
        checkbox_unchecked      =":/resource/image/checkbox_unchecked_black.png";
        checkbox_checked        =":/resource/image/checkbox_checked_black.png";

        //其他
        itemSelectBackground       ="rgba(175, 175, 175, 55)";
        editBackground             =" rgba(222, 222, 222,35)";
    }

    //设置其他主题基本参数
    void setWhiteThemeBaseParameter()
    {
        //容器相关
        topContainerBgImage        ="";
        topContainerBgColor        ="#c62f2f";
        leftboardBgColor           ="#f5f5f7                                     ";
        leftboardBorderColor       ="rgba(33, 33, 33, 33)                        ";
        bottomContainerColor       ="#f6f6f8                                     ";
        bottomContainerBorderColor ="rgba(33, 33, 33, 33)                        ";
        subPageContainerColor      ="rgb(250, 250, 250)        ";
        PopWindowBgColor           ="#2d2f33                                     ";

        MessageBoxBgColor           ="#fafafa";
        MessageBoxBottomColor       ="#f5f5f7";

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

        blueButtonColorNormal       ="#0c73c2";
        blueButtonColorHover        ="#1167a8";
        blueButtonColorDisabled      ="#2e4e7eee";

        titleColor                 ="#ffffff                                     ";
        playBgColor                ="#e83c3c                                     ";
        playHoverColor             ="#c62f2f                                     ";
        playBorderColor            ="#666666                                     ";
        playBorderHoverColor       ="#888888                                     ";
        playBorderSize             =0;

        btnSoundImage              =":/resource/image/sound_normal_white.png";
        btnSoundImageHover         =":/resource/image/sound_hover_white.png";
        btnSoundMuteImage          =":/resource/image/sound_mute_normal_white.png";
        btnSoundMuteImageHover     =":/resource/image/sound_mute_hover_white.png";
        btnPlayModeSingleNormal     =":/resource/image/play_mode_single_normal_black.png";
        btnPlayModeSingleHover      =":/resource/image/play_mode_single_hover_black.png";
        btnPlayModeSingleCycleNormal=":/resource/image/play_mode_single_cycle_normal_white.png";
        btnPlayModeSingleCycleHover =":/resource/image/play_mode_single_cycle_hover_white.png";

        btnLyricImage              =":/resource/image/btn_close_normal_white.png";
        btnLyricImageHover         =":/resource/image/btn_close_press_white.png";
        btnTabLeftBoderColor       ="#bb0033                                     ";

        btnPackbackToBoxImg       =":/resource/image/btn_pack_back_to_box_white.png";

        //特殊按钮样式
        imgViewMusicNormal          =":/resource/image/view_music_normal_white";
        imgViewMusicHover           =":/resource/image/view_music_hover_white";
        imgListenMusicNormal        =":/resource/image/listen_music_normal_white";
        imgListenMusicHover         =":/resource/image/listen_music_hover_white";
        imgDownloadMusicNormal      =":/resource/image/download_music_item_normal_white";
        imgDownloadMusicHover       =":/resource/image/download_music_item_hover_white";
        imgDeleteListItemNormal     =":/resource/image/delete_list_item_normal_white";
        imgDeleteListItemHover      =":/resource/image/delete_list_item_hover_white";
        imgEditListItemNormal       =":/resource/image/edit_list_info_normal_white";
        imgEditListItemHover        =":/resource/image/edit_list_info_hover_white";

        imgAddListNormal      =":/resource/image/add_list_normal_white";
        imgAddListHover       =":/resource/image/add_list_hover_white";
        imgToggleListNormal         =":/resource/image/btn_expand_normal_white";
        imgToggleListHover          =":/resource/image/btn_expand_hover_white";
        imgToggleListCheckedNormal  =":/resource/image/btn_collaps_normal_white";
        imgToggleListCheckedHover   =":/resource/image/btn_collaps_hover_white";

        //QList
        listItemNormalColor        ="#444444";

        //QSlider
        baseColor                 ="#CC0033";
        baseColorLighter          ="#ff3344";

        //QCheckBox
        checkbox_unchecked      =":/resource/image/checkbox_unchecked_white.png";
        checkbox_checked        =":/resource/image/checkbox_checked_white.png";

        //其他
        editBackground            = "white";
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

    QString btnSoundImage;
    QString btnSoundImageHover;
    QString btnSoundMuteImage;
    QString btnSoundMuteImageHover;
    QString btnPlayModeSingleNormal;
    QString btnPlayModeSingleHover;
    QString btnPlayModeSingleCycleNormal;
    QString btnPlayModeSingleCycleHover;

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
    QString blueButtonColorDisabled;  //#2e4e7e

    QString btnPackbackToBoxImg;        //":/resource/image/btn_pack_back_to_box_black.png"

    //特殊按钮样式
    QString imgViewMusicNormal;
    QString imgViewMusicHover;
    QString imgListenMusicNormal;
    QString imgListenMusicHover;
    QString imgDownloadMusicNormal;
    QString imgDownloadMusicHover;

    QString imgDeleteListItemNormal;
    QString imgDeleteListItemHover;
    QString imgEditListItemNormal;
    QString imgEditListItemHover;

    QString imgAddListNormal;
    QString imgAddListHover;
    QString imgToggleListNormal;
    QString imgToggleListHover;
    QString imgToggleListCheckedNormal;
    QString imgToggleListCheckedHover;


    //QList
    QString listItemNormalColor;     //#c0c0c0

    //QSlider
    QString baseColor;                  //#CC0033
    QString baseColorLighter;           //#ff3344

    //QCheckBox
    QString checkbox_unchecked;
    QString checkbox_checked;

    //其他
    QString itemSelectBackground;
    QString editBackground;


    //纯色动态配色值
protected:

    void initPureColor(QString colorStr)
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

    void setPureColor()
    {
        btnTabLeftBoderColor       =colorString;
        topContainerBgColor        =colorString;
        playBgColor                =colorLighter;
        playHoverColor             =colorString;

        //QSlider
        baseColor                 =colorString;
        baseColorLighter          =colorLighter;
    }

    QString colorString;        //主题颜色对应字符串
    QString colorLighter;       //比主题颜色稍微轻一点的颜色
};

#endif // ISKIN_H
