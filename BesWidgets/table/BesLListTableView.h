#ifndef BES_LLIST_TABLEVIEW_H
#define BES_LLIST_TABLEVIEW_H

#include "Define/Define.h"
#include "BesLListTableModel.h"
#include "BesLListButtonDelegate.h"
#include "BesLListViewStyle.hpp"
#include "NetAccess.h"

#include <QTableView>
#include <QVector>

class BesLListTableView : public QTableView
{
    Q_OBJECT
public:
    explicit BesLListTableView(QWidget *parent = 0);
    ~BesLListTableView();

    BesLListTableModel* tableModel() {return m_model;}

public:
    void setDataSource(LyricList* pData);

    void reloadTableFromData();

signals:
    void sig_deleteItem(int row);
    void sig_editItem(int row);
    void sig_playSongAndLyric(QString song, QString lyric);
    void sig_saveLyricListData();

public slots:
    void baseColorChanged(QColor color);    //基础颜色发生改变
    void finishDragging();                   //完成拖动

protected:
    void initEntity();
    void initConnection();
    virtual void resizeEvent(QResizeEvent *event);
    virtual void startDrag(Qt::DropActions supportedActions);
private:
    void BaseInit(); //基础的初始化

private:
    BesLListTableModel *m_model;
    BesLListButtonDelegate *m_buttonDelegate;
    NetworkAccess net;
    BesLListViewStyle* m_listViewStyle;
};

#endif // BES_LLIST_TABLEVIEW_H
