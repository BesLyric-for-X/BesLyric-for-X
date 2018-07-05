#ifndef BESFRAMELESSWIDGET_H
#define BESFRAMELESSWIDGET_H

#include <QWidget>
#include<QMouseEvent>

enum Direction{
    UP = 0,
    DOWN=1,
    LEFT,
    RIGHT,
    LEFTTOP,
    LEFTBOTTOM,
    RIGHTBOTTOM,
    RIGHTTOP,
    NONE
};

class BesFramelessWidget : public QWidget
{
    Q_OBJECT
public:
    BesFramelessWidget(QWidget *parent = 0);
    virtual~BesFramelessWidget(){}
private:
    void region(const QPoint &cursorPoint);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
protected:
    virtual void paintEvent(QPaintEvent *);
private:
    bool isLeftPressDown;
    QPoint dragPosition;
    Direction dir;
    int border;
};

#endif // BESFRAMELESSWIDGET_H
