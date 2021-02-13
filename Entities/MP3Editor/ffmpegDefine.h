#ifndef FFMPEG_DEFINI_H
#define FFMPEG_DEFINI_H

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#define __STDC_FORMAT_MACROS
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/mem.h>
#include <libavutil/avstring.h>
#include <libavutil/intreadwrite.h>
#include <libavutil/parseutils.h>
#include <libavutil/pixdesc.h>
#include <libavutil/eval.h>
#include <libavutil/fifo.h>
#include <libavutil/time.h>
#include <libavutil/timestamp.h>
#include <libavutil/bprint.h>

#ifdef _WIN32
//Windows
#include <stdlib.h>  //使用 _sleep();

#else
//Linux...
#include <unistd.h>
#endif
#ifdef __cplusplus
}
#endif


#define VSYNC_AUTO       -1
#define VSYNC_PASSTHROUGH 0
#define VSYNC_CFR         1
#define VSYNC_VFR         2
#define VSYNC_VSCFR       0xfe
#define VSYNC_DROP        0xff
#define DEFAULT_PASS_LOGFILENAME_PREFIX "ffmpeg2pass"


typedef struct InputStream {
    int file_index;
    AVStream *st;
    int discard;             /* true if stream data should be discarded */
    int user_set_discard;
    int decoding_needed;     /* non zero if the packets must be decoded in 'raw_fifo', see DECODING_FOR_* */
#define DECODING_FOR_OST    1
#define DECODING_FOR_FILTER 2

    AVCodecContext *dec_ctx;
    AVCodec *dec;
    AVFrame *decoded_frame;
    AVFrame *filter_frame; /* a ref of decoded_frame, to be sent to filters */

    int64_t       start;     /* time when read started */
    /* predicted dts of the next packet read for this stream or (when there are
     * several frames in a packet) of the next frame in current packet (in AV_TIME_BASE units) */
    int64_t       next_dts;
    int64_t       dts;       ///< dts of the last packet read for this stream (in AV_TIME_BASE units)

    int64_t       next_pts;  ///< synthetic pts for the next decode frame (in AV_TIME_BASE units)
    int64_t       pts;       ///< current pts of the decoded frame  (in AV_TIME_BASE units)
    int           wrap_correction_done;

    int64_t filter_in_rescale_delta_last;

    int64_t min_pts; /* pts with the smallest value in a current stream */
    int64_t max_pts; /* pts with the higher value in a current stream */

    // when forcing constant input framerate through -r,
    // this contains the pts that will be given to the next decoded frame
    int64_t cfr_next_pts;

    int64_t nb_samples; /* number of samples in the last decoded audio frame before looping */

    double ts_scale;
    int saw_first_ts;
    AVDictionary *decoder_opts;
    AVRational framerate;               /* framerate forced with -r */
    int top_field_first;
    int guess_layout_max;

    int autorotate;

    int fix_sub_duration;
    struct { /* previous decoded subtitle and related variables */
        int got_output;
        int ret;
        AVSubtitle subtitle;
    } prev_sub;

    struct sub2video {
        int64_t last_pts;
        int64_t end_pts;
        //AVFifoBuffer *sub_queue;    ///< queue of AVSubtitle* before filter init
        AVFrame *frame;
        int w, h;
    } sub2video;

    int dr1;

    /* decoded data from this stream goes into all those filters
     * currently video and audio only */
    //InputFilter **filters;
    int        nb_filters;

    int reinit_filters;

    /* hwaccel options */
    //enum HWAccelID hwaccel_id;
    enum AVHWDeviceType hwaccel_device_type;
    char  *hwaccel_device;
    enum AVPixelFormat hwaccel_output_format;

    /* hwaccel context */
    void  *hwaccel_ctx;
    void (*hwaccel_uninit)(AVCodecContext *s);
    int  (*hwaccel_get_buffer)(AVCodecContext *s, AVFrame *frame, int flags);
    int  (*hwaccel_retrieve_data)(AVCodecContext *s, AVFrame *frame);
    enum AVPixelFormat hwaccel_pix_fmt;
    enum AVPixelFormat hwaccel_retrieved_pix_fmt;
    AVBufferRef *hw_frames_ctx;

    /* stats */
    // combined size of all the packets read
    uint64_t data_size;
    /* number of packets successfully read for this stream */
    uint64_t nb_packets;
    // number of frames/samples retrieved from the decoder
    uint64_t frames_decoded;
    uint64_t samples_decoded;

    int64_t *dts_buffer;
    int nb_dts_buffer;

    int got_output;
} InputStream;

typedef struct InputFile {
    AVFormatContext *ctx;
    int eof_reached;      /* true if eof reached */
    int eagain;           /* true if last read attempt returned EAGAIN */
    int ist_index;        /* index of first stream in input_streams */
    int loop;             /* set number of times input stream should be looped */
    int64_t duration;     /* actual duration of the longest stream in a file
                             at the moment when looping happens */
    AVRational time_base; /* time base of the duration */
    int64_t input_ts_offset;

    int64_t ts_offset;
    int64_t last_ts;
    int64_t start_time;   /* user-specified start time in AV_TIME_BASE or AV_NOPTS_VALUE */
    int seek_timestamp;
    int64_t recording_time;
    int nb_streams;       /* number of stream that ffmpeg is aware of; may be different
                             from ctx.nb_streams if new streams appear during av_read_frame() */
    int nb_streams_warn;  /* number of streams that the user was warned of */
    int rate_emu;
    int accurate_seek;

#if HAVE_THREADS
    AVThreadMessageQueue *in_thread_queue;
    pthread_t thread;           /* thread reading from this file */
    int non_blocking;           /* reading packets from the thread should not block */
    int joined;                 /* the thread has been joined */
    int thread_queue_size;      /* maximum number of queued packets */
#endif
} InputFile;

enum forced_keyframes_const {
    FKF_N,
    FKF_N_FORCED,
    FKF_PREV_FORCED_N,
    FKF_PREV_FORCED_T,
    FKF_T,
    FKF_NB
};

typedef enum {
    ENCODER_FINISHED = 1,
    MUXER_FINISHED = 2,
} OSTFinished ;

typedef struct OutputStream {
    int file_index;          /* file index */
    int index;               /* stream index in the output file */
    int source_index;        /* InputStream index */
    AVStream *st;            /* stream in the output file */
    int encoding_needed;     /* true if encoding needed for this stream */
    int frame_number;
    /* input pts and corresponding output pts
       for A/V sync */
    struct InputStream *sync_ist; /* input stream to sync against */
    int64_t sync_opts;       /* output frame counter, could be changed to some true timestamp */ // FIXME look at frame_number
    /* pts of the first frame encoded for this stream, used for limiting
     * recording time */
    int64_t first_pts;
    /* dts of the last packet sent to the muxer */
    int64_t last_mux_dts;
    // the timebase of the packets sent to the muxer
    AVRational mux_timebase;
    AVRational enc_timebase;

    int                    nb_bitstream_filters;
    AVBSFContext            **bsf_ctx;

    AVCodecContext *enc_ctx;
    AVCodecParameters *ref_par; /* associated input codec parameters with encoders options applied */
    AVCodec *enc;
    int64_t max_frames;
    AVFrame *filtered_frame;
    AVFrame *last_frame;
    int last_dropped;
    int last_nb0_frames[3];

    void  *hwaccel_ctx;

    /* video only */
    AVRational frame_rate;
    int is_cfr;
    int force_fps;
    int top_field_first;
    int rotate_overridden;
    double rotate_override_value;

    AVRational frame_aspect_ratio;

    /* forced key frames */
    int64_t forced_kf_ref_pts;
    int64_t *forced_kf_pts;
    int forced_kf_count;
    int forced_kf_index;
    char *forced_keyframes;
    AVExpr *forced_keyframes_pexpr;
    double forced_keyframes_expr_const_values[FKF_NB];

    /* audio only */
    int *audio_channels_map;             /* list of the channels id to pick from the source stream */
    int audio_channels_mapped;           /* number of channels in audio_channels_map */

    char *logfile_prefix;
    FILE *logfile;

    //OutputFilter *filter;
    char *avfilter;
    char *filters;         ///< filtergraph associated to the -filter option
    char *filters_script;  ///< filtergraph script associated to the -filter_script option

    AVDictionary *encoder_opts;
    AVDictionary *sws_dict;
    AVDictionary *swr_opts;
    AVDictionary *resample_opts;
    char *apad;
    OSTFinished finished;        /* no more packets should be written for this stream */
    int unavailable;                     /* true if the steram is unavailable (possibly temporarily) */
    int stream_copy;

    // init_output_stream() has been called for this stream
    // The encoder and the bitstream filters have been initialized and the stream
    // parameters are set in the AVStream.
    int initialized;

    int inputs_done;

    const char *attachment_filename;
    int copy_initial_nonkeyframes;
    int copy_prior_start;
    char *disposition;

    int keep_pix_fmt;

    /* stats */
    // combined size of all the packets written
    uint64_t data_size;
    // number of packets send to the muxer
    uint64_t packets_written;
    // number of frames/samples sent to the encoder
    uint64_t frames_encoded;
    uint64_t samples_encoded;

    /* packet quality factor */
    int quality;

    int max_muxing_queue_size;

    /* the packets are buffered here until the muxer is ready to be initialized */
    AVFifoBuffer *muxing_queue;

    /* packet picture type */
    int pict_type;

    /* frame encode sum of squared error values */
    int64_t error[4];
} OutputStream;

