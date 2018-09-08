#include "NetAccess.h"


bool NetworkAccess::DownloadFile(const QString strUrl, const QString strSaveAs)
{

    return true;
}

bool NetworkAccess::DownloadString(const QString strUrl, QString &strSaveBuffer)
{
    //        QUrl url(strUrl);
    //        QFileInfo fileInfo = url.path();
    //        file =new QFile(fileInfo.fileName());
    //        file->open(QIODevice::WriteOnly);//只读方式打开文件

    //        /******************设置http的header***********************/
    //        // request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
    //        // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    //        // request.setRawHeader("Content-Disposition","form-data;name='doc';filename='a.txt'");
    //        //request.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());

    //        request.setUrl(QUrl("https://images2017.cnblogs.com/blog/683006/201710/683006-20171026142646008-1509778425.png"));

    pStrBuffer = &strSaveBuffer;
    bDownloadResult = false;


    QNetworkAccessManager*  manager = new QNetworkAccessManager(this);

    QNetworkRequest request;
    request.setUrl(QUrl(strUrl));

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

    reply = manager->get(request);

    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT( onDownloadProgress(qint64 ,qint64 )));//download文件时进度
    connect(reply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));



    //downloadDone.wait(&mutex);
    bDownloadFinish = false;
    while(!bDownloadFinish)
        _sleep(1);

    return bDownloadResult;
}

bool NetworkAccess::SyncDownloadString(const QString strUrl, QString &strSaveBuffer, QUrlQuery query)
{
    QUrl url(strUrl);
    if(!query.isEmpty())
        url.setQuery(query);

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Accept","*/*");
    request.setRawHeader("Accept-Language","zh-CN");
    request.setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("Accept-Encoding","deflate");
    //request.setRawHeader("Host","i.y.qq.com");
    //request.setRawHeader("Cookie","qqmusic_fromtag=3; qqmusic_miniversion=57; qqmusic_version=12;");

    QNetworkAccessManager manager;
    QNetworkReply *reply= manager.get(request);
    QEventLoop loop;
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)), &loop,SLOT(quit()));
    loop.exec();

    bool bRet = false;
    if(reply->error()==QNetworkReply::NoError)
    {
        QByteArray byte=reply->readAll();
        strSaveBuffer = QString(byte);
        bRet = true;
    }
    reply->deleteLater();

    return bRet;
}

bool NetworkAccess::SyncDownloadStringPost(const QString strUrl, QString &strSaveBuffer, QByteArray queryData)
{
    QNetworkRequest request;
    request.setUrl(strUrl);
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");

    QNetworkAccessManager manager;
    QNetworkReply *reply= manager.post(request,queryData);
    QEventLoop loop;
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
    connect(reply,SIGNAL(error(QNetworkReply::NetworkError)), &loop,SLOT(quit()));
    loop.exec();

    bool bRet = false;
    if(reply->error()==QNetworkReply::NoError)
    {
        QByteArray byte=reply->readAll();
        strSaveBuffer = QString(byte);
        bRet = true;
    }
    reply->deleteLater();

    return bRet;
}

void NetworkAccess::replyFinished(QNetworkReply *reply){
    //获取响应的信息，状态码为200表示正常
    //        QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //        QByteArray bytes = reply->readAll();  //获取字节
    //        QString result(bytes);  //转化为字符串
    //        qDebug()<<result;

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        qDebug()<<"success";
        QByteArray bytes = reply->readAll();  //获取字节
        *pStrBuffer =  QString(bytes);  //转化为字符串
        qDebug()<<*pStrBuffer;

        bDownloadResult = true;
    }
    else
    {
        //处理错误
        qDebug()<<"failed";
        bDownloadResult = false;
    }

    //reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;

    //downloadDone.wakeAll();

    bDownloadFinish = true;
}

void NetworkAccess::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug()<<"bytesSent/bytesTotal:" << bytesSent <<"/"<< bytesTotal << "\n";
}

void NetworkAccess::onReadyRead(){
    //file->write(reply->readAll());
    qDebug()<<"downloading...... \n";
}
