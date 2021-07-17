#ifndef FinishLrcTableView_H
#define FinishLrcTableView_H

#include "Define/Define.h"
#include "FinishLrcTableModel.h"
#include "FinishLrcButtonDelegate.h"
#include <QTableView>
#include <QVector>

class FinishLrcTableView : public QTableView
{
    Q_OBJECT
public:
    explicit FinishLrcTableView(QWidget *parent = 0);
    ~FinishLrcTableView();

    FinishLrcTableModel* tableModel() {return m_model;}

public:
    void appendItems(const QVector<LyricInfo>& infos);

    void setLyricData(QVector<QPair<qint64, QString>>* pLrcLines);

    QVector<QString> getSelectedLrc();

signals:
    void sig_showRawLyric(const LyricInfo&);
    void sig_showLrcLyric(const LyricInfo&);

public slots:
    void onEditRow(int row);

protected:
    void iniData();
    virtual void resizeEvent(QResizeEvent *event);

private:
    void BaseInit(); //基础的初始化

private:
    FinishLrcTableModel *m_model;
    FinishLrcButtonDelegate *m_buttonDelegate;
};

#endif // FinishLrcTableView_H
