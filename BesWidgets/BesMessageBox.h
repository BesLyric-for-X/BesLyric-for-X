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
    static int question(const QString &title,const QString& text,
           QString strYes = "", QString strNo = "",bool bShowIcon = true, bool bWordWrap = false);

    static int information(const QString &title,const QString& text, bool bWordWrap = false);


    BesMessageBox(QWidget *parent);

    void initLayout();
    void initConnection();


    static void setIsBlackTheme(bool value);

public slots:


private:
    QWidget* widgetMessageBoxContainer0;
    QWidget* widgetMessageBoxContainer;
    QWidget* widgetMessageTop;
    QWidget* widgetMessageMiddle;
    QWidget* widgetMessageBottom;

    QLabel *labelMessageTitle;
    BesButton *btnMessageClose;

    QLabel *labelMessageIcon;
    QLabel *labelMessageContent;

    BesButton *btnOK;
    BesButton *btnCancel;
    QMessageBox::StandardButton btnResult;

    static BesMessageBox* besMessageBox;
    static bool isBlackTheme;
};

#endif // BESMESSAGEBOX_H
