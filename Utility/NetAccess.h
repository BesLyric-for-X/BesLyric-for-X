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



class NetworkAccess :public QObject
{
    Q_OBJECT
public:
    NetworkAccess(){}

    /*
    *	@brief	下载到文件
    *	@param	strUrl			下载链接
    *	@param	strSaveAs		保存文件的路径
    *	@return	true -- 下载成功
    */
    bool DownloadFile(const QString strUrl, const QString strSaveAs);

    /*
    *	@brief	下载到字符串
    *	@param	strUrl				下载链接
    *	@param	strSaveBuffer		保存字符串的字符串
    *	@return	true -- 下载成功
    */
    bool DownloadString(const QString strUrl, QString& strSaveBuffer);

    /* get 请求下载，下载完成后才返回 */
    static bool SyncDownloadString(const QString strUrl, QString& strSaveBuffer, QUrlQuery query = QUrlQuery());

    /* post 请求下载，下载完成后才返回 */
    static bool SyncDownloadStringPost(const QString strUrl, QString& strSaveBuffer, QByteArray queryData = QByteArray());

public slots:

    void replyFinished(QNetworkReply *reply);
	
    void onDownloadProgress(qint64 bytesSent,qint64 bytesTotal);

    void onReadyRead();

public:

    QNetworkReply *reply;
    bool bDownloadResult;
    bool bDownloadFinish;
    QString* pStrBuffer;
};

