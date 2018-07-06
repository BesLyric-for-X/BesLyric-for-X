#ifndef PAGEMAIN_H
#define PAGEMAIN_H

#include <QWidget>
#include "SubPageMaking.h"
#include "SubPageDownloadSong.h"
#include "SubPageDownloadLyric.h"

class PageMain : public QWidget
{
    Q_OBJECT

public:
    PageMain(QWidget *parent = 0);
    ~PageMain();

    void initLayout();
    void connectAll();

private:
    SubPageMaking*          subPageMaking;
    SubPageDownloadSong*    subPageDownloadSong;
    SubPageDownloadLyric*   subPageDownloadLyric;
};

#endif // PAGEMAIN_H
