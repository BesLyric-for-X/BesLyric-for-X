
#include "BesList.h"
#include <QHBoxLayout>
#include <QSpacerItem>

BesList::BesList(QWidget *parent):QListWidget(parent)
{
    pLyricLists = nullptr;
    this->setMouseTracking(true);

    initConnection();
}

void BesList::initConnection()
{
    connect(this->model(), &QAbstractItemModel::rowsMoved, this,&BesList::rowsMoved);
}

void BesList::setLyricLists(QVector<LyricList> &lyricLists)
{
    pLyricLists = &lyricLists;

	this->setMinimumHeight(35 * pLyricLists->size());
	this->setMaximumHeight(35 * pLyricLists->size());

    //创建歌词单项
    for(auto& list:*pLyricLists)
    {
        addItem(list.name, false);
    }
}


void BesList::addItem(QString item, bool bConstructNewData)
{
    if(pLyricLists==nullptr)
    {
        return;
    }

	LyricList lyricList;
	lyricList.name = item;

    QString imageName = ":/resource/image/lyric_list_icon_black.png";

    if(item == tr("制作记录") || item==tr("历史记录"))
    {
        imageName = ":/resource/image/history_list_white.png";
    }

	if (bConstructNewData)
	{
		pLyricLists->push_back(lyricList);
		emit sig_listDataChanged();

		this->setMaximumHeight(35 * pLyricLists->size());
		this->setMinimumHeight(35 * pLyricLists->size());
	}

    QListWidgetItem *pItem = new QListWidgetItem(this);
    pItem->setIcon(QIcon(imageName));
    pItem->setText(lyricList.name);
}

void BesList::deleteCurrentItem()
{
    if(pLyricLists==nullptr)
        return;

    int index = this->currentRow();
    if(index == -1)
    {
        return;
    }

    QListWidgetItem* item = this->takeItem(index);
    if(item)
        delete item;

    //必须在 this->takeItem(index) 执行之后再删除实际数据，因为实践发现,执 行 this->takeItem(index) 时,
    //QListWidget::currentRowChanged 信号会被触发选中下一行的选中，进一步地，BesList::getCurrentItemData()
    //会被调用，其中会访问 pLyricLists 的数据，如果数据先于takeItem(index)删除，则可能会因越界而崩溃
    pLyricLists->removeAt(index);

    this->setMaximumHeight(35* pLyricLists->size());
    this->setMinimumHeight(35* pLyricLists->size());

	emit sig_listDataChanged();
}

void BesList::removeAll()
{
    if(pLyricLists==nullptr)
    {
        return;
    }

	pLyricLists->clear();

	emit sig_listDataChanged();
}

int BesList::getCurrentIndex()
{
    if(pLyricLists==nullptr)
        return -1;

    return this->currentRow();
}

LyricList *BesList::getCurrentItemData()
{
    if(pLyricLists==nullptr)
        return nullptr;

    int index = this->currentRow();
    if(index == -1 && pLyricLists->size() == 0)
        return nullptr;

    return (LyricList*)&(pLyricLists->at(index));
}

void BesList::reloadAllItemText()
{
    for(int i = 0; i< pLyricLists->size(); i++)
    {
        this->item(i)->setText(pLyricLists->at(i).name);
    }
}

void BesList::setFinalSkinName(QString skinName)
{
    if(skinName == "black")
    {
        for(int i = 0; i < this->count(); i++)
        {
            QString text = item(i)->data(Qt::DisplayRole).toString();

            QString imageName;
            if(text == tr("制作记录") || text==tr("历史记录"))
                imageName = ":/resource/image/history_list_white.png";
            else
                imageName = ":/resource/image/lyric_list_icon_black.png";

            item(i)->setIcon(QIcon(imageName));
        }
    }
    else
    {

        for(int i = 0; i < this->count(); i++)
        {
            QString text = item(i)->data(Qt::DisplayRole).toString();

            QString imageName;
            if(text == tr("制作记录") || text==tr("历史记录"))
                imageName = ":/resource/image/history_list_black.png";
            else
                imageName = ":/resource/image/lyric_list_icon_white.png";

            item(i)->setIcon(QIcon(imageName));
        }
    }
}

void BesList::rowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{
    Q_UNUSED(parent)
    Q_UNUSED(end)
    Q_UNUSED(destination)
    //(start,end)->row

    int from = start;
    int to = row;

    if(pLyricLists==nullptr)
    {
        return;
    }

    LyricList item = pLyricLists->at(from);
    pLyricLists->removeAt(from);

    if(to > from) //如果目标项下标大于拖动项，由于先去除了from对应的项，to 需要减 1
        to -= 1;

    pLyricLists->insert(to,item);

    emit sig_listDataChanged();
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
