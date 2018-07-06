#ifndef SKINBOXWIDGET_H
#define SKINBOXWIDGET_H

#include <QWidget>

class SkinBoxWidget : public QWidget
{
    Q_OBJECT

public:
    SkinBoxWidget(QWidget *parent = 0);
    ~SkinBoxWidget();

    void initLayout();
    void connectAll();

private:

};

#endif // SKINBOXWIDGET_H
