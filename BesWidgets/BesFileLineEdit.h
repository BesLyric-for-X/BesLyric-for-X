#pragma once
#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QList>
#include <QMimeData>
#include <QString>
#include <QDir>
#include <QFile>


enum BesEditFileType
{
    BesFileTypeMusic,
    BesFileTypeTxt,
    BesFileTypeLrc,
    BesFileTypeFloder,
    BesFileTypeAll
};

class BesFileLineEdit :public QLineEdit
{
    Q_OBJECT

public:
    BesFileLineEdit(BesEditFileType fileType = BesFileTypeAll, QWidget* parent = nullptr):
        QLineEdit(parent),m_fileType(fileType){}

    void dragEnterEvent(QDragEnterEvent *event)
    {
        if (event->mimeData()->hasUrls())
        {
            if(toTargetTypeStringList(event->mimeData()->urls()).size() != 0)
                event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent *event)
    {
        QList<QString> list = toTargetTypeStringList(event->mimeData()->urls());

        emit sig_filesHaveBeenDrop(list);
    }

signals:
    void sig_filesHaveBeenDrop(QList<QString> list);


private:

    QList<QString> toTargetTypeStringList(QList<QUrl> urlList)
    {
        QList<QString> list;
        for(auto url: urlList)
        {
            QString str = toFileString(url);
            if(isType(str,m_fileType))
                list.push_back(str);
        }

        return list;
    }

    bool isType(QString pathString ,BesEditFileType fileType)
    {
        if(pathString.size() == 0)
            return false;

        bool isFile = QFile(pathString).exists();
        bool isFloder = QDir(pathString).exists();
        pathString = pathString.toLower();

        switch (fileType) {
        case BesFileTypeMusic:
            if( isFile && !isFloder && (pathString.contains(".mp3")
                           || pathString.contains(".wav")
                           || pathString.contains(".ncm") ))
                return true;
            break;
        case BesFileTypeTxt:
            if( isFile && !isFloder &&  (pathString.contains(".txt")))
                return true;
            break;
        case BesFileTypeLrc:
            if( isFile && !isFloder && (pathString.contains(".lrc")))
                return true;
            break;
        case BesFileTypeFloder:
            if( isFloder)
                return true;
            break;
        default: //BesFileTypeAll
            if(isFile || isFloder)
                return true;
            break;
        }

        return false;
    }

    QString toFileString(QUrl url)
    {
       QString str = url.toString();

       QString filePrefix = "file:///";
       if(str.contains(filePrefix))
       {
           str = str.right(str.size() - filePrefix.size());
       }
       else
           str = "";

       return str;
    }


    BesEditFileType m_fileType;
};

