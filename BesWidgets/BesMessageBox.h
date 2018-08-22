#ifndef BESMESSAGEBOX_H
#define BESMESSAGEBOX_H

#include <BesShadowWidget.h>
#include <BesFramelessDialog.h>
#include <QMessageBox>
#include <BesButton.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPixmap>

class BesMessageBox :public BesFramelessDialog
{
public:
    static int question(const QString &title,const QString& text)
    {
        besMessageBox = new BesMessageBox(nullptr);

        besMessageBox->labelMessageIcon->setPixmap(QPixmap(":/resource/image/icon_question_white.png"));
        besMessageBox->labelMessageTitle->setText(title);
        besMessageBox->labelMessageContent->setText(text);
        besMessageBox->exec();

        delete besMessageBox;
        return besMessageBox->btnResult;
    }

    static int information(const QString &title,const QString& text)
    {
        besMessageBox = new BesMessageBox(nullptr);

        besMessageBox->labelMessageIcon->setPixmap(QPixmap(":/resource/image/icon_music_white.png"));
        besMessageBox->labelMessageTitle->setText(title);
        besMessageBox->labelMessageContent->setText(text);
        besMessageBox->btnCancel->setVisible(false);
        besMessageBox->exec();

        delete besMessageBox;
        return besMessageBox->btnResult;
    }


    BesMessageBox(QWidget *parent):BesFramelessDialog(parent)
    {
        this->setMinimumHeight(200);
        this->setMinimumWidth(400);

        initLayout();
        initConnection();
    }

    void initLayout()
    {
        QWidget* widgetMessageBoxContainer = new QWidget(this);
        widgetMessageBoxContainer->setObjectName("widgetMessageBoxContainer");
        QVBoxLayout * vMainLayout = new QVBoxLayout(this);
        vMainLayout->setMargin(0); //边缘放大缩小不要了
        vMainLayout->setSpacing(0);
        vMainLayout->addWidget(widgetMessageBoxContainer);

        QWidget* widgetMessageTop = new QWidget(widgetMessageBoxContainer);
        QWidget* widgetMessageMiddle = new QWidget(widgetMessageBoxContainer);
        QWidget* widgetMessageBottom = new QWidget(widgetMessageBoxContainer);
        widgetMessageTop->setObjectName("widgetMessageTop");
        widgetMessageMiddle->setObjectName("widgetMessageMiddle");
        widgetMessageBottom->setObjectName("widgetMessageBottom");

        widgetMessageTop->setMinimumHeight(45);
        widgetMessageTop->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        widgetMessageMiddle->setMinimumHeight(100);
        widgetMessageMiddle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        widgetMessageBottom->setMinimumHeight(60);
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
        hHeaderLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding, QSizePolicy::Fixed));
        hHeaderLayout->addWidget(btnMessageClose);


        QHBoxLayout *hBodyLayout = new QHBoxLayout(widgetMessageMiddle);

        labelMessageIcon  = new QLabel(widgetMessageMiddle);
        labelMessageIcon->setPixmap(QPixmap(":/resource/image/icon_music_white.png"));
        labelMessageIcon->setMinimumSize(48,48);
        labelMessageIcon->setMargin(15);
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
        btnOK->setMinimumSize(75,30);
        btnCancel->setMinimumSize(75,30);
        btnOK->setMaximumSize(75,30);
        btnCancel->setMaximumSize(75,30);
        btnOK->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        btnCancel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


        hBottomLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding, QSizePolicy::Fixed));
        hBottomLayout->addWidget(btnOK);
        hBottomLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed, QSizePolicy::Fixed));
        hBottomLayout->addWidget(btnCancel);
        hBottomLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding, QSizePolicy::Fixed));
    }
    void initConnection()
    {
        connect(btnMessageClose, &QPushButton::clicked,[=](){this->close(); btnResult= QMessageBox::StandardButton::Cancel;});
        connect(btnOK, &QPushButton::clicked,[=](){this->accept();btnResult= QMessageBox::StandardButton::Ok;});
        connect(btnCancel, &QPushButton::clicked,[=](){this->accept();btnResult= QMessageBox::StandardButton::Cancel;});
    }


public slots:


private:
    QLabel *labelMessageTitle;
    BesButton *btnMessageClose;

    QLabel *labelMessageIcon;
    QLabel *labelMessageContent;

    BesButton *btnOK;
    BesButton *btnCancel;
    QMessageBox::StandardButton btnResult;

    static BesMessageBox* besMessageBox;
};




#endif // BESMESSAGEBOX_H
