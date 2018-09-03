#ifndef SUBPAGEDOWNLOADLYRIC_H
#define SUBPAGEDOWNLOADLYRIC_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <BesButton.h>
#include <QTableWidget>
#include <QTabWidget>
#include <QListWidget>
#include <LyricSearcherFactory/threadsearchlyric.h>
#include "table/BesTableView.h"

class SubPageDownloadLyric : public QWidget
{
    Q_OBJECT

public:
    SubPageDownloadLyric(QWidget *parent = 0);
    ~SubPageDownloadLyric();

    void initEntity();
    void initLayout();
    void initConnection();

public slots:
    void OnSearchInProgram();

    void OnLyricResultChanged(LyricSearchResult result);

public:
    QLabel* labelTitleSearchLyric;
    QLabel* labelSearchLyricSong;
    QLabel* labelSearchLyricArtist;

    QLineEdit* editSearchLyricSong;
    QLineEdit* editSearchLyricArtist;

    BesButton* btnSearchInProgram;
    BesButton* btnSearchByBaidu;
    QTabWidget* tabpageLyricResult;

    QLabel* labelLyricResultTip1;
    QLabel* labelLyricResultTip2;
    QLabel* labelLyricResultTip3;
    QLabel* labelLyricResultTip4;
    QLabel* labelLyricResultTip5;


    BesTableView* tableLyricSearch;
    QWidget* widgetRawLyricBoard;
    QWidget* widgetLrcLyricBoard;

    ThreadSearchLyric searchThread;

};

#endif // SUBPAGEDOWNLOADLYRIC_H
