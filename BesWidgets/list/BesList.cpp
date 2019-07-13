
#include "BesList.h"
#include <QHBoxLayout>
#include <QSpacerItem>

BesList::BesList(QWidget *parent):QListWidget(parent)
{
    pLyricLists = nullptr;
    this->setMouseTracking(true);
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

	pLyricLists->removeAt(index);

    QListWidgetItem* item = this->takeItem(index);
    if(item)
        delete item;

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

void BesList::moveRow(int from, int to)
{
    if(pLyricLists==nullptr)
    {
        return;
    }

	LyricList item = pLyricLists->at(from);
	pLyricLists->removeAt(from);

    if(to > pLyricLists->size()) //但拖向最后一项时，由于移除了1项后，需要纠正目标下标防止越界访问
        to = pLyricLists->size();

	pLyricLists->insert(to,item);

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
    if(index == -1)
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
