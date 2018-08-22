#ifndef BESSHADOWDIALOG_H
#define BESSHADOWDIALOG_H

#include <QDialog>
#include<QMouseEvent>

class BesShadowDialog : public QDialog
{
    Q_OBJECT
public:
    BesShadowDialog(QWidget *parent = 0);
    virtual~BesShadowDialog(){}

    void SetShadowBorder(int boder);

    void SetMarginTop(int margin);

    void SetBackgroundColor(QColor bgColor);
protected:
    virtual void paintEvent(QPaintEvent *);

    QColor bgColor;     //背景颜色
    int marginTop;      //上边缘空出距离
    int border;
};

#endif // BESSHADOWDIALOG_H
