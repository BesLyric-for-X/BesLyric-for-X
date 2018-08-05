#include "SkinBoxWidget.h"
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPixmap>

SkinBoxWidget::SkinBoxWidget(QWidget *parent)
    : BesShadowWidget(parent),bLastSelectPage(1)
{
    BesShadowWidget::SetMarginTop(6);
    BesShadowWidget::SetBackgroundColor(QColor(0,0,0,0));

    initLayout();
    initConnection();
    initSliderGrooveColor();
}

SkinBoxWidget::~SkinBoxWidget()
{

}

void SkinBoxWidget::initLayout()
{
    frontLayer = new QWidget(this);
    mainLayer = new QWidget(this);
    frontLayer->setObjectName("skinBoxFrontLayer");
    mainLayer->setObjectName("skinBoxMainLayer");

    btnCheckMark = new BesButton(frontLayer); //具体位置交由paintEvent绘制
    btnCheckMark->setObjectName("btnCheckMark");

    QWidget* themeContainer = new QWidget(mainLayer);
    QWidget* pureColorContainer = new QWidget(mainLayer);

    btnTheme = new BesButton(mainLayer);
    btnPureColor = new BesButton(mainLayer);
    btnTheme->setObjectName("btnTheme");
    btnPureColor->setObjectName("btnPureColor");

    btnTheme->setCheckable(true);               btnTheme->setChecked(true);
    btnPureColor->setCheckable(true);
    btnTheme->setAutoExclusive(true);
    btnPureColor->setAutoExclusive(true);

    btnTheme->setText(tr("主题"));
    btnPureColor->setText(tr("纯色"));

    QHBoxLayout* layoutButtons = new QHBoxLayout();
    layoutButtons->addWidget(btnTheme);
    layoutButtons->addWidget(btnPureColor);
    layoutButtons->addSpacerItem(new QSpacerItem(20,25,
                              QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    //
    btnBlack = new ButtonTheme(themeContainer, new QImage(":/resource/image/酷炫黑.png"),"black",tr("酷炫黑"),QColor("#555555"));
    btnRed = new ButtonTheme(themeContainer, new QImage(":/resource/image/官方红.png"),"red",tr("官方红"),QColor("#e83c3c"));
    btnPink = new ButtonTheme(themeContainer, new QImage(":/resource/image/可爱粉.png"),"pink",tr("可爱粉"),QColor("#ff87b4"));
    btnBlue = new ButtonTheme(themeContainer, new QImage(":/resource/image/天际蓝.png"),"blue",tr("天际蓝"),QColor("#66b7ff"));
    btnGreen = new ButtonTheme(themeContainer, new QImage(":/resource/image/清新绿.png"),"green",tr("清新绿"),QColor("#5dc78a"));
    btnGold = new ButtonTheme(themeContainer, new QImage(":/resource/image/土豪金.png"),"gold",tr("土豪金"),QColor("#faac62"));

    btnBlack->setMinimumSize(116,116);
    btnBlack->setMaximumSize(116,116);
    btnRed->setMinimumSize(116,116);
    btnRed->setMaximumSize(116,116);
    btnPink->setMinimumSize(116,116);
    btnPink->setMaximumSize(116,116);
    btnBlue->setMinimumSize(116,116);
    btnBlue->setMaximumSize(116,116);
    btnGreen->setMinimumSize(116,116);
    btnGreen->setMaximumSize(116,116);
    btnGold->setMinimumSize(116,116);
    btnGold->setMaximumSize(116,116);
    btnBlack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnRed->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPink->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnBlue->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnGreen->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnGold->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    btnBlack->setObjectName("btnBlack");
    btnRed->setObjectName("btnRed");
    btnPink->setObjectName("btnPink");
    btnBlue->setObjectName("btnBlue");
    btnGreen->setObjectName("btnGreen");
    btnGold->setObjectName("btnGold");

    QGridLayout* themeGridLayout = new QGridLayout();
    themeGridLayout->setContentsMargins(12,8,12,18);
    themeGridLayout->setSpacing(6);
    themeGridLayout->addWidget(btnBlack,0,0);
    themeGridLayout->addWidget(btnRed,0,1);
    themeGridLayout->addWidget(btnPink,0,2);
    themeGridLayout->addWidget(btnBlue,1,0);
    themeGridLayout->addWidget(btnGreen,1,1);
    themeGridLayout->addWidget(btnGold,1,2);

    themeContainer->setLayout(themeGridLayout);                 //主题页面

    //12种预选纯色
    btnPureColor1 = new ButtonPureColor(pureColorContainer ,"#ff5c8a");
    btnPureColor2 = new ButtonPureColor(pureColorContainer ,"#ff7a9e");
    btnPureColor3 = new ButtonPureColor(pureColorContainer ,"#fe76c8");
    btnPureColor4 = new ButtonPureColor(pureColorContainer ,"#717ff9");
    btnPureColor5 = new ButtonPureColor(pureColorContainer ,"#4791eb");
    btnPureColor6 = new ButtonPureColor(pureColorContainer ,"#39afea");
    btnPureColor7 = new ButtonPureColor(pureColorContainer ,"#2bb669");
    btnPureColor8 = new ButtonPureColor(pureColorContainer ,"#6acc19");
    btnPureColor9 = new ButtonPureColor(pureColorContainer ,"#e2ab12");
    btnPureColor10 = new ButtonPureColor(pureColorContainer,"#ff8f57");
    btnPureColor11 = new ButtonPureColor(pureColorContainer,"#fd726d");
    btnPureColor12 = new ButtonPureColor(pureColorContainer,"#fd544e");

    btnPureColor1 ->setObjectName("btnPureColor1");
    btnPureColor2 ->setObjectName("btnPureColor2");
    btnPureColor3 ->setObjectName("btnPureColor3");
    btnPureColor4 ->setObjectName("btnPureColor4");
    btnPureColor5 ->setObjectName("btnPureColor5");
    btnPureColor6 ->setObjectName("btnPureColor6");
    btnPureColor7 ->setObjectName("btnPureColor7");
    btnPureColor8 ->setObjectName("btnPureColor8");
    btnPureColor9 ->setObjectName("btnPureColor9");
    btnPureColor10->setObjectName("btnPureColor10");
    btnPureColor11->setObjectName("btnPureColor11");
    btnPureColor12->setObjectName("btnPureColor12");

    btnPureColor1->setMinimumSize(55,55);
    btnPureColor2->setMinimumSize(55,55);
    btnPureColor3->setMinimumSize(55,55);
    btnPureColor4->setMinimumSize(55,55);
    btnPureColor5->setMinimumSize(55,55);
    btnPureColor6->setMinimumSize(55,55);
    btnPureColor7->setMinimumSize(55,55);
    btnPureColor8->setMinimumSize(55,55);
    btnPureColor9->setMinimumSize(55,55);
    btnPureColor10->setMinimumSize(55,55);
    btnPureColor11->setMinimumSize(55,55);
    btnPureColor12->setMinimumSize(55,55);

    btnPureColor1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor3->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor4->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor5->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor6->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor7->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor8->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor9->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor10->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor11->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnPureColor12->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QGridLayout* pureColorGridLayout = new QGridLayout();
    pureColorGridLayout->addWidget(btnPureColor1,0,0);
    pureColorGridLayout->addWidget(btnPureColor2,0,1);
    pureColorGridLayout->addWidget(btnPureColor3,0,2);
    pureColorGridLayout->addWidget(btnPureColor4,0,3);
    pureColorGridLayout->addWidget(btnPureColor5,0,4);
    pureColorGridLayout->addWidget(btnPureColor6,0,5);
    pureColorGridLayout->addWidget(btnPureColor7,1,0);
    pureColorGridLayout->addWidget(btnPureColor8,1,1);
    pureColorGridLayout->addWidget(btnPureColor9,1,2);
    pureColorGridLayout->addWidget(btnPureColor10,1,3);
    pureColorGridLayout->addWidget(btnPureColor11,1,4);
    pureColorGridLayout->addWidget(btnPureColor12,1,5);

    labelCustomizeColor = new QLabel(pureColorContainer);
    btnCustomizeColor=  new BesButton(pureColorContainer);
    SliderHue=  new QSlider(pureColorContainer);
    SliderLightness=  new QSlider(pureColorContainer);

    labelCustomizeColor->setText(tr("自定义颜色"));
    btnCustomizeColor->setMinimumSize(55,55);
    btnCustomizeColor->setMaximumSize(55,55);
    btnCustomizeColor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnCustomizeColor->setObjectName("btnCustomizeColor");

    SliderHue->setOrientation(Qt::Horizontal);
    SliderLightness->setOrientation(Qt::Horizontal);
    SliderHue->setRange(0,359);
    SliderLightness->setRange(20,170);

    SliderHue->setObjectName("SliderHue");
    SliderLightness->setObjectName("SliderLightness");

    SliderHue->setMinimumHeight(25);
    SliderLightness->setMinimumHeight(25);
    SliderHue->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    SliderLightness->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    QVBoxLayout * vSliderLayout = new QVBoxLayout();
    vSliderLayout->setSpacing(10);
    vSliderLayout->addWidget(SliderHue);
    vSliderLayout->addWidget(SliderLightness);

    QHBoxLayout * hCustomizeColorLayout = new QHBoxLayout();
    hCustomizeColorLayout->setSpacing(12);
    hCustomizeColorLayout->addWidget(btnCustomizeColor);
    hCustomizeColorLayout->addLayout(vSliderLayout);

    QVBoxLayout * vPureColorLayout = new QVBoxLayout();     //纯色页面整体布局
    vPureColorLayout->addLayout(pureColorGridLayout);
    vPureColorLayout->addSpacerItem(new QSpacerItem(20,20, QSizePolicy::Fixed,QSizePolicy::Fixed));
    vPureColorLayout->addWidget(labelCustomizeColor);
    vPureColorLayout->addLayout(hCustomizeColorLayout);
    vPureColorLayout->addSpacerItem(new QSpacerItem(20,8, QSizePolicy::Fixed,QSizePolicy::Fixed));

    pureColorContainer->setLayout(vPureColorLayout);                 //纯色页面

    skinStack = new QStackedLayout();
    skinStack->setStackingMode(QStackedLayout::StackOne);
    skinStack->addWidget(themeContainer);
    skinStack->addWidget(pureColorContainer);
    skinStack->setCurrentIndex(0);

    QVBoxLayout* vMainLayout = new QVBoxLayout(mainLayer);
    vMainLayout->setMargin(2);
    vMainLayout->setSpacing(5);
    vMainLayout->addLayout(layoutButtons);
    vMainLayout->addLayout(skinStack);
}

void SkinBoxWidget::initConnection()
{
    connect(btnTheme,&QPushButton::clicked,[=](){skinStack->setCurrentIndex(0);});
    connect(btnPureColor,&QPushButton::clicked,[=](){skinStack->setCurrentIndex(1);});

    connect(btnTheme,&QPushButton::clicked,[=](){swithToPage(1);});
    connect(btnPureColor,&QPushButton::clicked,[=](){swithToPage(2);});

    connect(btnBlack,&QPushButton::clicked,[=](){   markToPos(true,110+ 121*0 ,155); bLastSelectPage = 1;});
    connect(btnRed,&QPushButton::clicked,[=](){     markToPos(true,110+ 121*1 ,155); bLastSelectPage = 1;});
    connect(btnPink,&QPushButton::clicked,[=](){    markToPos(true,110+ 121*2 ,155);bLastSelectPage = 1;});
    connect(btnBlue,&QPushButton::clicked,[=](){    markToPos(true,110+ 121*0 ,155+122);bLastSelectPage = 1;});
    connect(btnGreen,&QPushButton::clicked,[=](){   markToPos(true,110+ 121*1 ,155+122);bLastSelectPage = 1;});
    connect(btnGold,&QPushButton::clicked,[=](){    markToPos(true,110+ 121*2 ,155+122);bLastSelectPage = 1;});

    connect(btnPureColor1,&QPushButton::clicked,[=](){markToPos(true, 48 + 61 *0, 98); bLastSelectPage = 2;});
    connect(btnPureColor2,&QPushButton::clicked,[=](){markToPos(true, 48 + 61 *1, 98); bLastSelectPage = 2;});
    connect(btnPureColor3,&QPushButton::clicked,[=](){markToPos(true, 48 + 61 *2, 98);bLastSelectPage = 2;});
    connect(btnPureColor4,&QPushButton::clicked,[=](){markToPos(true, 48 + 61 *3, 98);bLastSelectPage = 2;});
    connect(btnPureColor5,&QPushButton::clicked,[=](){markToPos(true, 48 + 61 *4, 98);bLastSelectPage = 2;});
    connect(btnPureColor6,&QPushButton::clicked,[=](){markToPos(true, 48 + 61 *5, 98);bLastSelectPage = 2;});
    connect(btnPureColor7,&QPushButton::clicked,[=](){markToPos(true, 48 + 61 *0, 98+ 61); bLastSelectPage = 2;});
    connect(btnPureColor8,&QPushButton::clicked,[=](){markToPos(true, 48 + 61 *1, 98+ 61); bLastSelectPage = 2;});
    connect(btnPureColor9,&QPushButton::clicked,[=](){markToPos(true, 48 + 61 *2, 98+ 61);bLastSelectPage = 2;});
    connect(btnPureColor10,&QPushButton::clicked,[=](){markToPos(true,48 + 61 *3, 98+ 61);bLastSelectPage = 2;});
    connect(btnPureColor11,&QPushButton::clicked,[=](){markToPos(true,48 + 61 *4, 98+ 61);bLastSelectPage = 2;});
    connect(btnPureColor12,&QPushButton::clicked,[=](){markToPos(true,48 + 61 *5, 98+ 61);bLastSelectPage = 2;});

    //注意：onSkinClick 这里有2处会触发皮肤的变换，一处是这里改变slider触发了纯颜色皮肤设置；另一处是在 StackFrame 中
    // 这里先连接，所以不会影响后面再次设置的自定义皮肤 ：全局搜索【FLAG_SETTING_SKIN】查看相关逻辑
    connect(btnBlack,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnRed,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPink,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnBlue,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnGreen,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnGold,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));

    connect(btnPureColor1,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor2,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor3,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor4,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor5,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor6,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor7,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor8,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor9,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor10,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor11,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));
    connect(btnPureColor12,SIGNAL(onSkinClick(QString)),this,SLOT(changeSliderValueBySkinName(QString)));



    connect(btnCustomizeColor,&QPushButton::clicked,[=](){markToPos(true,48 + 61 *0, 274);bLastSelectPage = 2;});

    connect(SliderHue, SIGNAL(valueChanged(int)),this,SLOT(initLightnessGrooveColorByHue(int)));
    connect(SliderHue, &QSlider::valueChanged,[=](){signalToCustomSkin();});
    connect(SliderLightness, &QSlider::valueChanged,[=](){signalToCustomSkin();});
}

