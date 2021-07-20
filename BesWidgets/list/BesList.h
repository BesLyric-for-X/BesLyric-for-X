#ifndef BES_LIST_H
#define BES_LIST_H

#include <QListWidget>
#include <QLabel>
#include <QList>
#include "BesButton.h"
#include "LyricListManager.h"

class BesList :public QListWidget
{
    Q_OBJECT
public:
    BesList(QWidget *parent = Q_NULLPTR);
    void initConnection();

    void setLyricLists(QVector<LyricList>& lyricLists);

    void addItem(QString item, bool bConstructNewData = true);
    void deleteCurrentItem();
    void removeAll();

    int getCurrentIndex();
    LyricList* getCurrentItemData();

    void reloadAllItemText();

    void setFinalSkinName(QString skinName);
signals:
    void sig_listDataChanged();

public slots:
    void rowsMoved( const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);

protected:

    virtual QSize sizeHint() const{return QSize(220,35);}   //默认宽度是256，和左侧列表250出现垂直滚动条时"挤压"，调小些

    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);

private:
    QString getImageNameByTitleAndSkinName(const QString& title, const QString& skinName);

private:
    QString currentSkinName;
    QVector<LyricList>* pLyricLists;
};

#endif // BES_LIST_H
