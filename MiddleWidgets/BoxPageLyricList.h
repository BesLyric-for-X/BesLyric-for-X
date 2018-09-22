#ifndef BOXPAGELYRICLIST_H
#define BOXPAGELYRICLIST_H

#include <QWidget>
#include <QLabel>
#include "BoxPagePreviewLyric.h"

class BoxPageLyricList : public QWidget
{
    Q_OBJECT

public:
    BoxPageLyricList(QWidget *parent = 0);
    ~BoxPageLyricList();

    void initLayout();
    void initConnection();

public slots:
    void changePic(QPixmap pic);

    void setToDefaultPic();

public:
    void setFinalSkinName(QString skinName);

protected:

    void mouseMoveEvent(QMouseEvent *event)
    {
        QWidget::mouseMoveEvent(event);
    }

    virtual void resizeEvent(QResizeEvent *event);



public:
    QWidget* widgetMainLyricList;
    ExtendButton* extendButton;
    QLabel* labelLyricListBoxTip;
};

#endif // BOXPAGELYRICLIST_H