//初始化滑动器的槽的颜色
void SkinBoxWidget::initSliderGrooveColor()
{
    //构造线性颜色stops 字符串
    QString strStops;
    int nSegCount = 8;                  //颜色点数
    for(int i=0; i<= nSegCount; i++)
    {
        double PosistionFactor = 1.0 / nSegCount * i;
        QColor colorFromHsl = QColor::fromHsl(PosistionFactor * 240,240,160);
        QColor colorSetHsl;
        colorSetHsl.setHsl(PosistionFactor * 359,255,170);

        QColor rgbColor = colorFromHsl.toRgb();
        rgbColor = colorSetHsl.toRgb();

        int r,g,b;
        rgbColor.getRgb(&r,&g,&b);

        QString strStop;
        strStop.sprintf("stop:%.3f rgb(%d,%d,%d)", PosistionFactor,r,g,b);

        if(i != nSegCount)
            strStop += ",";

        strStops += strStop;
    }


    //色调滑动器
    SliderHue->setStyleSheet(
    "QSlider#SliderHue::groove:horizontal {"
    "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, " + strStops + "); }");

}

//切换为自定义的颜色皮肤
void  SkinBoxWidget::signalToCustomSkin()
{
    //获得当前色调和亮度
    int h = SliderHue->value();
    int l = SliderLightness->value();
    QColor currentColor = QColor::fromHsl(h,255,l);

    int r,g,b;
    currentColor.getRgb(&r,&g,&b);

    QString colorStr;
    colorStr.sprintf("#%.2x%.2x%.2x",r,g,b);
    emit(signalSetCustomSkin(colorStr));
}



