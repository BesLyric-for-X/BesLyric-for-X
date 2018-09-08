#ifndef PAGEMAIN_H
#define PAGEMAIN_H

#include <QWidget>
#include "SubPageMaking.h"
#include "SubPageDownloadSong.h"
#include "SubPageDownloadLyric.h"
#include "BoxPageLyricList.h"
#include "BoxPagePreviewLyric.h"
#include <QStackedLayout>
#include "BesButton.h"

class PageMain : public QWidget
{
    Q_OBJECT

public:
    PageMain(QWidget *parent = 0);
    ~PageMain();

    void initLayout();
    void initConnection();

public slots:
    void OnAutoSelectRawLyric(const QString& RawlyricPath);

public:
    QWidget*                leftBoardMainPage;

    BesButton*              btnMakingLyric;
    BesButton*              btnDownloadSong;
    BesButton*              btnDownloadLyric;
    BoxPageLyricList*       boxPageLyricList;
    BoxPagePreviewLyric*    boxPagePreviewLyric;

    QWidget*                subPageContainer;
    QStackedLayout*         subpageStackedLayout;
    SubPageMaking*          subPageMaking;
    SubPageDownloadSong*    subPageDownloadSong;
    SubPageDownloadLyric*   subPageDownloadLyric;
};

#endif // PAGEMAIN_H
