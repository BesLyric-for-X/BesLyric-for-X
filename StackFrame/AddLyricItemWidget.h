#ifndef AddLyricItemWidget_H
#define AddLyricItemWidget_H

#include "global.h"
#include "BesShadowWidget.h"
#include <QWidget>
#include "BesButton.h"
#include <QLabel>
#include <QLineEdit>
#include <QImage>
#include <QPainter>
#include <QStackedLayout>

class AddLyricItemWidget : public BesShadowWidget
{
    Q_OBJECT

public:
    AddLyricItemWidget(QWidget *parent = 0);
    ~AddLyricItemWidget();

    void initLayout();
    void initConnection();

    void setFinalSkinName(QString);                     //设置最终使用的皮肤名
public slots:

    void OnTextChanged(QString text);

signals:
    void sig_toggleAddWidget();
    void sig_addListItem(QString itemName);

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *event);

public:
    QLabel* labelAddItemNameTip;
    QLineEdit* editAddItemName;
    BesButton* btnAddListItem;
    BesButton* btnCancelListItem;

private:

    QString  finalSkinName;         //储存最后真正使用的皮肤，用于区别绘制skinBox背景



};

#endif // AddLyricItemWidget_H