typedef struct OutputFile {
    AVFormatContext *ctx;
    AVDictionary *opts;
    int ost_index;       /* index of the first stream in output_streams */
    int64_t recording_time;  ///< desired length of the resulting file in microseconds == AV_TIME_BASE units
    int64_t start_time;      ///< start time in microseconds == AV_TIME_BASE units
    uint64_t limit_filesize; /* filesize limit expressed in bytes */

    int shortest;

    int header_written;
} OutputFile;


/* select an input stream for an output stream */
typedef struct StreamMap {
    int disabled;           /* 1 is this mapping is disabled by a negative map */
    int file_index;
    int stream_index;
    int sync_file_index;
    int sync_stream_index;
    char *linklabel;       /* name of an output link, for mapping lavfi outputs */
} StreamMap;



typedef struct SpecifierOpt {
    char *specifier;    /**< stream/chapter/program/... specifier */
    union {
        uint8_t *str;
        int        i;
        int64_t  i64;
        uint64_t ui64;
        float      f;
        double   dbl;
    } u;
} SpecifierOpt;

typedef struct OptionDef {
    const char *name;
    int flags;
#define HAS_ARG    0x0001
#define OPT_BOOL   0x0002
#define OPT_EXPERT 0x0004
#define OPT_STRING 0x0008
#define OPT_VIDEO  0x0010
#define OPT_AUDIO  0x0020
#define OPT_INT    0x0080
#define OPT_FLOAT  0x0100
#define OPT_SUBTITLE 0x0200
#define OPT_INT64  0x0400
#define OPT_EXIT   0x0800
#define OPT_DATA   0x1000
#define OPT_PERFILE  0x2000     /* the option is per-file (currently ffmpeg-only).
                                   implied by OPT_OFFSET or OPT_SPEC */
#define OPT_OFFSET 0x4000       /* option is specified as an offset in a passed optctx */
#define OPT_SPEC   0x8000       /* option is to be stored in an array of SpecifierOpt.
                                   Implies OPT_OFFSET. Next element after the offset is
                                   an int containing element count in the array. */
#define OPT_TIME  0x10000
#define OPT_DOUBLE 0x20000
#define OPT_INPUT  0x40000
#define OPT_OUTPUT 0x80000
     union {
        void *dst_ptr;
        int (*func_arg)(void *, const char *, const char *, void *);
        size_t off;
    } u;
    const char *help;
    const char *argname;
} OptionDef;

/**
 * An option extracted from the commandline.
 * Cannot use AVDictionary because of options like -map which can be
 * used multiple times.
 */
typedef struct Option {
    const OptionDef  *opt;
    const char       *key;
    const char       *val;
} Option;

typedef struct OptionGroupDef {
    /**< group name */
    const char *name;
    /**
     * Option to be used as group separator. Can be NULL for groups which
     * are terminated by a non-option argument (e.g. ffmpeg output files)
     */
    const char *sep;
    /**
     * Option flags that must be set on each option that is
     * applied to this group
     */
    int flags;
} OptionGroupDef;

typedef struct OptionGroup
{
    const OptionGroupDef *group_def;
    const char *arg;

    Option *opts;
    int  nb_opts;

    AVDictionary *codec_opts;
    AVDictionary *format_opts;
    AVDictionary *resample_opts;
    AVDictionary *sws_dict;
    AVDictionary *swr_opts;
} OptionGroup;

typedef struct OptionsContext {
    OptionGroup *g;

    /* input/output options */
    int64_t start_time;
    int64_t start_time_eof;
    int seek_timestamp;
    const char *format;

    SpecifierOpt *codec_names;
    int        nb_codec_names;
    SpecifierOpt *audio_channels;
    int        nb_audio_channels;
    SpecifierOpt *audio_sample_rate;
    int        nb_audio_sample_rate;
    SpecifierOpt *frame_rates;
    int        nb_frame_rates;
    SpecifierOpt *frame_sizes;
    int        nb_frame_sizes;
    SpecifierOpt *frame_pix_fmts;
    int        nb_frame_pix_fmts;

    /* input options */
    int64_t input_ts_offset;
    int loop;
    int rate_emu;
    int accurate_seek;
    int thread_queue_size;

    SpecifierOpt *ts_scale;
    int        nb_ts_scale;
    SpecifierOpt *dump_attachment;
    int        nb_dump_attachment;
    SpecifierOpt *hwaccels;
    int        nb_hwaccels;
    SpecifierOpt *hwaccel_devices;
    int        nb_hwaccel_devices;
    SpecifierOpt *hwaccel_output_formats;
    int        nb_hwaccel_output_formats;
    SpecifierOpt *autorotate;
    int        nb_autorotate;

    /* output options */
    StreamMap *stream_maps;
    int     nb_stream_maps;
    //AudioChannelMap *audio_channel_maps; /* one info entry per -map_channel */
    int           nb_audio_channel_maps; /* number of (valid) -map_channel settings */
    int metadata_global_manual;
    int metadata_streams_manual;
    int metadata_chapters_manual;
    const char **attachments;
    int       nb_attachments;

    int chapters_input_file;

    int64_t recording_time;
    int64_t stop_time;
    uint64_t limit_filesize;
    float mux_preload;
    float mux_max_delay;
    int shortest;
    int bitexact;

    int video_disable;
    int audio_disable;
    int subtitle_disable;
    int data_disable;

    /* indexed by output file stream index */
    int   *streamid_map;
    int nb_streamid_map;

    SpecifierOpt *metadata;
    int        nb_metadata;
    SpecifierOpt *max_frames;
    int        nb_max_frames;
    SpecifierOpt *bitstream_filters;
    int        nb_bitstream_filters;
    SpecifierOpt *codec_tags;
    int        nb_codec_tags;
    SpecifierOpt *sample_fmts;
    int        nb_sample_fmts;
    SpecifierOpt *qscale;
    int        nb_qscale;
    SpecifierOpt *forced_key_frames;
    int        nb_forced_key_frames;
    SpecifierOpt *force_fps;
    int        nb_force_fps;
    SpecifierOpt *frame_aspect_ratios;
    int        nb_frame_aspect_ratios;
    SpecifierOpt *rc_overrides;
    int        nb_rc_overrides;
    SpecifierOpt *intra_matrices;
    int        nb_intra_matrices;
    SpecifierOpt *inter_matrices;
    int        nb_inter_matrices;
    SpecifierOpt *chroma_intra_matrices;
    int        nb_chroma_intra_matrices;
    SpecifierOpt *top_field_first;
    int        nb_top_field_first;
    SpecifierOpt *metadata_map;
    int        nb_metadata_map;
    SpecifierOpt *presets;
    int        nb_presets;
    SpecifierOpt *copy_initial_nonkeyframes;
    int        nb_copy_initial_nonkeyframes;
    SpecifierOpt *copy_prior_start;
    int        nb_copy_prior_start;
    SpecifierOpt *filters;
    int        nb_filters;
    SpecifierOpt *filter_scripts;
    int        nb_filter_scripts;
    SpecifierOpt *reinit_filters;
    int        nb_reinit_filters;
    SpecifierOpt *fix_sub_duration;
    int        nb_fix_sub_duration;
    SpecifierOpt *canvas_sizes;
    int        nb_canvas_sizes;
    SpecifierOpt *pass;
    int        nb_pass;
    SpecifierOpt *passlogfiles;
    int        nb_passlogfiles;
    SpecifierOpt *max_muxing_queue_size;
    int        nb_max_muxing_queue_size;
    SpecifierOpt *guess_layout_max;
    int        nb_guess_layout_max;
    SpecifierOpt *apad;
    int        nb_apad;
    SpecifierOpt *discard;
    int        nb_discard;
    SpecifierOpt *disposition;
    int        nb_disposition;
    SpecifierOpt *program;
    int        nb_program;
    SpecifierOpt *time_bases;
    int        nb_time_bases;
    SpecifierOpt *enc_time_bases;
    int        nb_enc_time_bases;
} OptionsContext;

