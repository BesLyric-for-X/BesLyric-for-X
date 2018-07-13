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
#include <QStackedLayout>

class ButtonTheme: public BesButton
{
    Q_OBJECT
public:
    ButtonTheme(QWidget* parent, QImage* image,QString skinName, QString title,QColor color)
        :BesButton(parent),m_image(image),m_skinName(skinName),m_title(title),m_color(color),m_bHover(false){}

    virtual void paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);


        painter.drawPixmap(2,2,this->width()- 2*2,this->height()- 2*2,
                           QPixmap::fromImage(m_image->scaled(this->width()- 2*2, this->height()- 2*2,
                                              Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
        QWidget::paintEvent(event);

        if(m_bHover)
        {
            QRectF outerRect(0,0,this->width()-1,this->height()-1);
            painter.setPen(m_color);
            painter.drawRect(outerRect);
        }

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
        painter.drawText(rect,m_title, option);
    }

    virtual void enterEvent(QEvent *e){m_bHover=true;BesButton::enterEvent(e);}//进入事件
    virtual void leaveEvent(QEvent *e){m_bHover=false;BesButton::leaveEvent(e);}//离开事件

    virtual void mousePressEvent(QMouseEvent* e){emit(onSkinClick(m_skinName));BesButton::mousePressEvent(e);}

signals:
    void onSkinClick(QString name);

private:
    QImage*     m_image;
    QString     m_skinName;
    QString     m_title;
    QColor      m_color;
    bool        m_bHover;
};

class ButtonPureColor: public BesButton
{
    Q_OBJECT
public:
    ButtonPureColor(QWidget* parent,QString colorStr) :BesButton(parent),m_colorStr(colorStr),m_bHover(false){}


    virtual void paintEvent(QPaintEvent* event)
    {
        QWidget::paintEvent(event);

        QPainter painter(this);
        QColor color(m_colorStr);

        if(m_bHover)
        {
            QRectF outerRect(0,0,this->width()-1,this->height()-1);
            painter.setPen(color);
            painter.drawRect(outerRect);
        }

        QRectF rect;
        rect.setLeft(2);
        rect.setRight(this->width()-2);
        rect.setTop(2);
        rect.setBottom(this->height()- 2);
        painter.fillRect(rect,color);
    }

    virtual void enterEvent(QEvent *e){m_bHover=true;BesButton::enterEvent(e);}//进入事件
    virtual void leaveEvent(QEvent *e){m_bHover=false;BesButton::leaveEvent(e);}//离开事件

    virtual void mousePressEvent(QMouseEvent* e){emit(onSkinClick(m_colorStr));BesButton::mousePressEvent(e);}

signals:
    void onSkinClick(QString name);
private:
    QString     m_colorStr;
    bool        m_bHover;
};

class SkinBoxWidget : public BesShadowWidget
{
    Q_OBJECT

public:
    SkinBoxWidget(QWidget *parent = 0);
    ~SkinBoxWidget();

    void initLayout();
    void connectAll();

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *event);

public:
    QWidget*        frontLayer;
    QWidget*        mainLayer;

    QStackedLayout* skinStack;

    BesButton*      btnCheckMark;

    BesButton*      btnTheme;
    BesButton*      btnPureColor;

    ButtonTheme*    btnBlack;
    ButtonTheme*    btnRed;
    ButtonTheme*    btnPink;
    ButtonTheme*    btnBlue;
    ButtonTheme*    btnGreen;
    ButtonTheme*    btnGold;

    ButtonPureColor*    btnPureColor1;
    ButtonPureColor*    btnPureColor2;
    ButtonPureColor*    btnPureColor3;
    ButtonPureColor*    btnPureColor4;
    ButtonPureColor*    btnPureColor5;
    ButtonPureColor*    btnPureColor6;
    ButtonPureColor*    btnPureColor7;
    ButtonPureColor*    btnPureColor8;
    ButtonPureColor*    btnPureColor9;
    ButtonPureColor*    btnPureColor10;
    ButtonPureColor*    btnPureColor11;
    ButtonPureColor*    btnPureColor12;

    QLabel*         labelCustomizeColor;
    QPushButton*    btnCustomizeColor;
    QSlider*        SliderColor;
    QSlider*        SliderBrightness;
};

#endif // SKINBOXWIDGET_H
