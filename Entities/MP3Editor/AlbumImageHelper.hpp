#ifndef H_AlbumImageHelper_hpp
#define H_AlbumImageHelper_hpp

#include "LyricListManager.h"
#include <QRunnable>
#include <QDebug>

extern "C" {
    #include "ffmpegDefine.h"
}

class AlbumImageHelper :public QObject, public QRunnable
{
    Q_OBJECT
public:
    AlbumImageHelper* SetDataAndGetPointer(const LyricList& list)
    {
        lyricList = list;
        return this;
    }

    virtual void run()
    {
        qDebug()<<"start running AlbumImageHelper";
        QPixmap pixmap;
        if(GetFirstAvailableAlbumImage(lyricList, pixmap))
        {
            qDebug()<<"emit sig_lyricListAblumFound [lyricList.id]:" << lyricList.id;
            emit sig_lyricListAblumFound(lyricList.id, pixmap);
        }
    }

signals:
    void sig_lyricListAblumFound(int listId, QPixmap pixmap);

private:
    bool GetFirstAvailableAlbumImage(const LyricList& list, QPixmap& pixmap)
    {
        for(auto& item:list.items)
        {
            if(GetSongAlbum(item.song, pixmap))
                return true;
        }

        return false;
    }

    bool GetSongAlbum(const QString& song, QPixmap& pixmap)
    {
        //获得文件路径
        char url[1024];
        strcpy_s(url, song.toUtf8());

        AVFormatContext	*pFormatCtx = avformat_alloc_context();

        //打开输入流
        if(avformat_open_input(&pFormatCtx,url,NULL,NULL)!=0){
            qDebug()<<"Couldn't open input stream.";
            return false;
        }

        // 获取流信息
        if(avformat_find_stream_info(pFormatCtx,NULL)<0){
            qDebug()<<"Couldn't find stream information.";
            return false;
        }

        //这里仅仅读取 wav 或 mp3 格式的文件的图片信息
        bool bFound = false;
        if (strcmp(pFormatCtx->iformat->name, "wav") == 0 || strcmp(pFormatCtx->iformat->name, "mp3") == 0)
        {
            //读取专辑图片
            for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
                if (pFormatCtx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
                    AVPacket pkt = pFormatCtx->streams[i]->attached_pic;
                    //使用QImage读取完整图片数据（注意，图片数据是为解析的文件数据，需要用QImage::fromdata来解析读取）
                    QImage img = QImage::fromData((uchar*)pkt.data, pkt.size);
                    pixmap = QPixmap::fromImage(img);

                    bFound = true;
                    break;
                }
            }
        }

        avformat_close_input(&pFormatCtx);

        return bFound;
    }

    LyricList lyricList;
};

#endif //!H_AlbumImageHelper_hpp
