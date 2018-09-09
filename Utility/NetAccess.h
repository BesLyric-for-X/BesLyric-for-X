#pragma once
#include <QString>
#include <Qurl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QEventLoop>
#include <QUrlQuery>
#include <QQueue>

enum DOWNLOAD_FINISH_STATUS{
    NORMAL,
    NETEASE_MUSIC_NOT_FOUND
};

class DownloadInfo
{
public:
    QString strUrl;
    QString strSaveFilePath;
    QVariant data;
};

class NetworkAccess :public QObject
{
    Q_OBJECT
public:
    NetworkAccess(){}

    /*
    *	@brief	下载到文件
    *	@param	strUrl			下载链接
    *	@param	strSaveAs		保存文件的路径
    *   @param  data            用户自定义数据，向外发送消息时，发送的参数
    *	@return	true -- 下载成功
    */
    bool DownloadFile(const QString strUrl, const QString strSaveAs, QVariant data);


    /* get 请求下载，下载完成后才返回 */
    static bool SyncDownloadString(const QString strUrl, QString& strSaveBuffer, QUrlQuery query = QUrlQuery());

    /* post 请求下载，下载完成后才返回 */
    static bool SyncDownloadStringPost(const QString strUrl, QString& strSaveBuffer, QByteArray queryData = QByteArray());

signals:
    void finished();
    void sig_finishDownload(QVariant data, DOWNLOAD_FINISH_STATUS Status);
    void sig_progressChanged(QString speed, int percentage, QVariant data);
    void sig_netErrorWhenDownload(QVariant data);
    void sig_fileOpenErrorWhenSave(QVariant data);

public slots:

    void replyFinished(QNetworkReply *reply);
    void onDownloadProgress(qint64 bytesSent,qint64 bytesTotal);
    void onReadyRead();

    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

public:

    QNetworkAccessManager manager;
    QQueue<DownloadInfo> downloadQueue;
    QNetworkReply *currentDownload;

    QFile output;
    QTime downloadTime;

    bool bDownloadResult;
    bool bDownloadFinish;
    QString* pStrBuffer;
};

