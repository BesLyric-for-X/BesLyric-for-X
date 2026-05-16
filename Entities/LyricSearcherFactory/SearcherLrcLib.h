#pragma once
#include "ISearcher.h"
#include <QObject>
#include <QVector>
#include <QString>

class SearcherLrcLib : public ISearcher, public QObject
{
public:
    virtual ~SearcherLrcLib() {}
    virtual bool SearchLyric(QString strSong, QString strArtist, QVector<LyricInfo>& vecLyricInfo);
};
