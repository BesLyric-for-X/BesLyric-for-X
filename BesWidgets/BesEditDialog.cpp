#include "BesEditDialog.h"
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QPixmap>
#include "BesScaleUtil.h"

BesEditDialog::BesEditDialog(QWidget *parent)
    : BesFramelessDialog(parent)
{
    this->setMinimumHeight(300* BesScaleUtil::mscale());
    this->setMinimumWidth(600* BesScaleUtil::mscale());

    initLayout();
    initConnection();
}

BesEditDialog::~BesEditDialog()
{

}


void BesEditDialog::setText(QString text)
{
    strContent = text;

    //根据字符数变换窗口长度，最小600
    int length = 10 * text.size();
    if(length < 600)
        length = 600;
    this->setMinimumWidth(length * BesScaleUtil::mscale());

    editAddItemName->setText(strContent);
}

QString BesEditDialog::getText()
{
    return editAddItemName->text().trimmed();
}

void BesEditDialog::initLayout()
{
    widgetMessageBoxContainer = new QWidget(this);
    widgetMessageBoxContainer->setMouseTracking(true);
    widgetMessageBoxContainer->setObjectName("widgetBody");

    QVBoxLayout * vMainLayout0 = new QVBoxLayout(this);
    vMainLayout0->setMargin(8);
    vMainLayout0->setSpacing(0);
    vMainLayout0->addWidget(widgetMessageBoxContainer);

    labelAddItemNameTip = new QLabel(widgetMessageBoxContainer);
    labelAddItemNameTip->setObjectName("labelAddItemNameTip");
    labelAddItemNameTip->setText(tr("编辑"));
    labelAddItemNameTip->setMinimumHeight(45* BesScaleUtil::mscale());
    labelAddItemNameTip->setMaximumHeight(45* BesScaleUtil::mscale());
    labelAddItemNameTip->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    editAddItemName = new QLineEdit(widgetMessageBoxContainer);
    editAddItemName->setObjectName("editAddItemName");
    editAddItemName->setMinimumHeight(40* BesScaleUtil::mscale());
    editAddItemName->setMaximumHeight(40* BesScaleUtil::mscale());
    editAddItemName->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    //editAddItemName->setText(strContent);

    btnOK = new BesButton(widgetMessageBoxContainer);
    btnCancel= new BesButton(widgetMessageBoxContainer);
    btnOK->setText(tr("确定"));
    btnCancel->setText(tr("取消"));
    btnOK->setObjectName("btnOK");
    btnCancel->setObjectName("btnCancel");
    btnOK->setEnabled(false);

    btnOK->setMinimumHeight(40* BesScaleUtil::mscale());
    btnOK->setMaximumHeight(40* BesScaleUtil::mscale());
    btnOK->setMinimumWidth(100* BesScaleUtil::mscale());
    btnOK->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    btnCancel->setMinimumHeight(40* BesScaleUtil::mscale());
    btnCancel->setMaximumHeight(40* BesScaleUtil::mscale());
    btnCancel->setMinimumWidth(100* BesScaleUtil::mscale());
    btnCancel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);


    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout->addWidget(btnOK);
    hLayout->addWidget(btnCancel);
    hLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));

    QVBoxLayout* vMainLayout = new QVBoxLayout(widgetMessageBoxContainer);
    vMainLayout->setMargin(32* BesScaleUtil::mscale());
    vMainLayout->setSpacing(25* BesScaleUtil::mscale());
    vMainLayout->addWidget(labelAddItemNameTip);
    vMainLayout->addWidget(editAddItemName);
    vMainLayout->addLayout(hLayout);
}

void BesEditDialog::initConnection()
{
    connect(editAddItemName, &QLineEdit::textChanged, this, &BesEditDialog::OnTextChanged);

    connect(btnOK, &QPushButton::clicked,[=](){this->accept();});
    connect(btnCancel, &QPushButton::clicked,[=](){this->reject();});

}

void BesEditDialog::OnTextChanged(QString text)
{
    text = text.trimmed();
    if(text.size() == 0 || strContent == text )
        btnOK->setEnabled(false);
    else
        btnOK->setEnabled(true);
}





