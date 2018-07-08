#ifndef SKINBOXWIDGET_H
#define SKINBOXWIDGET_H

#include "global.h"
#include "BesShadowWidget.h"
#include <QWidget>
#include "BesButton.h"
#include <QSlider>
#include <QLabel>
#include <QImage>
#include <QPainter>

class ButtonTheme: public QPushButton
{
public:
    ButtonTheme(QWidget* parent, QImage* image, QString name):QPushButton(parent),m_image(image),m_name(name){}

    virtual void paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);


        painter.drawPixmap(2,2,this->width()- 2*2,this->height()- 2*2,
                           QPixmap::fromImage(m_image->scaled(this->width()- 2*2, this->height()- 2*2,
                                              Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
        QWidget::paintEvent(event);

        QRectF rect;
        rect.setLeft(2);
        rect.setRight(this->width()-2);
        rect.setTop(this->height()- 2 - 25);
        rect.setBottom(this->height()- 2);
        painter.fillRect(rect,QColor(2,0,0,10));

        rect.setLeft(8);
        painter.setPen(QColor("white"));
        QFont font = painter.font();
        font.setFamily("Microsoft YaHei");
        font.setPixelSize(15);
        painter.setFont(font);
        QTextOption option;
        option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        painter.drawText(rect,m_name, option);
    }

private:
    QImage*     m_image;
    QString     m_name;
};

class ButtonPureColor: public QPushButton
{

};

class SkinBoxWidget : public BesShadowWidget
{
    Q_OBJECT

public:
    SkinBoxWidget(QWidget *parent = 0);
    ~SkinBoxWidget();

    void initLayout();
    void connectAll();

public:
    QWidget*        frontLayer;
    QWidget*        mainLayer;

    QPushButton*    btnCheckMark;

    QPushButton*    btnTheme;
    QPushButton*    btnPureColor;

    ButtonTheme*    btnBlack;
    ButtonTheme*    btnRed;
    ButtonTheme*    btnPink;
    ButtonTheme*    btnBlue;
    ButtonTheme*    btnGreen;
    ButtonTheme*    btnGold;

    QPushButton*    btnPureColor1;
    QPushButton*    btnPureColor2;
    QPushButton*    btnPureColor3;
    QPushButton*    btnPureColor4;
    QPushButton*    btnPureColor5;
    QPushButton*    btnPureColor6;
    QPushButton*    btnPureColor7;
    QPushButton*    btnPureColor8;
    QPushButton*    btnPureColor9;
    QPushButton*    btnPureColor10;
    QPushButton*    btnPureColor11;
    QPushButton*    btnPureColor12;

    QLabel*         labelCustomizeColor;
    QPushButton*    btnCustomizeColor;
    QSlider*        SliderColor;
    QSlider*        SliderBrightness;
};

#endif // SKINBOXWIDGET_H
