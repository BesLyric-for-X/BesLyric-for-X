
#include "BesListHeader.h"
#include <QHBoxLayout>
#include <QSpacerItem>

BesListHeader::BesListHeader(QWidget *parent):
    QWidget(parent),labelListTitle(nullptr), showAddButton(false),showToggleButton(true)
{
    initAll();
}

BesListHeader::BesListHeader(QString title, bool bShowAddButton, bool bShowToggleButton, QWidget *parent):QWidget(parent)
{
    initAll();
    setHeader(title,bShowAddButton,bShowToggleButton);
}

void BesListHeader::setHeader(QString title, bool bShowAddButton, bool bShowToggleButton)
{
    labelListTitle->setText(title);
    showAddButton = bShowAddButton;
    showToggleButton = bShowToggleButton;

    updateVisibleStatus();
}

void BesListHeader::initAll()
{
    this->setMouseTracking(true);
    initEntity();
    initLayout();
    initConnection();
}

void BesListHeader::initEntity()
{

}

void BesListHeader::initLayout()
{
    labelListTitle = new QLabel(this);
    btnAddItem = new BesButton(this);
    btnToggleList= new BesButton(this);

    labelListTitle->setObjectName("labelListTitle");
    btnAddItem->setObjectName("btnAddItem");
    btnToggleList->setObjectName("btnToggleList");

    btnToggleList->setCheckable(true);
    btnToggleList->setChecked(false);

    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
    hLayout->addSpacerItem(new QSpacerItem(15,20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hLayout->addWidget(labelListTitle);
    hLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));
    hLayout->addWidget(btnAddItem);
    hLayout->addWidget(btnToggleList);

    updateVisibleStatus();
}

void BesListHeader::initConnection()
{
    connect(btnToggleList, &QPushButton::toggled, [=](bool checked){
        emit sig_toggle_list(checked);}
    );

}

void BesListHeader::updateVisibleStatus()
{
    btnAddItem->setVisible(showAddButton);
    btnToggleList->setVisible(showToggleButton);
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
