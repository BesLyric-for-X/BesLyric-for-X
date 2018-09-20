#ifndef SUBPAGEDOWNLOADSONG_H
#define SUBPAGEDOWNLOADSONG_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include "BesButton.h"
#include "LyricSearcherFactory/ThreadSearchNcmMusic.h"
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

public slots:
    void OnSearchSong();

    void OnSongResultChanged(LyricSearchResult result);

private:
    void showTipLabel(bool bShow);

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
