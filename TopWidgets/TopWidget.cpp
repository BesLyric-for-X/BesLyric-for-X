#include "TopWidget.h"
#include <QHBoxLayout>
#include "BesScaleUtil.h"

TopWidget::TopWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
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
    topWidgetContainer->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    btnIcon = new BesButton(topWidgetContainer);
    btnIcon->setIconSize(QSize(150,30));
    btnIcon->setObjectName("btnIcon");
    btnIcon->setFocusPolicy(Qt::NoFocus);
    btnIcon->setToolTip(tr("返回制作歌词主页"));

    labelTitle = new QLabel(topWidgetContainer);
    labelTitle->setText("一款专门制作网易云滚动歌词的软件");
    labelTitle->setObjectName("labelTitle");

    btnSkinBox = new BesButton(topWidgetContainer);
    btnSetting = new BesButton(topWidgetContainer);
    btnMini = new BesButton(topWidgetContainer);
    btnMax = new BesButton(topWidgetContainer);
    btnRestore = new BesButton(topWidgetContainer);
    btnClose = new BesButton(topWidgetContainer);

    btnSkinBox->setFocusPolicy(Qt::NoFocus);
    btnSetting->setFocusPolicy(Qt::NoFocus);
    btnMini->setFocusPolicy(Qt::NoFocus);
    btnMax->setFocusPolicy(Qt::NoFocus);
    btnRestore->setFocusPolicy(Qt::NoFocus);
    btnClose->setFocusPolicy(Qt::NoFocus);

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
    hLayout->addSpacerItem(new QSpacerItem(60 * BesScaleUtil::scale(),25,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(labelTitle);
    hLayout->addSpacerItem(new QSpacerItem(23 * BesScaleUtil::scale(),25,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout->addWidget(btnSkinBox);
    hLayout->addSpacerItem(new QSpacerItem(23 * BesScaleUtil::scale(),25,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(btnSetting);
    hLayout->addSpacerItem(new QSpacerItem(16 * BesScaleUtil::scale(),25,QSizePolicy::Fixed,QSizePolicy::Fixed));
    hLayout->addWidget(labelSepHeader);
    hLayout->addSpacerItem(new QSpacerItem(15 * BesScaleUtil::scale(),25,QSizePolicy::Fixed,QSizePolicy::Fixed));
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

