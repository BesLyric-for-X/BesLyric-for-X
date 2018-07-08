#include "TopWidget.h"
#include <QHBoxLayout>

TopWidget::TopWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
    initLayout();
    connectAll();
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
    //btnIcon->setIcon(*icon);
    btnIcon->setIconSize(QSize(150,35));
    btnIcon->setObjectName("btnIcon");

    labelTitle = new QLabel(topWidgetContainer);
    labelTitle->setText("Beslyric        一款专门做网易云音乐滚动歌词的软件");
    labelTitle->setObjectName("labelTitle");

    btnSkinBox = new BesButton(topWidgetContainer);
    btnSetting = new BesButton(topWidgetContainer);
    btnMini = new BesButton(topWidgetContainer);
    btnMax = new BesButton(topWidgetContainer);
    btnRestore = new BesButton(topWidgetContainer);
    btnClose = new BesButton(topWidgetContainer);

    btnSkinBox->setObjectName("btnSkinBox");
    btnSetting->setObjectName("btnSetting");
    btnMini->setObjectName("btnMini");
    btnMax->setObjectName("btnMax");
    btnRestore->setObjectName("btnRestore");
    btnClose->setObjectName("btnRestore");

    QHBoxLayout*    hLayout = new QHBoxLayout(topWidgetContainer);
    hLayout->addWidget(btnIcon);
    hLayout->addWidget(labelTitle);
    hLayout->addWidget(labelTitle);
    hLayout->addSpacerItem(new QSpacerItem(20,25,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout->addWidget(btnSkinBox);
    hLayout->addWidget(btnSetting);
    hLayout->addWidget(btnMini);
    hLayout->addWidget(btnMax);
    hLayout->addWidget(btnRestore);
    hLayout->addWidget(btnClose);

    QHBoxLayout*  hLayoutWrapper = new QHBoxLayout(this);
    hLayoutWrapper->setMargin(0);
    hLayoutWrapper->addWidget(topWidgetContainer);

}

void TopWidget::connectAll()
{

}
