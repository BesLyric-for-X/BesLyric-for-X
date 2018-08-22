#include "TopWidget.h"
#include <QHBoxLayout>

TopWidget::TopWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
    initLayout();
    initConnection();
}

TopWidget::~TopWidget()
{

}

void TopWidget::initLayout()
{
    QWidget* topWidgetContainer = new QWidget(this);
    topWidgetContainer->setObjectName("topWidgetContainer");
    topWidgetContainer->setMouseTracking(true);

    //icon = new QIcon(":/resource/image/netease_cloudmusic_48px.ico");
    btnIcon = new BesButton(topWidgetContainer);
    btnIcon->setIconSize(QSize(150,30));
    btnIcon->setObjectName("btnIcon");

    labelTitle = new QLabel(topWidgetContainer);
    labelTitle->setText("一款专门制作网易云滚动歌词的软件");
    labelTitle->setObjectName("labelTitle");

    btnSkinBox = new BesButton(topWidgetContainer);
    btnSetting = new BesButton(topWidgetContainer);
    btnMini = new BesButton(topWidgetContainer);
    btnMax = new BesButton(topWidgetContainer);
    btnRestore = new BesButton(topWidgetContainer);
    btnClose = new BesButton(topWidgetContainer);

    btnRestore->setVisible(false);

    btnSkinBox->setObjectName("btnSkinBox");
    btnSetting->setObjectName("btnSetting");

    QLabel* labelSepHeader = new QLabel(topWidgetContainer);
    labelSepHeader->setObjectName("labelSepHeader");
    labelSepHeader->setText("|");

    btnMini->setObjectName("btnMini");
    btnMax->setObjectName("btnMax");
    btnRestore->setObjectName("btnRestore");
    btnClose->setObjectName("btnClose");

    QHBoxLayout*    hLayout = new QHBoxLayout(topWidgetContainer);
    hLayout->addWidget(btnIcon);
    hLayout->addSpacerItem(new QSpacerItem(60,25,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(labelTitle);
    hLayout->addSpacerItem(new QSpacerItem(23,25,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout->addWidget(btnSkinBox);
    hLayout->addSpacerItem(new QSpacerItem(23,25,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnSetting);
    hLayout->addSpacerItem(new QSpacerItem(16,25,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(labelSepHeader);
    hLayout->addSpacerItem(new QSpacerItem(15,25,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnMini);
    hLayout->addWidget(btnMax);
    hLayout->addWidget(btnRestore);
    hLayout->addWidget(btnClose);
    hLayout->addSpacerItem(new QSpacerItem(10,25,QSizePolicy::Fixed,QSizePolicy::Fixed));

    QHBoxLayout*  hLayoutWrapper = new QHBoxLayout(this);
    hLayoutWrapper->setMargin(0);
    hLayoutWrapper->addWidget(topWidgetContainer);

}

void TopWidget::initConnection()
{
}