/**
 * A list of option groups that all have the same group type
 * (e.g. input files or output files)
 */
typedef struct OptionGroupList {
    const OptionGroupDef *group_def;

    OptionGroup *groups;
    int       nb_groups;
} OptionGroupList;

typedef struct OptionParseContext {
    OptionGroup global_opts;

    OptionGroupList *groups;
    int           nb_groups;

    /* parsing state */
    OptionGroup cur_group;
} OptionParseContext;


enum OptGroup {
    GROUP_OUTFILE,
    GROUP_INFILE,
};

static const OptionGroupDef groups[] = {
    { "output url",  NULL, OPT_OUTPUT },
    { "input url",   "i",  OPT_INPUT },
};


/**
 * Realloc array to hold new_size elements of elem_size.
 * Calls exit() on failure.
 *
 * @param array array to reallocate
 * @param elem_size size in bytes of each element
 * @param size new element count will be written here
 * @param new_size number of elements to place in reallocated array
 * @return reallocated array
 */
static void *grow_array(void *array, int elem_size, int *size, int new_size)
{
    if (new_size >= INT_MAX / elem_size) {
        av_log(NULL, AV_LOG_ERROR, "Array too big.\n");
        exit(1);
    }
    if (*size < new_size) {
        uint8_t *tmp = (uint8_t *)av_realloc_array(array, new_size, elem_size);
        if (!tmp) {
            av_log(NULL, AV_LOG_ERROR, "Could not alloc buffer.\n");
            exit(1);
        }
        memset(tmp + *size*elem_size, 0, (new_size-*size) * elem_size);
        *size = new_size;
        return tmp;
    }
    return array;
}

#define GROW_ARRAY(array, nb_elems)\
    array = grow_array(array, sizeof(*array), &nb_elems, nb_elems + 1)

#define GROW_ARRAY_2(array, nb_elems, baseType)\
{\
    array = (baseType*)grow_array(array, sizeof(*array), &nb_elems, nb_elems + 1);\
}

typedef struct FfmpegParamContext
{
public:
    InputStream **input_streams = NULL;
    int        nb_input_streams = 0;
    InputFile   **input_files   = NULL;
    int        nb_input_files   = 0;

    OutputStream **output_streams = NULL;
    int         nb_output_streams = 0;
    OutputFile   **output_files   = NULL;
    int         nb_output_files   = 0;

    int copy_ts           = 0;
    int start_at_zero     = 0;

    int file_overwrite     = 1;   //直接重写覆盖
    int no_file_overwrite  = 0;
    int stdin_interaction = 1;

    int input_stream_potentially_available = 0;
    int audio_sync_method = 0;
    int video_sync_method = VSYNC_AUTO;
    int frame_bits_per_raw_sample = 0;


    AVIOContext *progress_avio = NULL;

 typedef struct HWDevice {
     char *name;
     enum AVHWDeviceType type;
     AVBufferRef *device_ref;
 } HWDevice;

    AVBufferRef *hw_device_ctx;
    int nb_hw_devices;
    HWDevice **hw_devices;


    float dts_delta_threshold   = 10;
    float dts_error_threshold   = 3600*30;

    //static
    int intra_only         = 0;
    int do_psnr            = 0;
    int do_pkt_dump        = 0;
    int do_hex_dump       = 0;
    int exit_on_error      = 0;

    int main_return_code = 0;
    int print_stats       = -1;

    int nb_frames_drop = 0;

    int qp_hist           = 0;

    int nb_frames_dup = 0;

	bool openAvLog = true;
    int logLevel = AV_LOG_ERROR;

	//globle
	int totalAudioByte = 0;
	int transAudioByte = 0;
	double percentage = 0.0;
	bool finish = false;
}FfmpegParamContext;

/**
 * Check if the given stream matches a stream specifier.
 *
 * @param s  Corresponding format context.
 * @param st Stream from s to be checked.
 * @param spec A stream specifier of the [v|a|s|d]:[\<stream index\>] form.
 *
 * @return 1 if the stream matches, 0 if it doesn't, <0 on error
 */
static int check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec)
{
    int ret = avformat_match_stream_specifier(s, st, spec);
    if (ret < 0)
        av_log(s, AV_LOG_ERROR, "Invalid stream specifier: %s.\n", spec);
    return ret;
}

static int opt_map(void *optctx, const char *opt, const char *arg, void* paramCtx)
{
    {opt = opt;}

    FfmpegParamContext * pCtx = reinterpret_cast<FfmpegParamContext *>(paramCtx);

    OptionsContext *o = reinterpret_cast<OptionsContext *>(optctx);
    StreamMap *m = NULL;
    int i, negative = 0, file_idx, disabled = 0;
    int sync_file_idx = -1, sync_stream_idx = 0;
    char *p, *sync;
    char *map;
    char *allow_unused;

    if (*arg == '-') {
        negative = 1;
        arg++;
    }
    map = av_strdup(arg);
    if (!map)
        return AVERROR(ENOMEM);

    /* parse sync stream first, just pick first matching stream */
    if (sync = strchr(map, ',')) {
        *sync = 0;
        sync_file_idx = strtol(sync + 1, &sync, 0);
        if (sync_file_idx >= pCtx->nb_input_files || sync_file_idx < 0) {
            av_log(NULL, AV_LOG_FATAL, "Invalid sync file index: %d.\n", sync_file_idx);
            exit(1);
        }
        if (*sync)
            sync++;
        for (i = 0; i < pCtx->input_files[sync_file_idx]->nb_streams; i++)
            if (check_stream_specifier(pCtx->input_files[sync_file_idx]->ctx,
                                       pCtx->input_files[sync_file_idx]->ctx->streams[i], sync) == 1) {
                sync_stream_idx = i;
                break;
            }
        if (i == pCtx->input_files[sync_file_idx]->nb_streams) {
            av_log(NULL, AV_LOG_FATAL, "Sync stream specification in map %s does not "
                                       "match any streams.\n", arg);
            exit(1);
        }
        if (pCtx->input_streams[pCtx->input_files[sync_file_idx]->ist_index + sync_stream_idx]->user_set_discard == AVDISCARD_ALL) {
            av_log(NULL, AV_LOG_FATAL, "Sync stream specification in map %s matches a disabled input "
                                       "stream.\n", arg);
            exit(1);
        }
    }


    if (map[0] == '[') {
        /* this mapping refers to lavfi output */
        const char *c = map + 1;
        GROW_ARRAY_2(o->stream_maps, o->nb_stream_maps, StreamMap);
        m = &o->stream_maps[o->nb_stream_maps - 1];
        m->linklabel = av_get_token(&c, "]");
        if (!m->linklabel) {
            av_log(NULL, AV_LOG_ERROR, "Invalid output link label: %s.\n", map);
            exit(1);
        }
    } else {
        if (allow_unused = strchr(map, '?'))
            *allow_unused = 0;
        file_idx = strtol(map, &p, 0);
        if (file_idx >= pCtx->nb_input_files || file_idx < 0) {
            av_log(NULL, AV_LOG_FATAL, "Invalid input file index: %d.\n", file_idx);
            exit(1);
        }
        if (negative)
            /* disable some already defined maps */
            for (i = 0; i < o->nb_stream_maps; i++) {
                m = &o->stream_maps[i];
                if (file_idx == m->file_index &&
                        check_stream_specifier(pCtx->input_files[m->file_index]->ctx,
                                               pCtx->input_files[m->file_index]->ctx->streams[m->stream_index],
                                               *p == ':' ? p + 1 : p) > 0)
                    m->disabled = 1;
            }
        else
            for (i = 0; i < pCtx->input_files[file_idx]->nb_streams; i++) {
                if (check_stream_specifier(pCtx->input_files[file_idx]->ctx, pCtx->input_files[file_idx]->ctx->streams[i],
                                           *p == ':' ? p + 1 : p) <= 0)
                    continue;
                if (pCtx->input_streams[pCtx->input_files[file_idx]->ist_index + i]->user_set_discard == AVDISCARD_ALL) {
                    disabled = 1;
                    continue;
                }
                GROW_ARRAY_2(o->stream_maps, o->nb_stream_maps,StreamMap);
                m = &o->stream_maps[o->nb_stream_maps - 1];

                m->file_index   = file_idx;
                m->stream_index = i;

                if (sync_file_idx >= 0) {
                    m->sync_file_index   = sync_file_idx;
                    m->sync_stream_index = sync_stream_idx;
                } else {
                    m->sync_file_index   = file_idx;
                    m->sync_stream_index = i;
                }
            }
    }

    if (!m) {
        if (allow_unused) {
            av_log(NULL, AV_LOG_VERBOSE, "Stream map '%s' matches no streams; ignoring.\n", arg);
        } else if (disabled) {
            av_log(NULL, AV_LOG_FATAL, "Stream map '%s' matches disabled streams.\n"
                                       "To ignore this, add a trailing '?' to the map.\n", arg);
            exit(1);
        } else {
            av_log(NULL, AV_LOG_FATAL, "Stream map '%s' matches no streams.\n"
                                       "To ignore this, add a trailing '?' to the map.\n", arg);
            exit(1);
        }
    }

    av_freep(&map);
    return 0;
}




