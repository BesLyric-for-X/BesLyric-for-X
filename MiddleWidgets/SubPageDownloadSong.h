#ifndef SUBPAGEDOWNLOADSONG_H
#define SUBPAGEDOWNLOADSONG_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "BesButton.h"
#include "Thread/ThreadSearchNcmMusic.h"
#include "table/BesNcmSongTableView.h"

class SubPageDownloadSong : public QWidget
{
    Q_OBJECT

public:
    SubPageDownloadSong(QWidget *parent = 0);
    ~SubPageDownloadSong();

    void initLayout();
    void initConnection();

public:
    void searchNcmDirectly(const QString& artists, const QString& song);

public slots:
    void OnSearchSong();

    void OnSongResultChanged(LyricSearchResult result);

    //由于要求在下载过程中不能重新搜索歌曲，所以这里使用2个接口供外部调用，只有在没有下载任务时才启用搜索
    void onStartOneDownload();
    void onFinishOneDownload();


private:
    void showTipLabel(bool bShow);

private:
    int nCurrentDownloadCount;

public:
    QLabel* labelTitleSearchSong;
    QLabel* labelSearchNcmSong;
    QLabel* labelSearchNcmArtist;

    QLineEdit* editSearchNcmSong;
    QLineEdit* editSearchNcmArtist;

    BesButton* btnSearchNcmSong;
    QTabWidget* tabpageNcmSongResult;

    QLabel* labelNcmSongResultTip1;
    QLabel* labelNcmSongResultTip2;
    QLabel* labelNcmSongResultTip3;
    QLabel* labelNcmSongResultTip4;
    QLabel* labelNcmSongResultTip5;


    BesNcmSongTableView* tableNcmSongSearch;

    ThreadSearchNcmMusic searchThread;

private:
    QString strLastSongName;
    QString strLastArtistName;

};

#endif // SUBPAGEDOWNLOADSONG_H
