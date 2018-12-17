#ifndef BesEditDialog_H
#define BesEditDialog_H

#include "global.h"
#include "BesFramelessDialog.h"
#include <QWidget>
#include "BesButton.h"
#include <QLabel>
#include <QLineEdit>
#include <QImage>
#include <QPainter>
#include <QStackedLayout>

class BesEditDialog : public BesFramelessDialog
{
    Q_OBJECT

public:
    BesEditDialog(QWidget *parent = 0);
    ~BesEditDialog();

    void setText(QString text);
    QString getText();

    void initLayout();
    void initConnection();

public slots:

    void OnTextChanged(QString text);

signals:
    void sig_toggleAddWidget();
    void sig_addListItem(QString itemName);

protected:

public:
    QWidget* widgetMessageBoxContainer;

    QLabel* labelAddItemNameTip;
    QLineEdit* editAddItemName;

    BesButton *btnOK;
    BesButton *btnCancel;

    QString strContent;
};

#endif // BesEditDialog_H
