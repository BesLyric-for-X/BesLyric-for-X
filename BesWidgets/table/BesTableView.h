#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "Define/Define.h"
#include "BesTableModel.h"
#include "BesTableView.h"
#include "BesButtonDelegate.h"
#include <QTableView>
#include <QVector>

class BesTableView : public QTableView
{
    Q_OBJECT
public:
    explicit BesTableView(QWidget *parent = 0);
    ~BesTableView();

    BesTableModel* tableModel() {return m_model;}

public:
    void deleteAllItems();
    void appendItems(QVector<LyricInfo> infos);


signals:

public slots:

protected:
    void iniData();

private:
    void BaseInit(); //基础的初始化


//    virtual void resizeEvent(QResizeEvent *event);
private:
    BesTableModel *m_model;
    BesButtonDelegate *m_buttonDelegate;

};

#endif // TABLEVIEW_H