//#include fftools/cmdutils


static AVDictionary *filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                AVFormatContext *s, AVStream *st, AVCodec *codec)
{
    AVDictionary    *ret = NULL;
    AVDictionaryEntry *t = NULL;
    int            flags = s->oformat ? AV_OPT_FLAG_ENCODING_PARAM
                                      : AV_OPT_FLAG_DECODING_PARAM;
    char          prefix = 0;
    const AVClass    *cc = avcodec_get_class();

    if (!codec)
        codec            = s->oformat ? avcodec_find_encoder(codec_id)
                                      : avcodec_find_decoder(codec_id);

    switch (st->codecpar->codec_type) {
    case AVMEDIA_TYPE_VIDEO:
        prefix  = 'v';
        flags  |= AV_OPT_FLAG_VIDEO_PARAM;
        break;
    case AVMEDIA_TYPE_AUDIO:
        prefix  = 'a';
        flags  |= AV_OPT_FLAG_AUDIO_PARAM;
        break;
    case AVMEDIA_TYPE_SUBTITLE:
        prefix  = 's';
        flags  |= AV_OPT_FLAG_SUBTITLE_PARAM;
        break;
    }

    while (t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX)) {
        char *p = strchr(t->key, ':');

        /* check stream specification in opt name */
        if (p)
            switch (check_stream_specifier(s, st, p + 1)) {
            case  1: *p = 0; break;
            case  0:         continue;
            default:         exit(1);
            }

        if (av_opt_find(&cc, t->key, NULL, flags, AV_OPT_SEARCH_FAKE_OBJ) ||
            !codec ||
            (codec->priv_class &&
             av_opt_find(&codec->priv_class, t->key, NULL, flags,
                         AV_OPT_SEARCH_FAKE_OBJ)))
            av_dict_set(&ret, t->key, t->value, 0);
        else if (t->key[0] == prefix &&
                 av_opt_find(&cc, t->key + 1, NULL, flags,
                             AV_OPT_SEARCH_FAKE_OBJ))
            av_dict_set(&ret, t->key + 1, t->value, 0);

        if (p)
            *p = ':';
    }
    return ret;
}

static AVDictionary **setup_find_stream_info_opts(AVFormatContext *s,
                                           AVDictionary *codec_opts)
{
    unsigned int i;
    AVDictionary **opts;

    if (!s->nb_streams)
        return NULL;
    opts = (AVDictionary **)av_mallocz_array(s->nb_streams, sizeof(*opts));
    if (!opts) {
        av_log(NULL, AV_LOG_ERROR,
               "Could not alloc memory for stream options.\n");
        return NULL;
    }
    for (i = 0; i < s->nb_streams; i++)
        opts[i] = filter_codec_opts(codec_opts, s->streams[i]->codecpar->codec_id,
                                    s, s->streams[i], NULL);
    return opts;
}


static double parse_number_or_die(const char *context, const char *numstr, int type,
                           double min, double max)
{
    char *tail;
    const char *error;
    double d = av_strtod(numstr, &tail);
    if (*tail)
        error = "Expected number for %s but found: %s\n";
    else if (d < min || d > max)
        error = "The value for %s was %s which is not within %f - %f\n";
    else if (type == OPT_INT64 && (int64_t)d != d)
        error = "Expected int64 for %s but found %s\n";
    else if (type == OPT_INT && (int)d != d)
        error = "Expected int for %s but found %s\n";
    else
        return d;
    av_log(NULL, AV_LOG_FATAL, error, context, numstr, min, max);
    exit(1);
    return 0;
}

static int64_t parse_time_or_die(const char *context, const char *timestr,
                          int is_duration)
{
    int64_t us;
    if (av_parse_time(&us, timestr, is_duration) < 0) {
        av_log(NULL, AV_LOG_FATAL, "Invalid %s specification for %s: %s\n",
               is_duration ? "duration" : "date", context, timestr);
        exit(1);
    }
    return us;
}

static int write_option(void *optctx, const OptionDef *po, const char *opt,
                        const char *arg, void* paramCtx)
{
    /* new-style options contain an offset into optctx, old-style address of
     * a global var*/
    void *dst = po->flags & (OPT_OFFSET | OPT_SPEC) ?
                (uint8_t *)optctx + po->u.off : po->u.dst_ptr;
    int *dstcount;

    if (po->flags & OPT_SPEC) {
        SpecifierOpt **so = (SpecifierOpt **)dst;
        const char *p = strchr(opt, ':');
        char *str;

        dstcount = (int *)(so + 1);
        *so = (SpecifierOpt *)grow_array(*so, sizeof(**so), dstcount, *dstcount + 1);
        str = av_strdup(p ? p + 1 : "");
        if (!str)
            return AVERROR(ENOMEM);
        (*so)[*dstcount - 1].specifier = str;
        dst = &(*so)[*dstcount - 1].u;
    }

    if (po->flags & OPT_STRING) {
        char *str;
        str = av_strdup(arg);
        av_freep(dst);
        if (!str)
            return AVERROR(ENOMEM);
        *(char **)dst = str;
    } else if (po->flags & OPT_BOOL || po->flags & OPT_INT) {
        *(int *)dst = (int)parse_number_or_die(opt, arg, OPT_INT64, INT_MIN, INT_MAX);
    } else if (po->flags & OPT_INT64) {
        *(int64_t *)dst = (int64_t)parse_number_or_die(opt, arg, (int)OPT_INT64, (double)INT64_MIN, (double)INT64_MAX);
    } else if (po->flags & OPT_TIME) {
        *(int64_t *)dst = parse_time_or_die(opt, arg, 1);
    } else if (po->flags & OPT_FLOAT) {
        *(float *)dst = parse_number_or_die(opt, arg, OPT_FLOAT, -INFINITY, INFINITY);
    } else if (po->flags & OPT_DOUBLE) {
        *(double *)dst = parse_number_or_die(opt, arg, OPT_DOUBLE, -INFINITY, INFINITY);
    } else if (po->u.func_arg) {
        int ret = po->u.func_arg(optctx, opt, arg,(void*)paramCtx);
        if (ret < 0) {
            char errString[AV_ERROR_MAX_STRING_SIZE];
            av_log(NULL, AV_LOG_ERROR,
                   "Failed to set value '%s' for option '%s': %s\n",
                   arg, opt, av_make_error_string(errString, AV_ERROR_MAX_STRING_SIZE, ret));
            return ret;
        }
    }
    if (po->flags & OPT_EXIT)
        exit(0);

    return 0;
}

//暂时只需要这4个选项
#define OFFSET(x) offsetof(OptionsContext, x)
const OptionDef options[] = {
    { "c", HAS_ARG | OPT_STRING | OPT_SPEC |OPT_INPUT | OPT_OUTPUT,   (void*)OFFSET(codec_names) ,//{ .off       = OFFSET(codec_names) },
            "codec name", "codec" },
    { "map", HAS_ARG | OPT_EXPERT | OPT_PERFILE |OPT_OUTPUT,    (void*)opt_map,   // { .func_arg = opt_map },
            "set input stream mapping",
            "[-]input_file_id[:stream_specifier][,sync_file_id[:stream_specifier]]" },
    { "metadata", HAS_ARG | OPT_STRING | OPT_SPEC | OPT_OUTPUT, (void*)OFFSET(metadata),//{ .off = OFFSET(metadata) },
            "add metadata", "string=string" },
    { NULL, },
};

