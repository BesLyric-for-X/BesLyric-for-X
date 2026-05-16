#include "SearcherLrcLib.h"
#include "Utility/NetAccess.h"
#include "LrcProcessor.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

bool SearcherLrcLib::SearchLyric(QString strSong, QString strArtist, QVector<LyricInfo>& vecLyricInfo)
{
    QUrlQuery query;
    query.addQueryItem("track_name", strSong);
    if (!strArtist.isEmpty())
        query.addQueryItem("artist_name", strArtist);

    QString strResult;
    if (!NetworkAccess::SyncDownloadString("https://lrclib.net/api/search", strResult, query)) {
        m_strLastResult = tr("网络连接失败，无法访问 LRCLIB");
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(strResult.toUtf8());
    if (!doc.isArray()) {
        m_strLastResult = tr("LRCLIB 数据格式异常");
        return false;
    }

    QJsonArray arr = doc.array();
    for (int i = 0; i < arr.size(); i++) {
        QJsonObject obj = arr.at(i).toObject();

        QString syncedLyrics = obj.value("syncedLyrics").toString();
        if (syncedLyrics.isEmpty())
            continue;

        LrcProcessor processor;
        processor.LoadFromRawText(syncedLyrics);
        QVector<QPair<int, QString>> vecLines = processor.GetLrcLyric();
        if (vecLines.isEmpty())
            continue;

        LyricInfo info;
        info.strSong   = obj.value("trackName").toString();
        info.strArtist = obj.value("artistName").toString();
        info.strLyricFrom = tr("LRCLIB");

        QString plain, lrc;
        for (auto& pair : vecLines) {
            plain += pair.second + "\n";
            lrc   += LrcProcessor::ToLrcLine(pair) + "\n";
        }
        info.strPlaneText = plain;
        info.strLabelText = lrc;

        vecLyricInfo.push_back(info);
    }

    m_strLastResult = tr("找到 %1 条 LRCLIB 歌词").arg(vecLyricInfo.size());
    return true;
}
