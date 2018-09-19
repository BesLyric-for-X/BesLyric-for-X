#ifndef SUBPAGEDOWNLOADLYRIC_H
#define SUBPAGEDOWNLOADLYRIC_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <BesButton.h>
#include <QTableWidget>
#include <QTabWidget>
#include <QListWidget>
#include <QTextEdit>
#include "LyricSearcherFactory/threadsearchlyric.h"
#include "table/BesLrcTableView.h"

class SubPageDownloadLyric : public QWidget
{
    Q_OBJECT

public:
    SubPageDownloadLyric(QWidget *parent = 0);
    ~SubPageDownloadLyric();

    void initLayout();
    void initConnection();

signals:
    void sig_autoSelectRawLyric(const QString& RawlyricPath);


public slots:
    void OnSearchInProgram();

    void OnLyricResultChanged(LyricSearchResult result);

    void OnShowRawLyric(const LyricInfo& info);
    void OnShowLrcLyric(const LyricInfo& info);

    void OnSelectRawLyricSavePath();
    void OnSelectLrcLyricSavePath();
    void OnSaveRawLyric();
    void OnSavectLrcLyric();

private:
    void showTipLabel(bool bShow);

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


    BesLrcTableView* tableLyricSearch;
    QWidget* widgetRawLyricBoard;
    QWidget* widgetLrcLyricBoard;

    QLabel* labelRawLyricPanelSavePath;
    QLabel* labelRawLyricPanelSong;
    QLabel* labelRawLyricPanelArtist;
    QLineEdit* editRawLyricPanelSavePath;
    QLineEdit* editRawLyricPanelSong;
    QLineEdit* editRawLyricPanelArtist;
    BesButton* btnRawLyricPanelSelect;
    BesButton* btnRawLyricPanelSave;
    QTextEdit* editTextRawLyric;

    QLabel* labelLrcLyricPanelSavePath;
    QLabel* labelLrcLyricPanelSong;
    QLabel* labelLrcLyricPanelArtist;
    QLineEdit* editLrcLyricPanelSavePath;
    QLineEdit* editLrcLyricPanelSong;
    QLineEdit* editLrcLyricPanelArtist;
    BesButton* btnLrcLyricPanelSelect;
    BesButton* btnLrcLyricPanelSave;
    QTextEdit* editTextLrcLyric;

    ThreadSearchLyric searchThread;

private:
    QString strLastSongName;
    QString strLastArtistName;
};

#endif // SUBPAGEDOWNLOADLYRIC_H