static int parse_optgroup(void *optctx, OptionGroup *g, void* paramCtx)
{
    int i, ret;

    av_log(NULL, AV_LOG_DEBUG, "Parsing a group of options: %s %s.\n",
           g->group_def->name, g->arg);

    for (i = 0; i < g->nb_opts; i++) {
        Option *o = &g->opts[i];

        if (g->group_def->flags &&
            !(g->group_def->flags & o->opt->flags)) {
            av_log(NULL, AV_LOG_ERROR, "Option %s (%s) cannot be applied to "
                   "%s %s -- you are trying to apply an input option to an "
                   "output file or vice versa. Move this option before the "
                   "file it belongs to.\n", o->key, o->opt->help,
                   g->group_def->name, g->arg);
            return AVERROR(EINVAL);
        }

        av_log(NULL, AV_LOG_DEBUG, "Applying option %s (%s) with argument %s.\n",
               o->key, o->opt->help, o->val);

        ret = write_option(optctx, o->opt, o->key, o->val,(void*)paramCtx);
        if (ret < 0)
            return ret;
    }

    av_log(NULL, AV_LOG_DEBUG, "Successfully parsed a group of options.\n");

    return 0;
}






//ffmpeg_opt.c

#define MATCH_PER_STREAM_OPT(name, type, outvar, fmtctx, st)\
{\
    int i, ret;\
    for (i = 0; i < o->nb_ ## name; i++) {\
        char *spec = o->name[i].specifier;\
        if ((ret = check_stream_specifier(fmtctx, st, spec)) > 0)\
            outvar = o->name[i].u.type;\
        else if (ret < 0)\
            exit(1);\
    }\
}

//MATCH_PER_STREAM_OPT 中 uint8_t* 无法转为 char* 时使用 MATCH_PER_STREAM_OPT_2
#define MATCH_PER_STREAM_OPT_2(name, type, outvar, fmtctx, st)\
{\
    uint8_t** temp_uint8_t = (uint8_t**)&outvar;\
    MATCH_PER_STREAM_OPT(name, type, *temp_uint8_t, fmtctx, st)\
}

static AVCodec *find_codec_or_die(const char *name, enum AVMediaType type, int encoder)
{
    const AVCodecDescriptor *desc;
    const char *codec_string = encoder ? "encoder" : "decoder";
    AVCodec *codec;

    codec = encoder ?
        avcodec_find_encoder_by_name(name) :
        avcodec_find_decoder_by_name(name);

    if (!codec && (desc = avcodec_descriptor_get_by_name(name))) {
        codec = encoder ? avcodec_find_encoder(desc->id) :
                          avcodec_find_decoder(desc->id);
        if (codec)
            av_log(NULL, AV_LOG_VERBOSE, "Matched %s '%s' for codec '%s'.\n",
                   codec_string, codec->name, desc->name);
    }

    if (!codec) {
        av_log(NULL, AV_LOG_FATAL, "Unknown %s '%s'\n", codec_string, name);
        exit(1);
    }
    if (codec->type != type) {
        av_log(NULL, AV_LOG_FATAL, "Invalid %s type '%s'\n", codec_string, name);
        exit(1);
    }
    return codec;
}

static AVCodec *choose_decoder(OptionsContext *o, AVFormatContext *s, AVStream *st)
{
    char *codec_name = NULL;

    MATCH_PER_STREAM_OPT_2(codec_names, str, codec_name, s, st);

    if (codec_name) {
        AVCodec *codec = find_codec_or_die(codec_name, st->codecpar->codec_type, 0);
        st->codecpar->codec_id = codec->id;
        return codec;
    } else
        return avcodec_find_decoder(st->codecpar->codec_id);
}

/* return a copy of the input with the stream specifiers removed from the keys */
static AVDictionary *strip_specifiers(AVDictionary *dict)
{
    AVDictionaryEntry *e = NULL;
    AVDictionary    *ret = NULL;

    while ((e = av_dict_get(dict, "", e, AV_DICT_IGNORE_SUFFIX))) {
        char *p = strchr(e->key, ':');

        if (p)
            *p = 0;
        av_dict_set(&ret, e->key, e->value, 0);
        if (p)
            *p = ':';
    }
    return ret;
}


static int choose_encoder(OptionsContext *o, AVFormatContext *s, OutputStream *ost)
{
    enum AVMediaType type = ost->st->codecpar->codec_type;
    char *codec_name = NULL;

    if (type == AVMEDIA_TYPE_VIDEO || type == AVMEDIA_TYPE_AUDIO || type == AVMEDIA_TYPE_SUBTITLE) {
        MATCH_PER_STREAM_OPT_2(codec_names, str, codec_name, s, ost->st);
        if (!codec_name) {
            ost->st->codecpar->codec_id = av_guess_codec(s->oformat, NULL, s->url,
                                                         NULL, ost->st->codecpar->codec_type);
            ost->enc = avcodec_find_encoder(ost->st->codecpar->codec_id);
            if (!ost->enc) {
                av_log(NULL, AV_LOG_FATAL, "Automatic encoder selection failed for "
                       "output stream #%d:%d. Default encoder for format %s (codec %s) is "
                       "probably disabled. Please choose an encoder manually.\n",
                       ost->file_index, ost->index, s->oformat->name,
                       avcodec_get_name(ost->st->codecpar->codec_id));
                return AVERROR_ENCODER_NOT_FOUND;
            }
        } else if (!strcmp(codec_name, "copy"))
            ost->stream_copy = 1;
        else {
            ost->enc = find_codec_or_die(codec_name, ost->st->codecpar->codec_type, 1);
            ost->st->codecpar->codec_id = ost->enc->id;
        }
        ost->encoding_needed = !ost->stream_copy;
    } else {
        /* no encoding supported for other media types */
        ost->stream_copy     = 1;
        ost->encoding_needed = 0;
    }

    return 0;
}

#define AVCONV_DATADIR "."
static int get_preset_file_2(const char *preset_name, const char *codec_name, AVIOContext **s)
{
    int i, ret = -1;
    char filename[1000];
    const char *base[3] = { getenv("AVCONV_DATADIR"),
                            getenv("HOME"),
                            AVCONV_DATADIR,
                            };

    for (i = 0; i < FF_ARRAY_ELEMS(base) && ret < 0; i++) {
        if (!base[i])
            continue;
        if (codec_name) {
            snprintf(filename, sizeof(filename), "%s%s/%s-%s.avpreset", base[i],
                     i != 1 ? "" : "/.avconv", codec_name, preset_name);
            ret = avio_open2(s, filename, AVIO_FLAG_READ, NULL/*&int_cb*/, NULL);
        }
        if (ret < 0) {
            snprintf(filename, sizeof(filename), "%s%s/%s.avpreset", base[i],
                     i != 1 ? "" : "/.avconv", preset_name);
            ret = avio_open2(s, filename, AVIO_FLAG_READ, NULL/*&int_cb*/, NULL);
        }
    }
    return ret;
}

static uint8_t *get_line(AVIOContext *s)
{
    AVIOContext *line;
    uint8_t *buf;
    char c;

    if (avio_open_dyn_buf(&line) < 0) {
        av_log(NULL, AV_LOG_FATAL, "Could not alloc buffer for reading preset.\n");
        exit(1);
    }

    while ((c = avio_r8(s)) && c != '\n')
        avio_w8(line, c);
    avio_w8(line, 0);
    avio_close_dyn_buf(line, &buf);

    return buf;
}


