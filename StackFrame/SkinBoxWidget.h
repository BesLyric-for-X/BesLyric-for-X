#ifndef SKINBOXWIDGET_H
#define SKINBOXWIDGET_H

#include "global.h"
#include "BesShadowWidget.h"
#include <QWidget>
#include "BesButton.h"
#include <QSlider>
#include <QLabel>
#include <QImage>
#include <QPainter>
#include <QStackedLayout>

class ButtonTheme: public BesButton
{
    Q_OBJECT
public:
    ButtonTheme(QWidget* parent, QImage* image,QString skinName, QString title,QColor color)
        :BesButton(parent),m_image(image),m_skinName(skinName),m_title(title),m_color(color),m_bHover(false){}

    virtual void paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);


        painter.drawPixmap(2,2,this->width()- 2*2,this->height()- 2*2,
                           QPixmap::fromImage(m_image->scaled(this->width()- 2*2, this->height()- 2*2,
                                              Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
        QWidget::paintEvent(event);

        if(m_bHover)
        {
            QRectF outerRect(0,0,this->width()-1,this->height()-1);
            painter.setPen(m_color);
            painter.drawRect(outerRect);
        }

        QRectF rect;
        rect.setLeft(2);
        rect.setRight(this->width()-2);
        rect.setTop(this->height()- 2 - 25);
        rect.setBottom(this->height()- 2);
        painter.fillRect(rect,QColor(2,0,0,10));

        rect.setLeft(8);
        painter.setPen(QColor("white"));
        QFont font = painter.font();
        font.setFamily("Microsoft YaHei");
        font.setPixelSize(15);
        painter.setFont(font);
        QTextOption option;
        option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        painter.drawText(rect,m_title, option);
    }

    virtual void enterEvent(QEvent *e){m_bHover=true;BesButton::enterEvent(e);}//进入事件
    virtual void leaveEvent(QEvent *e){m_bHover=false;BesButton::leaveEvent(e);}//离开事件

    virtual void mousePressEvent(QMouseEvent* e){emit(onSkinClick(m_skinName));BesButton::mousePressEvent(e);}

signals:
    void onSkinClick(QString name);

private:
    QImage*     m_image;
    QString     m_skinName;
    QString     m_title;
    QColor      m_color;
    bool        m_bHover;
};

class ButtonPureColor: public BesButton
{
    Q_OBJECT
public:
    ButtonPureColor(QWidget* parent,QString colorStr) :BesButton(parent),m_colorStr(colorStr),m_bHover(false){}


    virtual void paintEvent(QPaintEvent* event)
    {
        QWidget::paintEvent(event);

        QPainter painter(this);
        QColor color(m_colorStr);


        if(m_bHover)
        {
            QRectF outerRect(0,0,this->width()-1,this->height()-1);
            painter.setPen(color);
            painter.drawRect(outerRect);
        }

        QRectF rect;
        rect.setLeft(2);
        rect.setRight(this->width()-2);
        rect.setTop(2);
        rect.setBottom(this->height()- 2);
        painter.fillRect(rect,color);
    }

    virtual void enterEvent(QEvent *e){m_bHover=true;BesButton::enterEvent(e);}//进入事件
    virtual void leaveEvent(QEvent *e){m_bHover=false;BesButton::leaveEvent(e);}//离开事件

    virtual void mousePressEvent(QMouseEvent* e){emit(onSkinClick(m_colorStr));BesButton::mousePressEvent(e);}

signals:
    void onSkinClick(QString name);
private:
    QString     m_colorStr;
    bool        m_bHover;
};

class SkinBoxWidget : public BesShadowWidget
{
    Q_OBJECT

public:
    SkinBoxWidget(QWidget *parent = 0);
    ~SkinBoxWidget();

    void initLayout();
    void initConnection();

    void initSliderGrooveColor(); //初始化滑动器的槽的颜色
    void signalToCustomSkin();    //发信号切换为自定义的颜色皮肤

    void swithToPage(int nIndex);
    void markToPos(bool bFront, int x, int y);

    void setFinalSkinName(QString);                     //设置最终使用的皮肤名

signals:
    void signalSetCustomSkin(QString colorString);

public slots:
    void changeSliderValueBySkinName(QString);          //仅仅通过皮肤名，改变slider.皮肤名是 预定义的6种主题颜色，以及#xxxxxx 字符串
    void initLightnessGrooveColorByHue(int hueValue);   //基于指定色调 hueValue， 设置亮度颜色

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *event);

private:
    int  bLastSelectPage;
    int  lastMarkPosX;
    int  lastMarkPosY;

    QString  finalSkinName;         //储存最后真正使用的皮肤，用于区别绘制skinBox背景

public:
    QWidget*        frontLayer;
    QWidget*        mainLayer;

    QStackedLayout* skinStack;

    BesButton*      btnCheckMark;

    BesButton*      btnTheme;
    BesButton*      btnPureColor;

    ButtonTheme*    btnBlack;
    ButtonTheme*    btnRed;
    ButtonTheme*    btnPink;
    ButtonTheme*    btnBlue;
    ButtonTheme*    btnGreen;
    ButtonTheme*    btnGold;

    /*
//6种主题的配色(↑ 上侧，↓ 下侧 ← 左侧 → 右侧 )
red		↑	c62f2f		↓	e83c3c
pink 	↑	fa7aa7		↓	ff87b4
gold					↓	faac62
green	↑	3bba7d		↓	5dc78a
blue	↑	44aaf8		↓	66b7ff

black	←	191b1f		→	16181c    ↓	222222

//除了黑色外的其他5种主题的白色配色
白左←：f5f5f7
白下↓：f6f6f8

     */

    ButtonPureColor*    btnPureColor1;
    ButtonPureColor*    btnPureColor2;
    ButtonPureColor*    btnPureColor3;
    ButtonPureColor*    btnPureColor4;
    ButtonPureColor*    btnPureColor5;
    ButtonPureColor*    btnPureColor6;
    ButtonPureColor*    btnPureColor7;
    ButtonPureColor*    btnPureColor8;
    ButtonPureColor*    btnPureColor9;
    ButtonPureColor*    btnPureColor10;
    ButtonPureColor*    btnPureColor11;
    ButtonPureColor*    btnPureColor12;

    /*
        这里12个颜色，通过 ubuntu 下gimp工具取色得到值依次为：

        12种颜色
        #ff5c8a
        #ff7a9e
        #fe76c8
        #717ff9
        #4791eb
        #39afea

        #2bb669
        #6acc19
        #e2ab12
        #ff8f57
        #fd726d
        #fd544e
     */

    QLabel*         labelCustomizeColor;
    BesButton*      btnCustomizeColor;
    QSlider*        SliderHue;              //色调
    QSlider*        SliderLightness;        //亮度

    /*
        颜色可以通过GRB表示，也可以通过 H（hue，色调）S（Saturation,饱和度）L（Lightness,亮度）表示
        关于颜色的知识详细看这里：https://blog.csdn.net/bjbz_cxy/article/details/79701006

        RGB 3个维度范围是 0~255
        HSL 3个维度范围是 0~240(mspaint 中的范围)； qt 三个维度范围分别是：The value of s, l, and a must all be in the range 0-255; the value of h must be in the range 0-359.
        通过 mspaint.exe 调色板调试发现，网易云这里的2个滑动条，可能使用2个维度 色调 和 亮度，另外的饱和度固定为240（饱和度越接近0，越是感受不到颜色的）

        上面一个滑动条 是色调 可能是 0~240 （qt 这里 h 的范围为 0~359）
        下面一个滑动条 是亮度 范围可能是 40~160 (qt 这里的 l 的范围大概是 42~170)
    */
};

#endif // SKINBOXWIDGET_H