void SkinBoxWidget::swithToPage(int nIndex)
{
    if(bLastSelectPage != nIndex)
        markToPos(false,0,0);
    else
        markToPos(true,lastMarkPosX,lastMarkPosY);
}

void SkinBoxWidget::markToPos(bool bFront, int x, int y)
{
    if(bFront)
    {
        frontLayer->setVisible(true);
        frontLayer->setGeometry(x,y,33,33);
        lastMarkPosX = x;
        lastMarkPosY = y;
        frontLayer->raise();
    }
    else
    {
        frontLayer->setVisible(false);
        mainLayer->raise();
    }
}

 //设置最终使用的皮肤名
void SkinBoxWidget::setFinalSkinName(QString skinName)
{
    finalSkinName = skinName;
}

 //仅仅通过皮肤名，改变slider.皮肤名是 预定义的6种主题颜色，以及#xxxxxx 字符串
void SkinBoxWidget::changeSliderValueBySkinName(QString skinName)
{
    QString colorString;

    if(skinName.contains("#"))
        colorString = skinName;
    else if(skinName == "black")
        colorString = "#222222";
    else if(skinName == "red")
        colorString = "#c62f2f";
    else if(skinName == "pink")
        colorString = "#fa7aa7";
    else if(skinName == "blue")
        colorString = "#44aaf8";
    else if(skinName == "green")
        colorString = "#3bba7d";
    else if(skinName == "gold")
        colorString = "#faac62";
    else
        colorString = "#222222";

    QColor color(colorString);

    int h,s,l;
    color.getHsl(&h,&s,&l);

    SliderHue->setValue(h);
    SliderLightness->setValue(l);
}