static OutputStream *new_output_stream(OptionsContext *o, AVFormatContext *oc, enum AVMediaType type, int source_index,FfmpegParamContext* paramCtx)
{
    OutputStream *ost;
    AVStream *st = avformat_new_stream(oc, NULL);
    int idx      = oc->nb_streams - 1, ret = 0;
    const char *bsfs = NULL, *time_base = NULL;
    char *next, *codec_tag = NULL;
    double qscale = -1;
    int i;

    if (!st) {
        av_log(NULL, AV_LOG_FATAL, "Could not alloc stream.\n");
        exit(1);
    }

    if (oc->nb_streams - 1 < (unsigned int)(o->nb_streamid_map))
        st->id = o->streamid_map[oc->nb_streams - 1];

    GROW_ARRAY_2(paramCtx->output_streams, paramCtx->nb_output_streams,OutputStream *);
    if (!(ost = (OutputStream *)av_mallocz(sizeof(*ost))))
        exit(1);
    paramCtx->output_streams[paramCtx->nb_output_streams - 1] = ost;

    ost->file_index = paramCtx->nb_output_files - 1;
    ost->index      = idx;
    ost->st         = st;
    ost->forced_kf_ref_pts = AV_NOPTS_VALUE;
    st->codecpar->codec_type = type;

    ret = choose_encoder(o, oc, ost);
    if (ret < 0) {
        av_log(NULL, AV_LOG_FATAL, "Error selecting an encoder for stream "
               "%d:%d\n", ost->file_index, ost->index);
        exit(1);
    }

    ost->enc_ctx = avcodec_alloc_context3(ost->enc);
    if (!ost->enc_ctx) {
        av_log(NULL, AV_LOG_ERROR, "Error allocating the encoding context.\n");
        exit(1);
    }
    ost->enc_ctx->codec_type = type;

    ost->ref_par = avcodec_parameters_alloc();
    if (!ost->ref_par) {
        av_log(NULL, AV_LOG_ERROR, "Error allocating the encoding parameters.\n");
        exit(1);
    }

    if (ost->enc) {
        AVIOContext *s = NULL;
        char *buf = NULL, *arg = NULL, *preset = NULL;

        ost->encoder_opts  = filter_codec_opts(o->g->codec_opts, ost->enc->id, oc, st, ost->enc);

        MATCH_PER_STREAM_OPT_2(presets, str, preset, oc, st);
        if (preset && (!(ret = get_preset_file_2(preset, ost->enc->name, &s)))) {
            do  {
                buf = (char*)get_line(s);
                if (!buf[0] || buf[0] == '#') {
                    av_free(buf);
                    continue;
                }
                if (!(arg = strchr(buf, '='))) {
                    av_log(NULL, AV_LOG_FATAL, "Invalid line found in the preset file.\n");
                    exit(1);
                }
                *arg++ = 0;
                av_dict_set(&ost->encoder_opts, buf, arg, AV_DICT_DONT_OVERWRITE);
                av_free(buf);
            } while (!s->eof_reached);
            avio_closep(&s);
        }
        if (ret) {
            av_log(NULL, AV_LOG_FATAL,
                   "Preset %s specified for stream %d:%d, but could not be opened.\n",
                   preset, ost->file_index, ost->index);
            exit(1);
        }
    } else {
        ost->encoder_opts = filter_codec_opts(o->g->codec_opts, AV_CODEC_ID_NONE, oc, st, NULL);
    }


    if (o->bitexact)
        ost->enc_ctx->flags |= AV_CODEC_FLAG_BITEXACT;

    MATCH_PER_STREAM_OPT_2(time_bases, str, time_base, oc, st);
    if (time_base) {
        AVRational q;
        if (av_parse_ratio(&q, time_base, INT_MAX, 0, NULL) < 0 ||
            q.num <= 0 || q.den <= 0) {
            av_log(NULL, AV_LOG_FATAL, "Invalid time base: %s\n", time_base);
            exit(1);
        }
        st->time_base = q;
    }

    MATCH_PER_STREAM_OPT_2(enc_time_bases, str, time_base, oc, st);
    if (time_base) {
        AVRational q;
        if (av_parse_ratio(&q, time_base, INT_MAX, 0, NULL) < 0 ||
            q.den <= 0) {
            av_log(NULL, AV_LOG_FATAL, "Invalid time base: %s\n", time_base);
            exit(1);
        }
        ost->enc_timebase = q;
    }

    ost->max_frames = INT64_MAX;
    MATCH_PER_STREAM_OPT(max_frames, i64, ost->max_frames, oc, st);
    for (i = 0; i<o->nb_max_frames; i++) {
        char *p = o->max_frames[i].specifier;
        if (!*p && type != AVMEDIA_TYPE_VIDEO) {
            av_log(NULL, AV_LOG_WARNING, "Applying unspecific -frames to non video streams, maybe you meant -vframes ?\n");
            break;
        }
    }

    ost->copy_prior_start = -1;
    MATCH_PER_STREAM_OPT(copy_prior_start, i, ost->copy_prior_start, oc ,st);

    MATCH_PER_STREAM_OPT_2(bitstream_filters, str, bsfs, oc, st);
    while (bsfs && *bsfs) {
        const AVBitStreamFilter *filter;
        char *bsf, *bsf_options_str, *bsf_name;

        bsf = av_get_token(&bsfs, ",");
        if (!bsf)
            exit(1);
        bsf_name = av_strtok(bsf, "=", &bsf_options_str);
        if (!bsf_name)
            exit(1);

        filter = av_bsf_get_by_name(bsf_name);
        if (!filter) {
            av_log(NULL, AV_LOG_FATAL, "Unknown bitstream filter %s\n", bsf_name);
            exit(1);
        }

        ost->bsf_ctx = (AVBSFContext**)av_realloc_array(ost->bsf_ctx,
                                        ost->nb_bitstream_filters + 1,
                                        sizeof(*ost->bsf_ctx));
        if (!ost->bsf_ctx)
            exit(1);

        ret = av_bsf_alloc(filter, &ost->bsf_ctx[ost->nb_bitstream_filters]);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error allocating a bitstream filter context\n");
            exit(1);
        }

        ost->nb_bitstream_filters++;

        if (bsf_options_str && filter->priv_class) {
            const AVOption *opt = av_opt_next(ost->bsf_ctx[ost->nb_bitstream_filters-1]->priv_data, NULL);
            const char * shorthand[2] = {NULL};

            if (opt)
                shorthand[0] = opt->name;

            ret = av_opt_set_from_string(ost->bsf_ctx[ost->nb_bitstream_filters-1]->priv_data, bsf_options_str, shorthand, "=", ":");
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error parsing options for bitstream filter %s\n", bsf_name);
                exit(1);
            }
        }
        av_freep(&bsf);

        if (*bsfs)
            bsfs++;
    }

    MATCH_PER_STREAM_OPT_2(codec_tags, str, codec_tag, oc, st);
    if (codec_tag) {
        uint32_t tag = strtol(codec_tag, &next, 0);
        if (*next)
            tag = AV_RL32(codec_tag);
        ost->st->codecpar->codec_tag =
        ost->enc_ctx->codec_tag = tag;
    }

    MATCH_PER_STREAM_OPT(qscale, dbl, qscale, oc, st);
    if (qscale >= 0) {
        ost->enc_ctx->flags |= AV_CODEC_FLAG_QSCALE;
        ost->enc_ctx->global_quality = FF_QP2LAMBDA * qscale;
    }

    MATCH_PER_STREAM_OPT_2(disposition, str, ost->disposition, oc, st);
    ost->disposition = av_strdup(ost->disposition);

    ost->max_muxing_queue_size = 128;
    MATCH_PER_STREAM_OPT(max_muxing_queue_size, i, ost->max_muxing_queue_size, oc, st);
    ost->max_muxing_queue_size *= sizeof(AVPacket);

    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        ost->enc_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    av_dict_copy(&ost->sws_dict, o->g->sws_dict, 0);

    av_dict_copy(&ost->swr_opts, o->g->swr_opts, 0);
    if (ost->enc && av_get_exact_bits_per_sample(ost->enc->id) == 24)
        av_dict_set(&ost->swr_opts, "output_sample_bits", "24", 0);

    av_dict_copy(&ost->resample_opts, o->g->resample_opts, 0);

    ost->source_index = source_index;
    if (source_index >= 0) {
        ost->sync_ist = paramCtx->input_streams[source_index];
        paramCtx->input_streams[source_index]->discard = 0;
        paramCtx->input_streams[source_index]->st->discard = (AVDiscard)paramCtx->input_streams[source_index]->user_set_discard;
    }
    ost->last_mux_dts = AV_NOPTS_VALUE;

    ost->muxing_queue = av_fifo_alloc(8 * sizeof(AVPacket));
    if (!ost->muxing_queue)
        exit(1);

    return ost;
}

