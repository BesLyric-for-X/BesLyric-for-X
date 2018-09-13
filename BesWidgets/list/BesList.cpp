
#include "BesList.h"
#include <QHBoxLayout>
#include <QSpacerItem>

BesList::BesList(QWidget *parent):QListWidget(parent)
{
    this->setMouseTracking(true);
}


void BesList::addItem(QString item)
{
    strList.push_back(item);

    QListWidgetItem *pItem = new QListWidgetItem(this);
    pItem->setIcon(QIcon(":/resource/image/btn_setting_press_white.png"));
    pItem->setText(item);

    this->setMaximumHeight(35*strList.size());
    this->setMinimumHeight(35*strList.size());

}

void BesList::deleteItem(int index)
{
    strList.removeAt(index);
}

void BesList::removeAll()
{
    strList.clear();
}

void BesList::moveRow(int from, int to)
{
    QString item = strList.at(from);
    strList.removeAt(from);
    strList.insert(to,item);
}




void BesList::enterEvent(QEvent *event)
{
    setCursor(QCursor(Qt::ArrowCursor));
    QListWidget::enterEvent(event);
}

void BesList::leaveEvent(QEvent *event)
{
    unsetCursor();
    QListWidget::leaveEvent(event);
}
