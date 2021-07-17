#ifndef BESLRCTABLEVIEW_H
#define BESLRCTABLEVIEW_H

#include "Define/Define.h"
#include "BesLrcTableModel.h"
#include "BesLrcButtonDelegate.h"
#include <QTableView>
#include <QVector>

class BesLrcTableView : public QTableView
{
    Q_OBJECT
public:
    explicit BesLrcTableView(QWidget *parent = 0);
    ~BesLrcTableView();

    BesLrcTableModel* tableModel() {return m_model;}

    void deleteAllItems();
    void appendItems(const QVector<LyricInfo>& infos);

signals:
    void sig_showRawLyric(const LyricInfo&);
    void sig_showLrcLyric(const LyricInfo&);

protected:
    void iniData();
    virtual void resizeEvent(QResizeEvent *event);

private:
    void BaseInit(); //基础的初始化

private:
    BesLrcTableModel *m_model;
    BesLrcButtonDelegate *m_buttonDelegate;
};

#endif // BESLRCTABLEVIEW_H
