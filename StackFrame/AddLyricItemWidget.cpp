#include "AddLyricItemWidget.h"
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPixmap>
#include "BesScaleUtil.h"

AddLyricItemWidget::AddLyricItemWidget(QWidget *parent)
    : BesShadowWidget(parent)
{
    BesShadowWidget::SetMarginTop(6);
    BesShadowWidget::SetBackgroundColor(QColor(0,0,0,0));

    initLayout();
    initConnection();
}

AddLyricItemWidget::~AddLyricItemWidget()
{

}

void AddLyricItemWidget::initLayout()
{
    labelAddItemNameTip = new QLabel(this);
    labelAddItemNameTip->setObjectName("labelAddItemNameTip");
    labelAddItemNameTip->setText(tr("新建歌词单"));
    labelAddItemNameTip->setMinimumHeight(45* BesScaleUtil::mscale());
    labelAddItemNameTip->setMaximumHeight(45* BesScaleUtil::mscale());
    labelAddItemNameTip->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    editAddItemName = new QLineEdit(this);
    editAddItemName->setObjectName("editAddItemName");
    editAddItemName->setMinimumHeight(40* BesScaleUtil::mscale());
    editAddItemName->setMaximumHeight(40* BesScaleUtil::mscale());
    editAddItemName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    btnAddListItem = new BesButton(this);
    btnCancelListItem= new BesButton(this);
    btnAddListItem->setText(tr("创建"));
    btnCancelListItem->setText(tr("取消"));
    btnAddListItem->setObjectName("btnAddListItem");
    btnCancelListItem->setObjectName("btnCancelListItem");
    btnAddListItem->setEnabled(false);

    btnAddListItem->setMinimumHeight(40* BesScaleUtil::mscale());
    btnAddListItem->setMaximumHeight(40* BesScaleUtil::mscale());
    btnAddListItem->setMinimumWidth(100* BesScaleUtil::mscale());
    btnAddListItem->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    btnCancelListItem->setMinimumHeight(40* BesScaleUtil::mscale());
    btnCancelListItem->setMaximumHeight(40* BesScaleUtil::mscale());
    btnCancelListItem->setMinimumWidth(100* BesScaleUtil::mscale());
    btnCancelListItem->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);


    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout->addWidget(btnAddListItem);
    hLayout->addWidget(btnCancelListItem);
    hLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));

    QVBoxLayout* vMainLayout = new QVBoxLayout(this);
    vMainLayout->setMargin(32* BesScaleUtil::mscale());
    vMainLayout->setSpacing(25* BesScaleUtil::mscale());
    vMainLayout->addWidget(labelAddItemNameTip);
    vMainLayout->addWidget(editAddItemName);
    vMainLayout->addLayout(hLayout);
}

void AddLyricItemWidget::initConnection()
{
    connect(editAddItemName, &QLineEdit::textChanged, this, &AddLyricItemWidget::OnTextChanged);

    connect(btnCancelListItem ,&QPushButton::clicked,[=](){ emit sig_toggleAddWidget();});
    connect(btnAddListItem,&QPushButton::clicked,[=](){
        emit sig_addListItem(editAddItemName->text().trimmed());
        emit sig_toggleAddWidget();
        editAddItemName->setText("");
    });


//    connect(btnPureColor,&QPushButton::clicked,[=](){skinStack->setCurrentIndex(1);});

//    connect(btnTheme,&QPushButton::clicked,[=](){swithToPage(1);});
//    connect(btnPureColor,&QPushButton::clicked,[=](){swithToPage(2);});

//    connect(btnCustomizeColor,&QPushButton::clicked,[=](){markToPos(true,48 + 61 *0, 274);bLastSelectPage = 2;});

//    connect(SliderHue, SIGNAL(valueChanged(int)),this,SLOT(initLightnessGrooveColorByHue(int)));
//    connect(SliderHue, &QSlider::valueChanged,[=](){signalToCustomSkin();});
//    connect(SliderLightness, &QSlider::valueChanged,[=](){signalToCustomSkin();});
}

 //设置最终使用的皮肤名
void AddLyricItemWidget::setFinalSkinName(QString skinName)
{
    finalSkinName = skinName;
}

void AddLyricItemWidget::OnTextChanged(QString text)
{
    text = text.trimmed();
    if(text.size() == 0)
        btnAddListItem->setEnabled(false);
    else
        btnAddListItem->setEnabled(true);
}


void AddLyricItemWidget::paintEvent(QPaintEvent *event)
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

//    //绘制直线 和 指标
//    p.setPen(QPen(LineColor,2));
//    p.drawLine(25,btnTheme->geometry().bottom()+ marginTop+border,
//               this->width()-25, btnTheme->geometry().bottom()+ marginTop+border);
//    p.drawPixmap(this->width()/2 - indicatorPixmap.width()/2, 0 ,indicatorPixmap);  //在中上位置绘制指标

    BesShadowWidget::paintEvent(event);
}

void AddLyricItemWidget::resizeEvent(QResizeEvent *event)
{
    BesShadowWidget::resizeEvent(event);

//    QRect mainLayerRect = QRect(8 ,8 + marginTop, this->width()- 16, this->height()-16 - marginTop);

//    mainLayer->setGeometry(mainLayerRect);

}

