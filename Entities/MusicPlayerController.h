#ifndef MusicPlayerController_H
#define MusicPlayerController_H

#include <QObject>
#include "LyricListManager.h"
#include "SettingManager.h"

//音乐播放器控制器
//所有的播放列表相关的操作，都由 MusicPlayerController 控制执行
class MusicPlayerController: public QObject
{
    Q_OBJECT
public:
    MusicPlayerController(QObject* parent = nullptr);

    void addToHistory(QString music, QString lyric);
    void addToHistoryAndPlay(QString music, QString lyric);

    void appendToPlayListAndPlay(LyricListItem item);
    void setPlayListAndPlay(const QVector<LyricListItem>& newList, int indexToPlay);

    //外部选择列表中的项后触发
    void toPlayHistory(int index);
    void toPlayList(int index);

private:
    void regenerateRandomIndex(); //重新生成随机序列

signals:
    void sig_historyListDataChanged();
    void sig_playMusicAndLyric(QString music, QString lyric);
    void sig_selectAndPlayPlayList(int index);                   //选择播放列表，由播放列表的选择触发播放操作
    void sig_stopPlaying();                                      //停止音乐的播放

public slots:
    void onEnterMakingLyricMode();
    void onExitMakingLyricMode();
    void onAudioFinished(bool isEndByForce);
    void onPlayNext(bool bPreviousOne = false);
    void onPlay(int index);                                 //播放列表中的指定项

private:
    bool bInMakingLyricMode;            //标记是否正在制作歌词的模式中

    QVector<LyricListItem>   playHistory;    //播放历史(播放历史仅仅是记录，并不作为一个实质列表，没有播放模式的概念)

    QVector<LyricListItem>   playList;       //当前播放列表
    int indexList;                           //当前播放列表下标
    QVector<int>   listRandomIndex;          //储存自动生成的随机播放序列（播放列表）
    int indexRandomList;                     //当前随机序列下标（播放列表）
};




#endif //MusicPlayerController_H
