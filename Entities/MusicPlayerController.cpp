#include "MusicPlayerController.h"
#include <QDateTime>
#include <QtGlobal>

MusicPlayerController::MusicPlayerController(QObject *parent):QObject(parent)
{
}

void MusicPlayerController::addToHistory(QString music, QString lyric)
{
    for(auto iter = playHistory.begin(); iter != playHistory.end(); iter++)
    {
        if(iter->lyric == lyric && iter->song == music)
        {
            playHistory.erase(iter);
            break;
        }
    }

    playHistory.push_front(LyricListItem(music, lyric));

    emit sig_historyListDataChanged();
}

void MusicPlayerController::addToHistoryAndPlay(QString music, QString lyric)
{
    addToHistory(music, lyric);

    appendToPlayListAndPlay(LyricListItem(music, lyric));
}

void MusicPlayerController::appendToPlayListAndPlay(LyricListItem item)
{
    //加入到当前列表的最开始
    playList.push_front(item);

    regenerateRandomIndex();

    indexList = 0;

    emit sig_selectAndPlayPlayList(indexList);
}

void MusicPlayerController::setPlayListAndPlay(const QVector<LyricListItem> &newList, int indexToPlay)
{
    playList = newList;

    if(indexToPlay < 0)
        indexToPlay = 0;
    else if(indexToPlay >= playList.size())
        indexToPlay = playList.size()-1;

    indexList = indexToPlay;

    regenerateRandomIndex();

    emit sig_selectAndPlayPlayList(indexList);
}

void MusicPlayerController::toPlayHistory(int index)
{
    if(index < 0)
        index = 0;
    else if(index >= playHistory.size())
        index = playHistory.size()-1;

    addToHistoryAndPlay(playHistory[index].song, playHistory[index].lyric);
}

void MusicPlayerController::toPlayList(int index)
{
    if(index < 0)
        index = 0;
    else if(index >= playList.size())
        index = playList.size()-1;

    indexList = index;

    addToHistory(playList[indexList].song, playList[indexList].lyric);

    emit sig_selectAndPlayPlayList(indexList);
}

void MusicPlayerController::regenerateRandomIndex()
{
    listRandomIndex.clear();

    QVector<int> vecIndex;
    for(int i = 0; i < playList.size(); i++)
        vecIndex.push_back(i);

    QDateTime dateTime = QDateTime();
    qsrand((unsigned int)dateTime.currentMSecsSinceEpoch());

    while(vecIndex.size() != 0)
    {
        int selected = qrand() % vecIndex.size();
        listRandomIndex.push_back(vecIndex.at(selected));
        vecIndex.removeAt(selected);
    }

    indexRandomList =  0;
}

void MusicPlayerController::onEnterMakingLyricMode()
{
    bInMakingLyricMode = true;
}

void MusicPlayerController::onExitMakingLyricMode()
{
    bInMakingLyricMode = false;
}

void MusicPlayerController::onAudioFinished(bool isEndByForce)
{
    if(isEndByForce)            //对于非自然结束的音乐，不需要考虑自动播放的事
        return;

    onPlayNext(false);          //播放下一首
}

void MusicPlayerController::onPlayNext(bool bPreviousOne)
{
    if(bInMakingLyricMode)      //处于制作歌词状态时，也不需要考虑自动播放的事
        return;

    if(playList.size() == 0)    //没有歌曲直接停止所有音乐
    {
        //emit sig_stopPlaying();
        return;
    }

    int nPlayMode = SettingManager::GetInstance().data().playMode;

    switch (nPlayMode) {
    case 1:         //【1 单曲循环】
        emit sig_selectAndPlayPlayList(indexList);
        return;
    case 2:         //【2 随机播放】
        if(bPreviousOne)
            indexRandomList--;
        else
            indexRandomList++;

        if(indexRandomList < 0)
            indexRandomList = listRandomIndex.size()-1;
        else if(indexRandomList >= listRandomIndex.size())
            indexRandomList = 0;

        emit sig_selectAndPlayPlayList(listRandomIndex[indexRandomList]);

        return;
    case 3:         //【3 顺序播放】
        indexList++;
        if(indexList >= playList.size())
        {
            indexList = playList.size();
            //emit sig_stopPlaying();
        }
        else
        {
            emit sig_selectAndPlayPlayList(indexList);
        }

        return;
    case 4:         //【4 列表循环】
        indexList++;
        if(indexList >= playList.size())
            indexList = 0;

        emit sig_selectAndPlayPlayList(indexList);
        return;
    case 0:         //【0 单曲播放】
    default:
        //emit sig_stopPlaying();
        return;
    }
}

void MusicPlayerController::onPlay(int index)
{
    if(playList.size() == 0)
        return;

    if(index < 0)
        index = 0;
    else if(index >= playList.size())
        index = playList.size()-1;

    indexList = index;

    emit sig_playMusicAndLyric(playList[indexList].song, playList[indexList].lyric);
}
