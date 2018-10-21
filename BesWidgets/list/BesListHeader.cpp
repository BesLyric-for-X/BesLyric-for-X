
#include "BesListHeader.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include "BesScaleUtil.h"

BesListHeader::BesListHeader(QWidget *parent):
    QWidget(parent),labelListHeaderTitle(nullptr), showAddButton(false),showToggleButton(true)
{
    //this->setMouseTracking(true);
    initAll();
}

BesListHeader::BesListHeader(QString title, bool bShowAddButton, bool bShowToggleButton, QWidget *parent):QWidget(parent)
{
    initAll();
    setHeader(title,bShowAddButton,bShowToggleButton);
}

void BesListHeader::setHeader(QString title, bool bShowAddButton, bool bShowToggleButton)
{
    labelListHeaderTitle->setText(title);
    showAddButton = bShowAddButton;
    showToggleButton = bShowToggleButton;

    updateVisibleStatus();
}

void BesListHeader::initAll()
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    initEntity();
    initLayout();
    initConnection();
}

void BesListHeader::initEntity()
{

}

void BesListHeader::initLayout()
{
    labelListHeaderTitle = new QLabel(this);
    btnAddItem = new BesButton(this);
    btnToggleList= new BesButton(this);

    labelListHeaderTitle->setObjectName("labelListHeaderTitle");
    btnAddItem->setObjectName("btnAddItem");
    btnToggleList->setObjectName("btnToggleList");

//    labelListHeaderTitle->setMinimumSize(100* BesScaleUtil::scale(),30);
//    labelListHeaderTitle->setMaximumSize(100* BesScaleUtil::scale(),30);
//    labelListHeaderTitle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    btnToggleList->setCheckable(true);
    btnToggleList->setChecked(false);

    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    hLayout->addSpacerItem(new QSpacerItem(15* BesScaleUtil::scale(),20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout->addWidget(labelListHeaderTitle);
    hLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    hLayout->addWidget(btnAddItem);
    hLayout->addWidget(btnToggleList);

    updateVisibleStatus();
}

void BesListHeader::initConnection()
{
    connect(btnToggleList, &QPushButton::toggled, [=](bool checked){
        emit sig_toggleList(checked);}
    );

    connect(btnAddItem, &QPushButton::clicked, [=](){
        emit sig_addButtonClicked();}
    );
}

void BesListHeader::updateVisibleStatus()
{
    btnAddItem->setVisible(showAddButton);
    btnToggleList->setVisible(showToggleButton);
}

void BesListHeader::OnMakeSureHeaderChecking()
{
    if(!btnToggleList->isChecked())
        btnToggleList->setChecked(true);
}



void BesListHeader::enterEvent(QEvent *event)
{
    setCursor(QCursor(Qt::PointingHandCursor));
    QWidget::enterEvent(event);
}

void BesListHeader::leaveEvent(QEvent *event)
{
    unsetCursor();
    QWidget::leaveEvent(event);
}

void BesListHeader::mousePressEvent(QMouseEvent *event)
{
    btnToggleList->toggle();
    QWidget::mousePressEvent(event);

}