static void check_streamcopy_filters(OptionsContext *o, AVFormatContext *oc,
                                     const OutputStream *ost, enum AVMediaType type)
{
    Q_UNUSED(o);
    Q_UNUSED(oc);
    if (ost->filters_script || ost->filters) {
        av_log(NULL, AV_LOG_ERROR,
               "%s '%s' was defined for %s output stream %d:%d but codec copy was selected.\n"
               "Filtering and streamcopy cannot be used together.\n",
               ost->filters ? "Filtergraph" : "Filtergraph script",
               ost->filters ? ost->filters : ost->filters_script,
               av_get_media_type_string(type), ost->file_index, ost->index);
        exit(1);
    }
}


static void parse_matrix_coeffs(uint16_t *dest, const char *str)
{
    int i;
    const char *p = str;
    for (i = 0;; i++) {
        dest[i] = atoi(p);
        if (i == 63)
            break;
        p = strchr(p, ',');
        if (!p) {
            av_log(NULL, AV_LOG_FATAL, "Syntax error in matrix \"%s\" at coeff %d\n", str, i);
            exit(1);
        }
        p++;
    }
}

/* read file contents into a string */
static uint8_t *read_file(const char *filename)
{
    AVIOContext *pb      = NULL;
    AVIOContext *dyn_buf = NULL;
    int ret = avio_open(&pb, filename, AVIO_FLAG_READ);
    uint8_t buf[1024], *str;

    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Error opening file %s.\n", filename);
        return NULL;
    }

    ret = avio_open_dyn_buf(&dyn_buf);
    if (ret < 0) {
        avio_closep(&pb);
        return NULL;
    }
    while ((ret = avio_read(pb, buf, sizeof(buf))) > 0)
        avio_write(dyn_buf, buf, ret);
    avio_w8(dyn_buf, 0);
    avio_closep(&pb);

    ret = avio_close_dyn_buf(dyn_buf, &str);
    if (ret < 0)
        return NULL;
    return str;
}

static char *get_ost_filters(OptionsContext *o, AVFormatContext *oc,
                             OutputStream *ost)
{
    Q_UNUSED(o);
    Q_UNUSED(oc);

    AVStream *st = ost->st;

    if (ost->filters_script && ost->filters) {
//        av_log(NULL, AV_LOG_ERROR, "Both -filter and -filter_script set for "
//               "output stream #%d:%d.\n", nb_output_files, st->index);
        exit(1);
    }

    if (ost->filters_script)
        return (char*)read_file(ost->filters_script);
    else if (ost->filters)
        return av_strdup(ost->filters);

    return av_strdup(st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO ?
                     "null" : "anull");
}

static OutputStream *new_video_stream(OptionsContext *o, AVFormatContext *oc, int source_index,FfmpegParamContext* paramCtx)
{
    AVStream *st;
    OutputStream *ost;
    AVCodecContext *video_enc;
    char *frame_rate = NULL, *frame_aspect_ratio = NULL;

    ost = new_output_stream(o, oc, AVMEDIA_TYPE_VIDEO, source_index, paramCtx);
    st  = ost->st;
    video_enc = ost->enc_ctx;

    MATCH_PER_STREAM_OPT_2(frame_rates, str, frame_rate, oc, st);
    if (frame_rate && av_parse_video_rate(&ost->frame_rate, frame_rate) < 0) {
        av_log(NULL, AV_LOG_FATAL, "Invalid framerate value: %s\n", frame_rate);
        exit(1);
    }
    if (frame_rate && paramCtx->video_sync_method == VSYNC_PASSTHROUGH)
        av_log(NULL, AV_LOG_ERROR, "Using -vsync 0 and -r can produce invalid output files\n");

    MATCH_PER_STREAM_OPT_2(frame_aspect_ratios, str, frame_aspect_ratio, oc, st);
    if (frame_aspect_ratio) {
        AVRational q;
        if (av_parse_ratio(&q, frame_aspect_ratio, 255, 0, NULL) < 0 ||
            q.num <= 0 || q.den <= 0) {
            av_log(NULL, AV_LOG_FATAL, "Invalid aspect ratio: %s\n", frame_aspect_ratio);
            exit(1);
        }
        ost->frame_aspect_ratio = q;
    }

    MATCH_PER_STREAM_OPT_2(filter_scripts, str, ost->filters_script, oc, st);
    MATCH_PER_STREAM_OPT_2(filters,        str, ost->filters,        oc, st);

    //if (o->nb_filters > 1)
    //    av_log(NULL, AV_LOG_ERROR, "Only '-vf %s' read, ignoring remaining -vf options: Use ',' to separate filters\n", ost->filters);

    if (!ost->stream_copy) {
        const char *p = NULL;
        char *frame_size = NULL;
        char *frame_pix_fmt = NULL;
        char *intra_matrix = NULL, *inter_matrix = NULL;
        char *chroma_intra_matrix = NULL;
        int do_pass = 0;
        int i;

        MATCH_PER_STREAM_OPT_2(frame_sizes, str, frame_size, oc, st);
        if (frame_size && av_parse_video_size(&video_enc->width, &video_enc->height, frame_size) < 0) {
            av_log(NULL, AV_LOG_FATAL, "Invalid frame size: %s.\n", frame_size);
            exit(1);
        }

        video_enc->bits_per_raw_sample = paramCtx->frame_bits_per_raw_sample;
        MATCH_PER_STREAM_OPT_2(frame_pix_fmts, str, frame_pix_fmt, oc, st);
        if (frame_pix_fmt && *frame_pix_fmt == '+') {
            ost->keep_pix_fmt = 1;
            if (!*++frame_pix_fmt)
                frame_pix_fmt = NULL;
        }
        if (frame_pix_fmt && (video_enc->pix_fmt = av_get_pix_fmt(frame_pix_fmt)) == AV_PIX_FMT_NONE) {
            av_log(NULL, AV_LOG_FATAL, "Unknown pixel format requested: %s.\n", frame_pix_fmt);
            exit(1);
        }
        st->sample_aspect_ratio = video_enc->sample_aspect_ratio;

        if (paramCtx->intra_only)
            video_enc->gop_size = 0;
        MATCH_PER_STREAM_OPT_2(intra_matrices, str, intra_matrix, oc, st);
        if (intra_matrix) {
            if (!(video_enc->intra_matrix = (uint16_t*)av_mallocz(sizeof(*video_enc->intra_matrix) * 64))) {
                av_log(NULL, AV_LOG_FATAL, "Could not allocate memory for intra matrix.\n");
                exit(1);
            }
            parse_matrix_coeffs(video_enc->intra_matrix, intra_matrix);
        }
        MATCH_PER_STREAM_OPT_2(chroma_intra_matrices, str, chroma_intra_matrix, oc, st);
        if (chroma_intra_matrix) {
            uint16_t *p = (uint16_t *)av_mallocz(sizeof(*video_enc->chroma_intra_matrix) * 64);
            if (!p) {
                av_log(NULL, AV_LOG_FATAL, "Could not allocate memory for intra matrix.\n");
                exit(1);
            }
            video_enc->chroma_intra_matrix = p;
            parse_matrix_coeffs(p, chroma_intra_matrix);
        }
        MATCH_PER_STREAM_OPT_2(inter_matrices, str, inter_matrix, oc, st);
        if (inter_matrix) {
            if (!(video_enc->inter_matrix =(uint16_t*) av_mallocz(sizeof(*video_enc->inter_matrix) * 64))) {
                av_log(NULL, AV_LOG_FATAL, "Could not allocate memory for inter matrix.\n");
                exit(1);
            }
            parse_matrix_coeffs(video_enc->inter_matrix, inter_matrix);
        }

        MATCH_PER_STREAM_OPT_2(rc_overrides, str, p, oc, st);
        for (i = 0; p; i++) {
            int start, end, q;
            int e = sscanf(p, "%d,%d,%d", &start, &end, &q);
            if (e != 3) {
                av_log(NULL, AV_LOG_FATAL, "error parsing rc_override\n");
                exit(1);
            }
            video_enc->rc_override =(RcOverride *)
                av_realloc_array(video_enc->rc_override,
                                 i + 1, sizeof(RcOverride));
            if (!video_enc->rc_override) {
                av_log(NULL, AV_LOG_FATAL, "Could not (re)allocate memory for rc_override.\n");
                exit(1);
            }
            video_enc->rc_override[i].start_frame = start;
            video_enc->rc_override[i].end_frame   = end;
            if (q > 0) {
                video_enc->rc_override[i].qscale         = q;
                video_enc->rc_override[i].quality_factor = 1.0;
            }
            else {
                video_enc->rc_override[i].qscale         = 0;
                video_enc->rc_override[i].quality_factor = -q/100.0;
            }
            p = strchr(p, '/');
            if (p) p++;
        }
        video_enc->rc_override_count = i;

        if (paramCtx->do_psnr)
            video_enc->flags|= AV_CODEC_FLAG_PSNR;

        /* two pass mode */
        MATCH_PER_STREAM_OPT(pass, i, do_pass, oc, st);
        if (do_pass) {
            if (do_pass & 1) {
                video_enc->flags |= AV_CODEC_FLAG_PASS1;
                av_dict_set(&ost->encoder_opts, "flags", "+pass1", AV_DICT_APPEND);
            }
            if (do_pass & 2) {
                video_enc->flags |= AV_CODEC_FLAG_PASS2;
                av_dict_set(&ost->encoder_opts, "flags", "+pass2", AV_DICT_APPEND);
            }
        }

        MATCH_PER_STREAM_OPT_2(passlogfiles, str, ost->logfile_prefix, oc, st);
        if (ost->logfile_prefix &&
            !(ost->logfile_prefix = av_strdup(ost->logfile_prefix)))
            exit(1);

        if (do_pass) {
            char logfilename[1024];
            FILE *f;

            snprintf(logfilename, sizeof(logfilename), "%s-%d.log",
                     ost->logfile_prefix ? ost->logfile_prefix :
                                           DEFAULT_PASS_LOGFILENAME_PREFIX,
                     i);
            if (!strcmp(ost->enc->name, "libx264")) {
                av_dict_set(&ost->encoder_opts, "stats", logfilename, AV_DICT_DONT_OVERWRITE);
            } else {
                if (video_enc->flags & AV_CODEC_FLAG_PASS2) {
                    char  *logbuffer = (char *)read_file(logfilename);

                    if (!logbuffer) {
                        av_log(NULL, AV_LOG_FATAL, "Error reading log file '%s' for pass-2 encoding\n",
                               logfilename);
                        exit(1);
                    }
                    video_enc->stats_in = logbuffer;
                }
                if (video_enc->flags & AV_CODEC_FLAG_PASS1) {
                    f = av_fopen_utf8(logfilename, "wb");
                    if (!f) {
                        av_log(NULL, AV_LOG_FATAL,
                               "Cannot write log file '%s' for pass-1 encoding: %s\n",
                               logfilename, strerror(errno));
                        exit(1);
                    }
                    ost->logfile = f;
                }
            }
        }

        MATCH_PER_STREAM_OPT_2(forced_key_frames, str, ost->forced_keyframes, oc, st);
        if (ost->forced_keyframes)
            ost->forced_keyframes = av_strdup(ost->forced_keyframes);

        MATCH_PER_STREAM_OPT(force_fps, i, ost->force_fps, oc, st);

        ost->top_field_first = -1;
        MATCH_PER_STREAM_OPT(top_field_first, i, ost->top_field_first, oc, st);


        ost->avfilter = get_ost_filters(o, oc, ost);
        if (!ost->avfilter)
            exit(1);
    } else {
        MATCH_PER_STREAM_OPT(copy_initial_nonkeyframes, i, ost->copy_initial_nonkeyframes, oc ,st);
    }

    if (ost->stream_copy)
        check_streamcopy_filters(o, oc, ost, AVMEDIA_TYPE_VIDEO);

    return ost;
}

