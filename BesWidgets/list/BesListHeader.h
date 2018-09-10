#ifndef BES_LIST_HEADER_H
#define BES_LIST_HEADER_H

#include <QWidget>
#include <QLabel>
#include "BesButton.h"

class BesListHeader :public QWidget
{
    Q_OBJECT

public:
    BesListHeader(QWidget* parent = nullptr);
    BesListHeader(QString title, bool bShowAddButton, bool bShowToggleButton, QWidget* parent = nullptr);
    void setHeader(QString title, bool bShowAddButton, bool bShowToggleButton);

private:
    void initAll();
    void initEntity();
    void initLayout();
    void initConnection();
    void updateVisibleStatus();

signals:
    void sig_toggle_list(bool show);

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent* event);

public:
    QLabel * labelListTitle;
    BesButton*  btnAddItem;
    BesButton*  btnToggleList;
    bool showAddButton;
    bool showToggleButton;

};

#endif // BES_LIST_HEADER_H
