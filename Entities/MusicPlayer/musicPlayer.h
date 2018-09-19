#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include <QObject>
#include <QString>
#include <QThread>
#include <QPixmap>
#include <QTimer>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "SDL.h"
}

#include <stdlib.h>  //使用 _sleep();

#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include "SDL.h"
#ifdef __cplusplus
}
#include <unistd.h>
#endif
#endif

#define FLUSH_DATA "FLUSH"


//自定义的音频产生方式的状态
enum AudioGenStatus{
   AGS_PLAYING,         //正常播放
   AGS_SEEK,            //定位
   AGS_FINISH,          //结束
};


//音频包队列，提供音频原数据
typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;

	void clear()
	{
		first_pkt = nullptr;
		last_pkt = nullptr;
		mutex = nullptr;
		cond = nullptr;
		nb_packets = size = 0;
	}
} PacketQueue;


class PlayThread;
typedef struct{
    AVFormatContext* fct; //格式上下文

    AVFrame *wanted_frame;//音频目标帧
    AVCodecContext *acct;//音频解码上下文
	AVStream *aStream;
	AVStream *vStream;
    PacketQueue audioq; //音频队列

    double audio_clock; //储存毫秒时间
    uint8_t volume;

    PlayThread* playThread;

	void clear() //仅仅是清空本结构内容，不负责清空堆栈中的内存
	{
		fct = nullptr;
		wanted_frame = nullptr;
		acct = nullptr;
		aStream = nullptr;
		vStream = nullptr;
		fct = nullptr;
		audioq.clear();
		audio_clock = 0;
        volume = 128;
        playThread = nullptr;
	}

}mediaState;


//播放音乐使用的线程
class PlayThread: public QThread
{
    friend class MusicPlayer;
    Q_OBJECT
public:
    PlayThread(QObject* parent = nullptr):QThread(parent) {bIsDeviceInit = false;}

protected:
    virtual void run();

signals:
    void audioPlay();               //播放
    void audioPause();              //暂停
    void audioFinish();
    void volumeChanged(uint8_t);    //音量发生改变
    void durationChanged(qint64);   //总长发生改变（单位 微秒 10e-6）
    void errorOccur(int errorCode, QString errorMessage);

    void albumFound(QString);       //发现信息
    void artistFound(QString);
    void titleFound(QString);
    void pictureFound(QPixmap);

    void threadDone();              //线程结束

private:
    //SDL 模块需要的回调函数，用于填充需要的音频数据
    static void fillAudio(void *udata,Uint8 *stream,int len);

    int audio_decode_frame(mediaState* MS, uint8_t* audio_buf);

    bool initDeviceAndFfmpegContext();  //尝试初始化播放设备 和 ffmpeg 上下文

    void playDevice();                  //启动播放设备

    void pauseDevice();                 //暂停播放设备

    void generateAudioDataLoop();       //生产音频数据的循环

    void clearContextAndCloseDevice();  //清空上下文并关闭设备

    SDL_AudioStatus GetDeviceStatus();  //获得设备状态

    void ResetToInitAll();              //重置以初始化所有状态
    void ReleaseAll();                  //释放所有可能分配的内存

private:
    bool getIsDeviceInit();             //实现互斥访问 isDeviceInit 的接口

private:

    AVFormatContext	*pFormatCtx;
    int	audioStream, videoStream;
    AVCodecContext	*pCodecCtx;
    AVCodec			*pCodec;
    uint8_t			*out_buffer;
    SDL_AudioSpec wanted_spec;
    int ret;
    int got_picture;
    int index = 0;
    int64_t in_channel_layout;
    struct SwrContext *au_convert_ctx;

    QString musicPath;


    //Out Audio Param
    uint64_t out_channel_layout;
    //nb_samples: AAC-1024 MP3-1152
    int out_nb_samples;
    AVSampleFormat out_sample_fmt;
    int out_sample_rate;
    int out_channels;
    //Out Buffer Size
    int out_buffer_size;

private:

    /*duration with now playing the media */
    inline qint64 getDuration(){ if(m_MS.fct)return m_MS.fct->duration;return 0;}

    /*get current media time  millisecond*/
    inline int getCurrentTime(){return static_cast<int>(m_MS.audio_clock);}


    mediaState m_MS;                    //保存音频相关的上下文状态

private:
    QString album;
    QString title;
    QString artist;
    QPixmap picture;

    //过程控制数据
private:

    AudioGenStatus AGStatus;            //音频产生方式状态
    bool bIsDeviceInit;                 //设备是否已经初始化
    uint64_t millisecondToSeek;         //定位的 毫秒数）
};


//音乐播放器
class MusicPlayer :public QObject
{
    Q_OBJECT

public:
    enum Status
    {
        StoppedState,
        PlayingState,
        PausedState
    };

    MusicPlayer(QObject* parent = nullptr);
    ~MusicPlayer() ;

    void setMusicPath(QString path);
    QString getMusicPath();

    //音乐文件信息
    QString getTitle();
    QString getArtist();
    QString getAlbum();
    QPixmap getPicture();

signals:
    void audioPlay();               //播放
    void audioPause();              //暂停
    void audioFinish();             //播放完毕
    void durationChanged(qint64);   //总长发生改变（单位 毫秒）
    void positionChanged(int);   //位置发生改变（单位 毫秒）
    void volumeChanged(int);        //音量大小发生改变，范围 0-128
    void errorOccur(QString msg);   //发生错误

    void albumFound(QString);       //发现信息
    void artistFound(QString);
    void titleFound(QString);
    void pictureFound(QPixmap);

public slots:
    //播放控制
    void reload();                   //stop() 并 play();
    void play();
    void pause();
    void stop();
    void seek(quint64 pos);          //跳到时间点播放（单位 毫秒）
    void forwordSeek(quint64 step);  //往后跳（单位 毫秒）
    void backwardSeek(quint64 step); //往回跳（单位 毫秒）

    void setVolume(int volume);  //音量大小范围 0-128
    int getVolume();
    quint64 duration();  //获得当总时长（单位 毫秒）
    quint64 position();  //获得当总位置（单位 毫秒）

    void setNotifyInterval(int msec);   //设置通知间隔（歌曲位置进度）
    Status state();

private slots:
    void sendPosChangedSignal();


private:
    //歌曲文件信息
    QString m_album;
    QString m_title;
    QString m_artist;
    QPixmap m_picture;

private:
    QString musicPath;
    quint64 m_position;              //当前时间（单位 毫秒）

    QTimer  m_positionUpdateTimer;    //通知歌曲进度发生改变的Timer
    int     m_interval;                //间隔，单位毫秒

    PlayThread* playThread;

};

#endif // MUSIC_PLAYER_H
