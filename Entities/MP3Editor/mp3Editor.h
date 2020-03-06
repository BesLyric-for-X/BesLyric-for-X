#ifndef MP3_EDITOR_H
#define MP3_EDITOR_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QPixmap>
#include <QTimer>
#include <QMutex>

extern "C" {
    #include "ffmpegDefine.h"
}

class CustomMp3Data
{
public:
    //输入参数
    QString imagePath;
    QString title;
    QString artist;
    QString album;

    //输出参数
    QString mp3OutputPath;

    void clear()
    {
        imagePath = title = artist = album = mp3OutputPath = "";
    }

    bool isEmpty()
    {
        return (imagePath.isEmpty() &&title.isEmpty() &&artist.isEmpty() &&album.isEmpty());
    }
};

//转换线程
class ConvertThread: public QThread
{
    Q_OBJECT
public:
    ConvertThread(QObject* parent = nullptr):QThread(parent) {}

    void SetConvertedData(QString filePath, const CustomMp3Data &customMp3Data);
	int GetProcessingPercentage();
signals:
    void sig_getEditResult(bool success, QString path, QString errorTip);

protected:
    virtual void run();

private:
    void ResetToInitAll();
    void ReleaseAll();

    //初始化选项内容（仿造ffmpeg.exe）
    int ffmpeg_parse_options();

    void init_opts(void);
    void uninit_opts(void);

    static void uninit_options(OptionsContext *o);

    static void init_options(OptionsContext *o);


    void buildOptionContent(OptionParseContext* octx);  //模拟 split_commandline 构建 OptionParseContext

    void init_parse_context(OptionParseContext *octx,
                            const OptionGroupDef *groups, int nb_groups);

    void add_input_streams(OptionsContext *o, AVFormatContext *ic);

    int open_files(OptionGroupList *l, bool isInput);
    int open_input_file(OptionsContext *o, const char *filename);
    int open_output_file(OptionsContext *o, const char *filename);

    int init_input_stream(int ist_index);
    InputStream *get_input_stream(OutputStream *ost);
    int init_output_stream_streamcopy(OutputStream *ost);
    int check_init_output_file(OutputFile *of, int file_index);
    int init_output_stream(OutputStream *ost/*, char *error, int error_len*/);
    int transcode_init();
    void close_output_stream(OutputStream *ost);
    int need_output(void);
    OutputStream *choose_output(void);
    int get_input_packet(InputFile *f, AVPacket *pkt);
    void reset_eagain(void);
    void report_new_stream(int input_index, AVPacket *pkt);

    int sub2video_get_blank_frame(InputStream *ist);
    void sub2video_copy_rect(uint8_t *dst, int dst_linesize, int w, int h,AVSubtitleRect *r);
    void sub2video_push_ref(InputStream *ist, int64_t pts);
    void sub2video_update(InputStream *ist, AVSubtitle *sub);
    void sub2video_heartbeat(InputStream *ist, int64_t pts);

    int check_output_constraints(InputStream *ist, OutputStream *ost);
    void close_all_output_streams(OutputStream *ost, OSTFinished this_stream, OSTFinished others);
    void write_packet(OutputFile *of, AVPacket *pkt, OutputStream *ost, int unqueue);
    void output_packet(OutputFile *of, AVPacket *pkt,OutputStream *ost, int eof);
    void do_streamcopy(InputStream *ist, OutputStream *ost, const AVPacket *pkt);
    int process_input_packet(InputStream *ist, const AVPacket *pkt, int no_eof);

    void print_final_stats(int64_t total_size);
    void print_report(int is_last_report, int64_t timer_start, int64_t cur_time);

    void finish_output_stream(OutputStream *ost);
    int process_input(int file_index);
    int transcode_step(void);
    int transcode();


    void hw_device_free_all(void);
private:
    void avformat_get_context_defaults(AVFormatContext *s);

    /*
     * Finish parsing an option group.
     *
     * @param group_idx which group definition should this group belong to
     * @param arg argument of the group delimiting option
     */
    void finish_group(OptionParseContext *octx, int group_idx,
                             const char *arg);


    /*
     * Add an option instance to currently parsed group.
     */
    void add_opt(OptionParseContext *octx, const OptionDef *opt,
                        const char *key, const char *val);


    void print_error(const char *filename, int err);


    void remove_avoptions(AVDictionary **a, AVDictionary *b);

    void assert_avoptions(AVDictionary *m);

    int read_yesno(void);

    void assert_file_overwrite(const char *filename);

    void uninit_parse_context(OptionParseContext *octx);


private:
    QString inputMp3Path;
    CustomMp3Data customData;

    QByteArray inputMp3Utf8;
    QByteArray inputImageUtf8;
    QByteArray outputMp3Utf8;

    QByteArray mataArtistUtf8;
    QByteArray mataTitleUtf8;
    QByteArray mataAlbumUtf8;

    QString outputMp3Path;
private:
    AVDictionary *sws_dict;
    AVDictionary *swr_opts;
    AVDictionary *format_opts, *codec_opts, *resample_opts;

#define OFFSET(x) offsetof(OptionsContext, x)
    OptionDef optionMap = { "map", HAS_ARG | OPT_EXPERT | OPT_PERFILE |OPT_OUTPUT,{(void*)opt_map},   // { .func_arg = opt_map },
            "set input stream mapping",
            "[-]input_file_id[:stream_specifier][,sync_file_id[:stream_specifier]]" };
    OptionDef optionCodecName = { "c", HAS_ARG | OPT_STRING | OPT_SPEC |OPT_INPUT | OPT_OUTPUT,   {(void*)OFFSET(codec_names)} ,//{ .off       = OFFSET(codec_names) },
                                  "codec name", "codec" };
    OptionDef optionMetadata = { "metadata", HAS_ARG | OPT_STRING | OPT_SPEC | OPT_OUTPUT, {(void*)OFFSET(metadata)},//{ .off = OFFSET(metadata) },
                                 "add metadata", "string=string" };

    FfmpegParamContext paramCtx;
};

//音乐播放器
class Mp3Editor :public QObject
{
    Q_OBJECT

public:
    Mp3Editor(QObject* parent = nullptr);
    ~Mp3Editor() ;

    bool CustomizeMp3(QString filePath,const CustomMp3Data& customData);

signals:
    void sig_getEditResult(bool success, QString path, QString errorTip);
	void sig_processChange(int percentage);
private:
    void OnGetEditReuslt(bool success, QString path, QString errorTip);
	void OnSendProcessing();
private:
    QString sourceMp3;
    ConvertThread* convertThread;

	QTimer  processTimer;    //通知进度改变的 Timer
	int nLastPercentage = 0;
};

#endif // MP3_EDITOR_H
