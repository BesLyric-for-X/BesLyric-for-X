
#include "BesList.h"
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPainter>
#include <QPainterPath>
#include <QDrag>

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
        QString imageName = getImageNameByTitleAndSkinName(list.name, currentSkinName);

        QListWidgetItem *pItem = new QListWidgetItem(this);
        pItem->setIcon(QIcon(imageName));
        pItem->setText(list.name);
    }
}

void BesList::addItem(QString item, int newId)
{
    if(pLyricLists==nullptr)
        return;

    //构建列表项
    QString imageName = getImageNameByTitleAndSkinName(item, currentSkinName);

    QListWidgetItem *pItem = new QListWidgetItem(this);
    pItem->setIcon(QIcon(imageName));
    pItem->setText(item);

    this->setMaximumHeight(35 * pLyricLists->size());
    this->setMinimumHeight(35 * pLyricLists->size());

    //构建实际的数据
    LyricList lyricList;
    lyricList.name = item;
    lyricList.id = newId;
    pLyricLists->push_back(lyricList);
    emit sig_saveLyriclistData();
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

    emit sig_saveLyriclistData();
}

void BesList::removeAll()
{
    if(pLyricLists==nullptr)
    {
        return;
    }

	pLyricLists->clear();

    emit sig_saveLyriclistData();
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
    for(int i = 0; i < this->count(); i++)
    {
        QString text = item(i)->data(Qt::DisplayRole).toString();
        QString imageName = getImageNameByTitleAndSkinName(text,skinName);
        item(i)->setIcon(QIcon(imageName));
    }
    currentSkinName = skinName;
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

    emit sig_saveLyriclistData();

    //拖动变换顺序之后，通知当前选择行发生改变，以使得外部可以重新加载
    emit currentRowChanged(to);
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

void BesList::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();
    if (indexes.count() > 0) {
        QMimeData *data = this->model()->mimeData(indexes);
        if (!data)
            return;

        //绘制拖动时显示的图像 (以预想拖动多个的项的情况来绘图，实际上歌词单每次只能拖动一个项)
        QPixmap pixmap;
        {
            //获得实际拖动的数据
            QVector<QString> items;
            {
                QSet<int> rowsSet;
                for(QModelIndex index:indexes)
                    rowsSet.insert(index.row());

                QList<int> rows(rowsSet.begin(),rowsSet.end());
                std::sort(rows.begin(),rows.end());

                for(int row:rows)
                {
                    QString name = pLyricLists->at(row).name;
                    items.push_back(name);
                }
            }

            //开始绘制
            int margin = 6;    //距边
            int interval = 4;  //行间隔
            int totalLine = items.size()>1 ? items.size()+1 : items.size(); //多于一行时，额外显示一个统计

            QFont font;
            QFontMetrics fmText = QFontMetrics(font);
            int heightOfLine = fmText.boundingRect("文本").height();   //一行高度

            int maxWidthOfText = 0;     //文字最大宽度
            for(QString fileName:items)
            {
                QRect rect = fmText.boundingRect(fileName);
                maxWidthOfText = qMax(maxWidthOfText,rect.width());
            }

            int HeightOfAllText = margin * 2 + totalLine * heightOfLine + (totalLine-1)*interval;
            int WidthOfAllText = margin * 2 + maxWidthOfText;

            pixmap = QPixmap(WidthOfAllText+1,HeightOfAllText+1);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            painter.setOpacity(0.6);
            painter.setFont(font);

            //绘制圆角矩形背景
            painter.setRenderHint(QPainter::Antialiasing);
            QPainterPath path;
            path.addRoundedRect(QRect(0, 0, WidthOfAllText, HeightOfAllText), margin, margin);
            painter.fillPath(path, Qt::white);
            painter.drawPath(path);

            //绘制文字
            QString extraHeader = items.size() >1?QString("共 %1 个歌单").arg(items.size()):"";
            int currentY = margin + heightOfLine;
            if(!extraHeader.isEmpty())
            {
                painter.drawText(margin,currentY,extraHeader);
                currentY += heightOfLine + interval;
            }
            for(QString fileName:items)
            {
                painter.drawText(margin,currentY,fileName);
                currentY += heightOfLine + interval;
            }
        }

        QDrag *drag = new QDrag(this);
        drag->setPixmap(pixmap);
        drag->setMimeData(data);
        drag->exec(supportedActions);
    }
}

QString BesList::getImageNameByTitleAndSkinName(const QString& title, const QString& skinName)
{
    QString imageName;
    if(skinName == "black")
    {
        if(title == tr("制作记录") || title==tr("历史记录"))
            imageName = ":/resource/image/history_list_black.png";
        else
            imageName = ":/resource/image/lyric_list_icon_black.png";
    }
    else
    {
        if(title == tr("制作记录") || title==tr("历史记录"))
            imageName = ":/resource/image/history_list_white.png";
        else
            imageName = ":/resource/image/lyric_list_icon_white.png";
    }
    return imageName;
}
