#ifndef BES_NCM_SONG_TABLEVIEW_H
#define BES_NCM_SONG_TABLEVIEW_H

#include "Define/Define.h"
#include "BesNcmSongTableModel.h"
#include "BesNcmSongButtonDelegate.h"
#include "NetAccess.h"
#include "Thread/ThreadConvertMp3.h"

#include <QTableView>
#include <QVector>

class BesNcmSongTableView : public QTableView
{
    Q_OBJECT
public:
    explicit BesNcmSongTableView(QWidget *parent = 0);
    ~BesNcmSongTableView();

    BesNcmSongTableModel* tableModel() {return m_model;}

public:
    void appendItems(const QVector<SONGINFO>& infos);
    void setItems(const QVector<SONGINFO>& infos);

signals:
    void sig_showRawLyric(const SONGINFO&);
    void sig_showLrcLyric(const SONGINFO&);
    void sig_setMusicPathToMakingPage(QString musicPath);

    void sig_oneDownloadStarted();
    void sig_oneDownloadFinished();

public slots:
    void OnDownloadNcmMusic(SONGINFO songInfo);
    void OnFinishedDownload(QVariant, DOWNLOAD_FINISH_STATUS);
    void OnProgressChanged(QString speed, int percentage, QVariant data);

    void OnFinishConversion(int ncmId);

protected:
    void initEntity();
    void initConnection();
    virtual void resizeEvent(QResizeEvent *event);

private:

    QVector<SONGINFO> initSongInfo(const QVector<SONGINFO>& infos);

private:
    void BaseInit(); //基础的初始化

private:
    BesNcmSongTableModel *m_model;
    BesNcmSongButtonDelegate *m_buttonDelegate;
    NetworkAccess net;
    ThreadConvertMp3 mp3Converter;
};

#endif // BES_NCM_SONG_TABLEVIEW_H
