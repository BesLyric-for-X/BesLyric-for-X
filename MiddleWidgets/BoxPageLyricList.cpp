#include "BoxPageLyricList.h"
#include <QVBoxLayout>

BoxPageLyricList::BoxPageLyricList(QWidget *parent)
    : QWidget(parent)
{
    initLayout();
    initConnection();
}

BoxPageLyricList::~BoxPageLyricList()
{

}

void BoxPageLyricList::initLayout()
{
    widgetMainLyricList = new QWidget(this);
    widgetMainLyricList->setObjectName("widgetMainLyricList");

    QVBoxLayout* layoutMain = new QVBoxLayout(this);
    layoutMain->setMargin(0);
    layoutMain->addWidget(widgetMainLyricList);
}

void BoxPageLyricList::initConnection()
{

}
