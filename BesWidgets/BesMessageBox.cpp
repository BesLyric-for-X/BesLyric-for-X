#include "global.h"
#include "BesMessageBox.h"
#include "BesScaleUtil.h"

BesMessageBox* BesMessageBox::besMessageBox = nullptr;

bool BesMessageBox::isBlackTheme = true;

int BesMessageBox::question(const QString &title, const QString &text, QString strYes, QString strNo, bool bShowIcon, bool bWordWrap)
{
    besMessageBox = new BesMessageBox(nullptr);

    if(!bShowIcon)
        besMessageBox->labelMessageIcon->setVisible(false);

    if(!strYes.isEmpty())
        besMessageBox->btnOK->setText(strYes);
    if(!strNo.isEmpty())
        besMessageBox->btnCancel->setText(strNo);

    if(isBlackTheme)
        besMessageBox->labelMessageIcon->setPixmap(QPixmap(":/resource/image/icon_question_black.png"));
    else
        besMessageBox->labelMessageIcon->setPixmap(QPixmap(":/resource/image/icon_question_white.png"));

    besMessageBox->labelMessageTitle->setText(title);
    besMessageBox->labelMessageContent->setText(text);
    besMessageBox->labelMessageContent->setWordWrap(bWordWrap);
    besMessageBox->labelMessageContent->setOpenExternalLinks(true);
    besMessageBox->exec();

    QMessageBox::StandardButton result = besMessageBox->btnResult;
    delete besMessageBox;
    return result;
}

int BesMessageBox::information(const QString &title, const QString &text, bool bWordWrap)
{
    besMessageBox = new BesMessageBox(nullptr);

    if(isBlackTheme)
        besMessageBox->labelMessageIcon->setPixmap(QPixmap(":/resource/image/icon_music_black.png"));
    else
        besMessageBox->labelMessageIcon->setPixmap(QPixmap(":/resource/image/icon_music_white.png"));

    besMessageBox->labelMessageTitle->setText(title);
    besMessageBox->labelMessageContent->setText(text);
    besMessageBox->labelMessageContent->setWordWrap(bWordWrap);
    besMessageBox->labelMessageContent->setOpenExternalLinks(true);
    besMessageBox->btnCancel->setVisible(false);
    besMessageBox->exec();

    QMessageBox::StandardButton result = besMessageBox->btnResult;
    delete besMessageBox;
    return result;
}

BesMessageBox::BesMessageBox(QWidget *parent):BesFramelessDialog(parent)
{
    this->setMinimumHeight(200* BesScaleUtil::scale());
    this->setMinimumWidth(400* BesScaleUtil::scale());

    initLayout();
    initConnection();
}

void BesMessageBox::initLayout()
{
    widgetMessageBoxContainer0 = new QWidget(this);
    widgetMessageBoxContainer0->setObjectName("widgetMessageBoxContainer");

    QVBoxLayout * vMainLayout0 = new QVBoxLayout(this);
    vMainLayout0->setMargin(0); //边缘的缩放会成问题，放大缩小不要了
    vMainLayout0->setSpacing(0);

    //由于设置样式要求有边缘，再内嵌一个QWidget
    widgetMessageBoxContainer= new QWidget(widgetMessageBoxContainer0);
    widgetMessageBoxContainer->setObjectName("widgetMessageBoxContainer");

    QVBoxLayout * vMainLayout = new QVBoxLayout(widgetMessageBoxContainer0);
    vMainLayout->setMargin(1);
    vMainLayout->setSpacing(0);
    vMainLayout->addWidget(widgetMessageBoxContainer);

    vMainLayout0->addWidget(widgetMessageBoxContainer0);

    widgetMessageTop = new QWidget(widgetMessageBoxContainer);
    widgetMessageMiddle = new QWidget(widgetMessageBoxContainer);
    widgetMessageBottom = new QWidget(widgetMessageBoxContainer);
    widgetMessageTop->setObjectName("widgetMessageTop");
    widgetMessageMiddle->setObjectName("widgetMessageMiddle");
    widgetMessageBottom->setObjectName("widgetMessageBottom");

    widgetMessageTop->setMinimumHeight(45* BesScaleUtil::scale());
    widgetMessageTop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    widgetMessageMiddle->setMinimumHeight(100* BesScaleUtil::scale());
    widgetMessageMiddle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widgetMessageBottom->setMinimumHeight(60* BesScaleUtil::scale());
    widgetMessageBottom->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout * vLayout = new QVBoxLayout(widgetMessageBoxContainer);
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    vLayout->addWidget(widgetMessageTop);
    vLayout->addWidget(widgetMessageMiddle);
    vLayout->addWidget(widgetMessageBottom);

    QHBoxLayout *hHeaderLayout = new QHBoxLayout(widgetMessageTop);

    labelMessageTitle = new QLabel(widgetMessageTop);
    labelMessageTitle->setObjectName("labelMessageTitle");
    labelMessageTitle->setText(tr("标题"));
    btnMessageClose = new BesButton(widgetMessageTop);
    btnMessageClose->setObjectName("btnMessageClose");

    hHeaderLayout->addWidget(labelMessageTitle);
    hHeaderLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::Expanding, QSizePolicy::Fixed));
    hHeaderLayout->addWidget(btnMessageClose);


    QHBoxLayout *hBodyLayout = new QHBoxLayout(widgetMessageMiddle);

    labelMessageIcon  = new QLabel(widgetMessageMiddle);
    labelMessageIcon->setPixmap(QPixmap(":/resource/image/icon_music_white.png"));
    labelMessageIcon->setMinimumSize(48* BesScaleUtil::scale(),48* BesScaleUtil::scale());
    labelMessageIcon->setMargin(15* BesScaleUtil::scale());
    labelMessageIcon->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    labelMessageContent = new QLabel(widgetMessageMiddle);
    labelMessageContent->setObjectName("labelMessageContent");
    labelMessageContent->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    labelMessageContent->setText("提示内容");

    hBodyLayout->addWidget(labelMessageIcon);
    hBodyLayout->addWidget(labelMessageContent);

    QHBoxLayout *hBottomLayout = new QHBoxLayout(widgetMessageBottom);

    btnOK = new BesButton(widgetMessageBottom);
    btnCancel = new BesButton(widgetMessageBottom);
    btnOK->setObjectName("btnOK");
    btnCancel->setObjectName("btnCancel");
    btnOK->setText(tr("确定"));
    btnCancel->setText(tr("取消"));
    btnOK->setMinimumSize(75* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    btnCancel->setMinimumSize(75* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    btnOK->setMaximumSize(200* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    btnCancel->setMaximumSize(200* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    btnOK->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    btnCancel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);


    hBottomLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::Expanding, QSizePolicy::Fixed));
    hBottomLayout->addWidget(btnOK);
    hBottomLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hBottomLayout->addWidget(btnCancel);
    hBottomLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20,QSizePolicy::Expanding, QSizePolicy::Fixed));
}

void BesMessageBox::initConnection()
{
    connect(btnMessageClose, &QPushButton::clicked,[=](){this->close(); btnResult= QMessageBox::StandardButton::Cancel;});
    connect(btnOK, &QPushButton::clicked,[=](){this->accept();btnResult= QMessageBox::StandardButton::Ok;});
    connect(btnCancel, &QPushButton::clicked,[=](){this->accept();btnResult= QMessageBox::StandardButton::Cancel;});
}

void BesMessageBox::setIsBlackTheme(bool value)
{
    isBlackTheme = value;
}