static OutputStream *new_audio_stream(OptionsContext *o, AVFormatContext *oc, int source_index, FfmpegParamContext* paramCtx)
{
    //int n;
    AVStream *st;
    OutputStream *ost;
    AVCodecContext *audio_enc;

    ost = new_output_stream(o, oc, AVMEDIA_TYPE_AUDIO, source_index, paramCtx);
    st  = ost->st;

    audio_enc = ost->enc_ctx;
    audio_enc->codec_type = AVMEDIA_TYPE_AUDIO;

    MATCH_PER_STREAM_OPT_2(filter_scripts, str, ost->filters_script, oc, st);
    MATCH_PER_STREAM_OPT_2(filters,        str, ost->filters,        oc, st);
    if (o->nb_filters > 1)
        av_log(NULL, AV_LOG_ERROR, "Only '-af %s' read, ignoring remaining -af options: Use ',' to separate filters\n", ost->filters);

    if (!ost->stream_copy) {
        char *sample_fmt = NULL;

        MATCH_PER_STREAM_OPT(audio_channels, i, audio_enc->channels, oc, st);

        MATCH_PER_STREAM_OPT_2(sample_fmts, str, sample_fmt, oc, st);
        if (sample_fmt &&
            (audio_enc->sample_fmt = av_get_sample_fmt(sample_fmt)) == AV_SAMPLE_FMT_NONE) {
            av_log(NULL, AV_LOG_FATAL, "Invalid sample format '%s'\n", sample_fmt);
            exit(1);
        }

        MATCH_PER_STREAM_OPT(audio_sample_rate, i, audio_enc->sample_rate, oc, st);

        MATCH_PER_STREAM_OPT_2(apad, str, ost->apad, oc, st);
        ost->apad = av_strdup(ost->apad);

        ost->avfilter = get_ost_filters(o, oc, ost);
        if (!ost->avfilter)
            exit(1);

        /* check for channel mapping for this audio stream */
//        for (n = 0; n < o->nb_audio_channel_maps; n++) {
//            AudioChannelMap *map = &o->audio_channel_maps[n];
//            if ((map->ofile_idx   == -1 || ost->file_index == map->ofile_idx) &&
//                (map->ostream_idx == -1 || ost->st->index  == map->ostream_idx)) {
//                InputStream *ist;

//                if (map->channel_idx == -1) {
//                    ist = NULL;
//                } else if (ost->source_index < 0) {
//                    av_log(NULL, AV_LOG_FATAL, "Cannot determine input stream for channel mapping %d.%d\n",
//                           ost->file_index, ost->st->index);
//                    continue;
//                } else {
//                    ist = input_streams[ost->source_index];
//                }

//                if (!ist || (ist->file_index == map->file_idx && ist->st->index == map->stream_idx)) {
//                    if (av_reallocp_array(&ost->audio_channels_map,
//                                          ost->audio_channels_mapped + 1,
//                                          sizeof(*ost->audio_channels_map)
//                                          ) < 0 )
//                        exit(1);

//                    ost->audio_channels_map[ost->audio_channels_mapped++] = map->channel_idx;
//                }
//            }
//        }

    }

    if (ost->stream_copy)
        check_streamcopy_filters(o, oc, ost, AVMEDIA_TYPE_AUDIO);

    return ost;
}


/**
 * Parse a metadata specifier passed as 'arg' parameter.
 * @param arg  metadata string to parse
 * @param type metadata type is written here -- g(lobal)/s(tream)/c(hapter)/p(rogram)
 * @param index for type c/p, chapter/program index is written here
 * @param stream_spec for type s, the stream specifier is written here
 */
static void parse_meta_type(char *arg, char *type, int *index, const char **stream_spec)
{
    if (*arg) {
        *type = *arg;
        switch (*arg) {
        case 'g':
            break;
        case 's':
            if (*(++arg) && *arg != ':') {
                av_log(NULL, AV_LOG_FATAL, "Invalid metadata specifier %s.\n", arg);
                exit(1);
            }
            *stream_spec = *arg == ':' ? arg + 1 : "";
            break;
        case 'c':
        case 'p':
            if (*(++arg) == ':')
                *index = strtol(++arg, NULL, 0);
            break;
        default:
            av_log(NULL, AV_LOG_FATAL, "Invalid metadata type %c.\n", *arg);
            exit(1);
        }
    } else
        *type = 'g';
}


//#include fftools/ffmpeg.c
static int guess_input_channel_layout(InputStream *ist)
{
    AVCodecContext *dec = ist->dec_ctx;

    if (!dec->channel_layout) {
        char layout_name[256];

        if (dec->channels > ist->guess_layout_max)
            return 0;
        dec->channel_layout = av_get_default_channel_layout(dec->channels);
        if (!dec->channel_layout)
            return 0;
        av_get_channel_layout_string(layout_name, sizeof(layout_name),
                                     dec->channels, dec->channel_layout);
        av_log(NULL, AV_LOG_WARNING, "Guessed Channel Layout for Input Stream "
               "#%d.%d : %s\n", ist->file_index, ist->st->index, layout_name);
    }
    return 1;
}


#endif  //FFMPEG_DEFINI_H