//基于指定色调 hueValue， 设置亮度颜色
void SkinBoxWidget::initLightnessGrooveColorByHue(int hueValue)
{
    QColor lightLow = QColor::fromHsl(hueValue,255,20);
    QColor lightHeight =QColor::fromHsl(hueValue,255,170);

    int r1,g1,b1;
    int r2,g2,b2;
    lightLow.getRgb(&r1,&g1,&b1);
    lightHeight.getRgb(&r2,&g2,&b2);

    QString colorStr = QString("qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgb(%1,%2,%3), stop:1 rgb(%4,%5,%6))").arg(r1).arg(g1).arg(b1).arg(r2).arg(g2).arg(b2);
    SliderLightness->setStyleSheet("QSlider#SliderLightness::groove:horizontal {"
                                   "background:"+ colorStr+ " ; }");
}

void SkinBoxWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::transparent);

    QPixmap indicatorPixmap;
    QColor LineColor;
    QColor BackgroundColor;

    if(finalSkinName == "black")
    {
        indicatorPixmap = QPixmap(":/resource/image/box_indicator_black.png");
        BackgroundColor = QColor("#2d2f33");
        LineColor=QColor("#3a3c40");
    }
    else
    {
        indicatorPixmap = QPixmap(":/resource/image/box_indicator_white.png");
        BackgroundColor = QColor("#fafafa");
        LineColor=QColor("#e1e1e1");
    }

    p.setBrush(BackgroundColor);//刷背景
    QRect mainLayerRect = QRect(8 ,8 + marginTop, this->width()- 16, this->height()-16 - marginTop);
    p.drawRoundedRect(mainLayerRect,5,5);

    //绘制直线 和 指标
    p.setPen(QPen(LineColor,2));
    p.drawLine(25,btnTheme->geometry().bottom()+ marginTop+border,
               this->width()-25, btnTheme->geometry().bottom()+ marginTop+border);
    p.drawPixmap(this->width()/2 - indicatorPixmap.width()/2, 0 ,indicatorPixmap);  //在中上位置绘制指标

    BesShadowWidget::paintEvent(event);
}

void SkinBoxWidget::resizeEvent(QResizeEvent *event)
{
    BesShadowWidget::resizeEvent(event);

    QRect mainLayerRect = QRect(8 ,8 + marginTop, this->width()- 16, this->height()-16 - marginTop);

    mainLayer->setGeometry(mainLayerRect);

}

