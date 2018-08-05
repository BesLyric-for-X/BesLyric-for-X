#ifndef BOXPAGEPREVIEWLYRIC_H
#define BOXPAGEPREVIEWLYRIC_H

#include <global.h>
#include <QWidget>
#include <BesButton.h>
#include <QPixmap>
#include <QPainter>

class ExtendButton: public BesButton
{
public:
    ExtendButton(QWidget *parent = nullptr):BesButton(parent),m_bHover(false)
    {
        extendMark = QPixmap(":/resource/image/extend.png");
        SetToDefaultPixmap();
    }

    void SetPixmap(QPixmap img){image = img;update();}
    void SetToDefaultPixmap(){image = QPixmap(":/resource/image/AlbumCover1.jpg");update();}

protected:
    virtual void paintEvent(QPaintEvent* event)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

        QRect outerRect(0,0,this->width()-1,this->height()-1);
        painter.drawPixmap(outerRect, image);
        QWidget::paintEvent(event);

        if(m_bHover)
        {
            painter.drawPixmap(outerRect,extendMark);
        }
    }

    virtual void enterEvent(QEvent *e){m_bHover=true;BesButton::enterEvent(e);}//进入事件
    virtual void leaveEvent(QEvent *e){m_bHover=false;BesButton::leaveEvent(e);}//离开事件

    virtual void mousePressEvent(QMouseEvent* e){BesButton::mousePressEvent(e);}//emit(onSkinClick(m_skinName));



private:
    bool m_bHover;
    QPixmap extendMark;
    QPixmap image;
};

class BoxPagePreviewLyric : public QWidget
{
    Q_OBJECT

public:
    BoxPagePreviewLyric(QWidget *parent = 0);
    ~BoxPagePreviewLyric();

    void initLayout();
    void initConnection();

public slots:
    void changePic(QPixmap pic);

    void setToDefaultPic();

protected:

    void mouseMoveEvent(QMouseEvent *event)
    {
        QWidget::mouseMoveEvent(event);
    }

    virtual void resizeEvent(QResizeEvent *event);

public:
    QWidget* widgetMainPreviewLyric;
    ExtendButton* extendButton;

};

#endif // BOXPAGEPREVIEWLYRIC_H
