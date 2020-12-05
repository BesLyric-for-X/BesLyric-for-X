#include "mp3Editor.h"
#include <QFileInfo>
#include <QDir>
#include <assert.h>

#define RAW_PACKET_BUFFER_SIZE 2500000

#include<QException>
#include <QDebug>
void throwException(int exceptionCode)
{
    qDebug() << "Converting Error : exception code("<<exceptionCode<<")";
    throw QUnhandledException();
}

void ConvertThread::SetConvertedData(QString filePath, const CustomMp3Data &customMp3Data)
{
    inputMp3Path = filePath;
    customData = customMp3Data;
}

int ConvertThread::GetProcessingPercentage()
{
	int percentage = int(paramCtx.percentage * 100);
	if (percentage >= 100)
	{
		if (paramCtx.finish == false)
			percentage = 99;
		else
			percentage = 100;
	}
	return percentage;
}

void ConvertThread::run()
{
    QString errorCause;

    ResetToInitAll();           //重置以初始化所有状态

    //初始化选项内容（仿造ffmpeg.exe）

    /* parse options and open all input/output files */
    int ret = ffmpeg_parse_options();
    if(ret < 0){
        errorCause = QString("error while opening files");
        goto fail;
    }

    //转码输出
    ret = transcode();
    if(ret < 0){
        errorCause = QString("error while transcoding");
        goto fail;
    }

fail:
    ReleaseAll();

    if(ret < 0)
        emit sig_getEditResult(false, inputMp3Path , errorCause);
    else
        emit sig_getEditResult(true, outputMp3Path , "success");

}

void ConvertThread::ResetToInitAll()
{
	paramCtx = FfmpegParamContext();
    sws_dict = swr_opts = format_opts = codec_opts = resample_opts = NULL;
}

void ConvertThread::ReleaseAll()
{
    int i,j;

    /* close files */
    for (i = 0; i < paramCtx.nb_output_files; i++)
    {
        OutputFile *of = paramCtx.output_files[i];
        AVFormatContext *s;
        if (!of)
            continue;

        s = of->ctx;
        if (s && s->oformat && !(s->oformat->flags & AVFMT_NOFILE))
            avio_closep(&s->pb);
        avformat_free_context(s);
        av_dict_free(&of->opts);

        av_freep(&paramCtx.output_files[i]);
    }

    for (i = 0; i < paramCtx.nb_output_streams; i++) {
            OutputStream *ost = paramCtx.output_streams[i];

            if (!ost)
                continue;

            for (j = 0; j < ost->nb_bitstream_filters; j++)
                av_bsf_free(&ost->bsf_ctx[j]);
            av_freep(&ost->bsf_ctx);

            av_frame_free(&ost->filtered_frame);
            av_frame_free(&ost->last_frame);
            av_dict_free(&ost->encoder_opts);

            av_freep(&ost->forced_keyframes);
            av_expr_free(ost->forced_keyframes_pexpr);
            av_freep(&ost->avfilter);
            av_freep(&ost->logfile_prefix);

            av_freep(&ost->audio_channels_map);
            ost->audio_channels_mapped = 0;

            av_dict_free(&ost->sws_dict);

            avcodec_free_context(&ost->enc_ctx);
            avcodec_parameters_free(&ost->ref_par);

            if (ost->muxing_queue) {
                while (av_fifo_size(ost->muxing_queue)) {
                    AVPacket pkt;
                    av_fifo_generic_read(ost->muxing_queue, &pkt, sizeof(pkt), NULL);
                    av_packet_unref(&pkt);
                }
                av_fifo_freep(&ost->muxing_queue);
            }

            av_freep(&paramCtx.output_streams[i]);
        }
    #if HAVE_THREADS
        //free_input_threads();
    #endif

    for (i = 0; i < paramCtx.nb_input_files; i++) {
        avformat_close_input(&paramCtx.input_files[i]->ctx);
        av_freep(&paramCtx.input_files[i]);
    }
    for (i = 0; i < paramCtx.nb_input_streams; i++) {
        InputStream *ist = paramCtx.input_streams[i];

        av_frame_free(&ist->decoded_frame);
        av_frame_free(&ist->filter_frame);
        av_dict_free(&ist->decoder_opts);
        avsubtitle_free(&ist->prev_sub.subtitle);
        av_frame_free(&ist->sub2video.frame);
        //av_freep(&ist->filters);
        av_freep(&ist->hwaccel_device);
        av_freep(&ist->dts_buffer);

        avcodec_free_context(&ist->dec_ctx);

        av_freep(&paramCtx.input_streams[i]);
    }

    //av_freep(&vstats_filename);

    av_freep(&paramCtx.input_streams);
    av_freep(&paramCtx.input_files);
    av_freep(&paramCtx.output_streams);
    av_freep(&paramCtx.output_files);

    uninit_opts();

    //avformat_network_deinit();

	paramCtx.finish = true;
}

int ConvertThread::ffmpeg_parse_options()
{
    OptionParseContext octx;
    char error[128];
    int ret;

    buildOptionContent(&octx);

	//loglevel
	if(paramCtx.openAvLog)
	{ 
		av_log_set_flags(AV_LOG_SKIP_REPEATED);
		av_log_set_level(paramCtx.logLevel);
	}

    try{
        //打开输入的文件（mp3 和 图片）
        ret = open_files(&octx.groups[GROUP_INFILE], true);

        if(ret < 0){
            goto fail;
        }

        //打开输出文件（mp3）
        ret = open_files(&octx.groups[GROUP_OUTFILE], false);

        if(ret < 0){
            goto fail;
        }
    }
    catch(QException&)
    {
        uninit_parse_context(&octx);
        return -1;
    }

fail:
    uninit_parse_context(&octx);
    if (ret < 0) {
        av_strerror(ret, error, sizeof(error));
        av_log(NULL, AV_LOG_FATAL, "%s\n", error);
    }
    return ret;

}

void ConvertThread::init_opts()
{
    av_dict_set(&sws_dict, "flags", "bicubic", 0);
}

void ConvertThread::uninit_opts()
{
    av_dict_free(&swr_opts);
    av_dict_free(&sws_dict);
    av_dict_free(&format_opts);
    av_dict_free(&codec_opts);
    av_dict_free(&resample_opts);
}

void ConvertThread::uninit_options(OptionsContext *o)
{
    int i;

    //parse_optgroup 中分配了字符串等，需要在次释放
    const OptionDef *po = options;

    /* all OPT_SPEC and OPT_STRING can be freed in generic way */
    while (po->name) {
        void *dst = (uint8_t*)o + po->u.off;

        if (po->flags & OPT_SPEC) {
            SpecifierOpt **so = (SpecifierOpt **)dst;
            int i, *count = (int*)(so + 1);
            for (i = 0; i < *count; i++) {
                av_freep(&(*so)[i].specifier);
                if (po->flags & OPT_STRING)
                    av_freep(&(*so)[i].u.str);
            }
            av_freep(so);
            *count = 0;
        } else if (po->flags & OPT_OFFSET && po->flags & OPT_STRING)
            av_freep(dst);
        po++;
    }

    for (i = 0; i < o->nb_stream_maps; i++)
        av_freep(&o->stream_maps[i].linklabel);
    av_freep(&o->stream_maps);
    //av_freep(&o->audio_channel_maps);
    av_freep(&o->streamid_map);
    //av_freep(&o->attachments);
}

void ConvertThread::init_options(OptionsContext *o)
{
    memset(o, 0, sizeof(*o));

    o->stop_time = INT64_MAX;
    o->mux_max_delay  = (float)0.7;
    o->start_time     = AV_NOPTS_VALUE;
    o->start_time_eof = AV_NOPTS_VALUE;
    o->recording_time = INT64_MAX;
    o->limit_filesize = UINT64_MAX;
    o->chapters_input_file = INT_MAX;
    o->accurate_seek  = 1;
}

void ConvertThread::buildOptionContent(OptionParseContext *octx)
{
    memset(octx, 0, sizeof(*octx));
    init_parse_context(octx, groups, FF_ARRAY_ELEMS(groups));

    //构建相关选项

    //构建输入项选项
    //#0
    inputMp3Utf8 = inputMp3Path.toUtf8();//保证变量一直存在
    finish_group(octx,OptGroup::GROUP_INFILE, inputMp3Utf8);

    //#1
    if(!customData.imagePath.isEmpty())
    {
        inputImageUtf8 = customData.imagePath.toUtf8();//保证变量一直存在
        finish_group(octx,OptGroup::GROUP_INFILE, inputImageUtf8);
    }

    //构建输出项选项

    if(!customData.imagePath.isEmpty())
    {
        //有图片文件时，分别使用原来的音频流和新图片的视频流
        add_opt(octx,&optionMap,"map","0:0"); //音乐文件#0的音频流(0)
        add_opt(octx,&optionMap,"map","1:0"); //图片文件#1的视频流(0)
    }
    else
        add_opt(octx,&optionMap,"map","0:0"); //音乐文件#0的音频流(0)

    if( !customData.artist.isEmpty()|| !customData.title.isEmpty()||!customData.album.isEmpty())
    {
         if(!customData.artist.isEmpty()){
             mataArtistUtf8 = QString("artist="+customData.artist).toUtf8();
             add_opt(octx,&optionMetadata,"metadata",mataArtistUtf8);
         }
         if(!customData.title.isEmpty()){
             mataTitleUtf8 = QString("title="+customData.title).toUtf8();
             add_opt(octx,&optionMetadata,"metadata",mataTitleUtf8);
         }
         if(!customData.album.isEmpty()){
             mataAlbumUtf8 = QString("album="+customData.album).toUtf8();
             add_opt(octx,&optionMetadata,"metadata",mataAlbumUtf8);
         }
    }

     //输出使用的codec直接复制使用输入文件的
     add_opt(octx,&optionCodecName,"c","copy");

     //设置 mp3 头格式版本为 3
     av_dict_set(&format_opts, "id3v2_version", "3", 0);

     //构建输出文件的名称
     if(customData.mp3OutputPath.isEmpty())
     {
         QFileInfo fileInfo(inputMp3Path);
         outputMp3Path = fileInfo.dir().absolutePath()+"/"+fileInfo.completeBaseName()+"-converted.mp3";
     }
     else
     {
         outputMp3Path =customData.mp3OutputPath;
     }
     outputMp3Utf8 = outputMp3Path.toUtf8();//保证变量一直存在
     finish_group(octx,OptGroup::GROUP_OUTFILE, outputMp3Utf8);
}

void ConvertThread::init_parse_context(OptionParseContext *octx, const OptionGroupDef *groups, int nb_groups)
{
    static const OptionGroupDef global_group = { "global" };
    int i;

    memset(octx, 0, sizeof(*octx));

    octx->nb_groups = nb_groups;
    octx->groups    = reinterpret_cast<OptionGroupList*>(av_mallocz_array(octx->nb_groups, sizeof(*octx->groups)));
    if (!octx->groups)
        throwException(1);

    for (i = 0; i < octx->nb_groups; i++)
        octx->groups[i].group_def = &groups[i];

    octx->global_opts.group_def = &global_group;
    octx->global_opts.arg       = "";

    init_opts();
}

void ConvertThread::add_input_streams(OptionsContext *o, AVFormatContext *ic)
{
    int ret;

    for (unsigned int i = 0; i < ic->nb_streams; i++)
    {
        AVStream *st = ic->streams[i];
        AVCodecParameters *par = st->codecpar;
        InputStream *ist = (InputStream *)av_mallocz(sizeof(*ist));
        char *framerate = NULL, *hwaccel_device = NULL;
        const char *hwaccel = NULL;
        char *hwaccel_output_format = NULL;
        char *codec_tag = NULL;
        char *next;
        char *discard_str = NULL;
        const AVClass *cc = avcodec_get_class();
        const AVOption *discard_opt = av_opt_find(&cc, "skip_frame", NULL, 0, 0);

        if (!ist)
            throwException(1);

        GROW_ARRAY_2(paramCtx.input_streams, paramCtx.nb_input_streams,InputStream *);

        paramCtx.input_streams[ paramCtx.nb_input_streams - 1] = ist;

        ist->st = st;
        ist->file_index = paramCtx.nb_input_files;
        ist->discard = 1;
        st->discard  = AVDISCARD_ALL;
        ist->nb_samples = 0;
        ist->min_pts = INT64_MAX;
        ist->max_pts = INT64_MIN;

        ist->ts_scale = 1.0;
        MATCH_PER_STREAM_OPT(ts_scale, dbl, ist->ts_scale, ic, st);

        ist->autorotate = 1;
        MATCH_PER_STREAM_OPT(autorotate, i, ist->autorotate, ic, st);

        MATCH_PER_STREAM_OPT_2(codec_tags, str, codec_tag, ic, st);
        if (codec_tag) {
            uint32_t tag = strtol(codec_tag, &next, 0);
            if (*next)
                tag = AV_RL32(codec_tag);
            st->codecpar->codec_tag = tag;
        }

        ist->dec = choose_decoder(o, ic, st);
        ist->decoder_opts = filter_codec_opts(o->g->codec_opts, ist->st->codecpar->codec_id, ic, st, ist->dec);

        ist->reinit_filters = -1;
        MATCH_PER_STREAM_OPT(reinit_filters, i, ist->reinit_filters, ic, st);

        MATCH_PER_STREAM_OPT_2(discard, str, discard_str, ic, st);
        ist->user_set_discard = AVDISCARD_NONE;

        if ((o->video_disable && ist->st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) ||
            (o->audio_disable && ist->st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) ||
            (o->subtitle_disable && ist->st->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) ||
            (o->data_disable && ist->st->codecpar->codec_type == AVMEDIA_TYPE_DATA))
                ist->user_set_discard = AVDISCARD_ALL;

        if (discard_str && av_opt_eval_int(&cc, discard_opt, discard_str, &ist->user_set_discard) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error parsing discard %s.\n",
                    discard_str);
            throwException(1);
        }

        ist->filter_in_rescale_delta_last = AV_NOPTS_VALUE;

        ist->dec_ctx = avcodec_alloc_context3(ist->dec);
        if (!ist->dec_ctx) {
            av_log(NULL, AV_LOG_ERROR, "Error allocating the decoder context.\n");
            throwException(1);
        }

        ret = avcodec_parameters_to_context(ist->dec_ctx, par);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error initializing the decoder context.\n");
            throwException(1);
        }

        if (o->bitexact)
            ist->dec_ctx->flags |= AV_CODEC_FLAG_BITEXACT;

        switch (par->codec_type) {
        case AVMEDIA_TYPE_VIDEO:
            if(!ist->dec)
                ist->dec = avcodec_find_decoder(par->codec_id);
#if FF_API_LOWRES
            if (st->codec->lowres) {
                ist->dec_ctx->lowres = st->codec->lowres;
                ist->dec_ctx->width  = st->codec->width;
                ist->dec_ctx->height = st->codec->height;
                ist->dec_ctx->coded_width  = st->codec->coded_width;
                ist->dec_ctx->coded_height = st->codec->coded_height;
            }
#endif

            // avformat_find_stream_info() doesn't set this for us anymore.
            ist->dec_ctx->framerate = st->avg_frame_rate;

            MATCH_PER_STREAM_OPT_2(frame_rates, str, framerate, ic, st);
            if (framerate && av_parse_video_rate(&ist->framerate,
                                                 framerate) < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error parsing framerate %s.\n",
                       framerate);
                throwException(1);
            }

            ist->top_field_first = -1;
            MATCH_PER_STREAM_OPT(top_field_first, i, ist->top_field_first, ic, st);

            MATCH_PER_STREAM_OPT_2(hwaccels, str, hwaccel, ic, st);
            if (hwaccel) {
                assert(false);
                //未搬运逻辑
                throwException(1);
            }

            MATCH_PER_STREAM_OPT_2(hwaccel_devices, str, hwaccel_device, ic, st);
            if (hwaccel_device) {
                ist->hwaccel_device = av_strdup(hwaccel_device);
                if (!ist->hwaccel_device)
                    throwException(1);
            }

            MATCH_PER_STREAM_OPT_2(hwaccel_output_formats, str,
                                 hwaccel_output_format, ic, st);
            if (hwaccel_output_format) {
                ist->hwaccel_output_format = av_get_pix_fmt(hwaccel_output_format);
                if (ist->hwaccel_output_format == AV_PIX_FMT_NONE) {
                    av_log(NULL, AV_LOG_FATAL, "Unrecognised hwaccel output "
                           "format: %s", hwaccel_output_format);
                }
            } else {
                ist->hwaccel_output_format = AV_PIX_FMT_NONE;
            }

            ist->hwaccel_pix_fmt = AV_PIX_FMT_NONE;

            break;
        case AVMEDIA_TYPE_AUDIO:
            ist->guess_layout_max = INT_MAX;
            MATCH_PER_STREAM_OPT(guess_layout_max, i, ist->guess_layout_max, ic, st);
            guess_input_channel_layout(ist);
            break;
        case AVMEDIA_TYPE_DATA:
        case AVMEDIA_TYPE_SUBTITLE: {
            break;
        }
        case AVMEDIA_TYPE_ATTACHMENT:
        case AVMEDIA_TYPE_UNKNOWN:
            break;
        default:
            abort();
        }

        ret = avcodec_parameters_from_context(par, ist->dec_ctx);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error initializing the decoder context.\n");
            throwException(1);
        }
    }

}

int ConvertThread::open_files(OptionGroupList *l,bool isInput)
{
    int i, ret;
    const char * inout = isInput? "input":"output";

    for (i = 0; i < l->nb_groups; i++) {
        OptionGroup *g = &l->groups[i];
        OptionsContext o;

        init_options(&o);
        o.g = g;

        ret = parse_optgroup(&o, g, (void*)&paramCtx);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error parsing options for %s file "
                                       "%s.\n", inout, g->arg);
            return ret;
        }

        av_log(NULL, AV_LOG_DEBUG, "Opening an %s file: %s.\n", inout, g->arg);

        if(isInput)
            ret = open_input_file(&o, g->arg);
        else
            ret = open_output_file(&o, g->arg);

        uninit_options(&o);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error opening %s file %s.\n",
                   inout, g->arg);
            return ret;
        }
        av_log(NULL, AV_LOG_DEBUG, "Successfully opened the file.\n");
    }

    return 0;
}

int ConvertThread::open_input_file(OptionsContext *o, const char *filename)
{
    static int find_stream_info = 1;

    InputFile *f;
    AVInputFormat *file_iformat = NULL;
    int err, ret;
    unsigned int i;
    int64_t timestamp = 0;
    AVDictionary *unused_opts = NULL;
    AVDictionaryEntry *e = NULL;
    int scan_all_pmts_set = 0;

    //由于自定义AVFormatContext 创建内容时，需要用到 ->av_class = &av_format_context_class
    // 而 av_format_context_class 在 FFmpeg\libavformat\options.c 中定义，不打算搬运过来
    //所以 由后面 avformat_open_input 支持的方式自动分配

    AVFormatContext *ic = NULL;

    if (!av_dict_get(o->g->format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE)) {
        av_dict_set(&o->g->format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
        scan_all_pmts_set = 1;
    }

    /* open the input file with generic avformat function */
    err = avformat_open_input(&ic, filename, file_iformat, &o->g->format_opts);
    if (err < 0) {
        print_error(filename, err);
        if (err == AVERROR_PROTOCOL_NOT_FOUND)
            av_log(NULL, AV_LOG_ERROR, "Did you mean file:%s?\n", filename);
        throwException(1);
    }
    if (scan_all_pmts_set)
        av_dict_set(&o->g->format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE);
    remove_avoptions(&o->g->format_opts, o->g->codec_opts);
    assert_avoptions(o->g->format_opts);

    /* apply forced codec ids */
    for (i = 0; i < ic->nb_streams; i++)
        choose_decoder(o, ic, ic->streams[i]);

    if (find_stream_info) {
        AVDictionary **opts = setup_find_stream_info_opts(ic, o->g->codec_opts);
        unsigned int orig_nb_streams = ic->nb_streams;

        /* If not enough info to get the stream parameters, we decode the
           first frames to get it. (used in mpeg case for example) */
        ret = avformat_find_stream_info(ic, opts);

		if (ic->streams[0]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			paramCtx.totalAudioByte = ic->duration / 1000000 * ic->bit_rate / 8;

        for (i = 0; i < orig_nb_streams; i++)
            av_dict_free(&opts[i]);
        av_freep(&opts);

        if (ret < 0) {
            av_log(NULL, AV_LOG_FATAL, "%s: could not find codec parameters\n", filename);
            if (ic->nb_streams == 0) {
                avformat_close_input(&ic);
                throwException(1);
            }
        }
    }

    /* update the current parameters so that they match the one of the input stream */
    add_input_streams(o, ic);

    /* dump the file content */
    av_dump_format(ic, paramCtx.nb_input_files, filename, 0);

    /* 构建 InputFile *f ，存储到 paramCtx.input_files */

    GROW_ARRAY_2(paramCtx.input_files, paramCtx.nb_input_files,InputFile*);
    f = (InputFile *)av_mallocz(sizeof(*f));
    if (!f)
       throwException(1);

    paramCtx.input_files[paramCtx.nb_input_files - 1] = f;

    f->ctx        = ic;
    f->ist_index  = paramCtx.nb_input_streams - ic->nb_streams;
    f->start_time = o->start_time;
    f->recording_time = o->recording_time;
    f->input_ts_offset = o->input_ts_offset;
    f->ts_offset  = o->input_ts_offset - (paramCtx.copy_ts ? (paramCtx.start_at_zero && ic->start_time != AV_NOPTS_VALUE ? ic->start_time : 0) : timestamp);
    f->nb_streams = ic->nb_streams;
    f->rate_emu   = o->rate_emu;
    f->accurate_seek = o->accurate_seek;
    f->loop = o->loop;
    f->duration = 0;
    f->time_base = { 1, 1};//(AVRational){ 1, 1}
#if HAVE_THREADS
    f->thread_queue_size = o->thread_queue_size > 0 ? o->thread_queue_size : 8;
#endif

    /* check if all codec options have been used */
    unused_opts = strip_specifiers(o->g->codec_opts);
    for (int i = f->ist_index; i < paramCtx.nb_input_streams; i++) {
        e = NULL;
        while ((e = av_dict_get(paramCtx.input_streams[i]->decoder_opts, "", e,
                                AV_DICT_IGNORE_SUFFIX)))
            av_dict_set(&unused_opts, e->key, NULL, 0);
    }

    e = NULL;
    while ((e = av_dict_get(unused_opts, "", e, AV_DICT_IGNORE_SUFFIX))) {
       assert(false);
       //未搬运
       throwException(1);
    }
    av_dict_free(&unused_opts);

    paramCtx.input_stream_potentially_available = 1;

    return 0;
}

int ConvertThread::open_output_file(OptionsContext *o, const char *filename)
{
    AVFormatContext *oc;
    int i,  err;//j,
    OutputFile *of;
    InputStream  *ist;
    OutputStream *ost;
    AVDictionaryEntry *e = NULL;
    int format_flags = 0;

    if (o->stop_time != INT64_MAX && o->recording_time != INT64_MAX) {
        o->stop_time = INT64_MAX;
        av_log(NULL, AV_LOG_WARNING, "-t and -to cannot be used together; using -t.\n");
    }

    GROW_ARRAY_2(paramCtx.output_files, paramCtx.nb_output_files,OutputFile*);
    of = (OutputFile*)av_mallocz(sizeof(*of));
    if (!of)
        throwException(1);

    paramCtx.output_files[paramCtx.nb_output_files - 1] = of;
    of->ost_index      = paramCtx.nb_output_streams;
    of->recording_time = o->recording_time;
    of->start_time     = o->start_time;
    of->limit_filesize = o->limit_filesize;
    of->shortest       = o->shortest;
    av_dict_copy(&of->opts, o->g->format_opts, 0);

    err = avformat_alloc_output_context2(&oc, NULL, o->format, filename);
    if (!oc) {
        print_error(filename, err);
        throwException(1);
    }

    of->ctx = oc;
    if (o->recording_time != INT64_MAX)
        oc->duration = o->recording_time;

    //oc->interrupt_callback = int_cb;

    e = av_dict_get(o->g->format_opts, "fflags", NULL, 0);
    if (e) {
        const AVOption *o = av_opt_find(oc, "fflags", NULL, 0, 0);
        av_opt_eval_flags(oc, o, e->value, &format_flags);
    }
    if (o->bitexact) {
        format_flags |= AVFMT_FLAG_BITEXACT;
        oc->flags    |= AVFMT_FLAG_BITEXACT;
    }

    if (!o->nb_stream_maps)
    {
        //不搬运这部分逻辑
    }
    else
    {
        for (i = 0; i < o->nb_stream_maps; i++) {
           StreamMap *map = &o->stream_maps[i];

           if (map->disabled)
               continue;

           if (map->linklabel)
           {
               //不搬运这部分逻辑
           }
           else
           {
               int src_idx = paramCtx.input_files[map->file_index]->ist_index + map->stream_index;

               ist = paramCtx.input_streams[paramCtx.input_files[map->file_index]->ist_index + map->stream_index];

               if (ist->user_set_discard == AVDISCARD_ALL) {
                   av_log(NULL, AV_LOG_FATAL, "Stream #%d:%d is disabled and cannot be mapped.\n",
                          map->file_index, map->stream_index);
                   throwException(1);
               }
               if(o->subtitle_disable && ist->st->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE)
                   continue;
               if(o->   audio_disable && ist->st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
                   continue;
               if(o->   video_disable && ist->st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
                   continue;
               if(o->    data_disable && ist->st->codecpar->codec_type == AVMEDIA_TYPE_DATA)
                   continue;

               ost = NULL;
               switch (ist->st->codecpar->codec_type) {
               case AVMEDIA_TYPE_VIDEO:      ost = new_video_stream     (o, oc, src_idx,&paramCtx); break;
               case AVMEDIA_TYPE_AUDIO:      ost = new_audio_stream     (o, oc, src_idx,&paramCtx); break;
               default:
                    av_log(NULL, true/*ignore_unknown_streams*/ ? AV_LOG_WARNING : AV_LOG_FATAL,
                           "Cannot map stream #%d:%d - unsupported type.\n",
                           map->file_index, map->stream_index);
               }
               if (ost)
                    ost->sync_ist = paramCtx.input_streams[  paramCtx.input_files[map->sync_file_index]->ist_index
                                                                 + map->sync_stream_index];
           }
        }
    }

    /* handle attached files */
    //不搬运

#if FF_API_LAVF_AVCTX
    for (i = paramCtx.nb_output_streams - oc->nb_streams; i < paramCtx.nb_output_streams; i++) { //for all streams of this output file
        AVDictionaryEntry *e;
        ost = paramCtx.output_streams[i];

        if ((ost->stream_copy || ost->attachment_filename)
            && (e = av_dict_get(o->g->codec_opts, "flags", NULL, AV_DICT_IGNORE_SUFFIX))
            && (!e->key[5] || check_stream_specifier(oc, ost->st, e->key+6)))
            if (av_opt_set(ost->st->codec, "flags", e->value, 0) < 0)
                throwException(1);
    }
#endif

    /* check if all codec options have been used */
    //不搬运

    /* set the decoding_needed flags and create simple filtergraphs */
    //不搬运

    /* check filename in case of an image number is expected */
    //不搬运

    if (!(oc->oformat->flags & AVFMT_NOFILE)) {
        /* test if it already exists to avoid losing precious files */
        assert_file_overwrite(filename);

        /* open the file */
        if ((err = avio_open2(&oc->pb, filename, AVIO_FLAG_WRITE,
                              &oc->interrupt_callback,
                              &of->opts)) < 0) {
            print_error(filename, err);
            throwException(1);
        }
    } else if (strcmp(oc->oformat->name, "image2")==0 && !av_filename_number_test(filename))
        assert_file_overwrite(filename);

//    if (o->mux_preload) {
//        av_dict_set_int(&of->opts, "preload", o->mux_preload*AV_TIME_BASE, 0);
//    }
    oc->max_delay = (int)(o->mux_max_delay * AV_TIME_BASE);

    /* copy metadata */
    //不搬运

    /* copy chapters */
    //不搬运


    /* copy global metadata by default */
    if (!o->metadata_global_manual && paramCtx.nb_input_files){
        av_dict_copy(&oc->metadata, paramCtx.input_files[0]->ctx->metadata,
                     AV_DICT_DONT_OVERWRITE);
        if(o->recording_time != INT64_MAX)
            av_dict_set(&oc->metadata, "duration", NULL, 0);
        av_dict_set(&oc->metadata, "creation_time", NULL, 0);
    }
    if (!o->metadata_streams_manual)
        for (i = of->ost_index; i < paramCtx.nb_output_streams; i++) {
            InputStream *ist;
            if (paramCtx.output_streams[i]->source_index < 0)         /* this is true e.g. for attached files */
                continue;
            ist = paramCtx.input_streams[paramCtx.output_streams[i]->source_index];
            av_dict_copy(&paramCtx.output_streams[i]->st->metadata, ist->st->metadata, AV_DICT_DONT_OVERWRITE);
            if (!paramCtx.output_streams[i]->stream_copy) {
                av_dict_set(&paramCtx.output_streams[i]->st->metadata, "encoder", NULL, 0);
            }
        }

    /* process manually set programs */

    /* process manually set metadata */
    for (i = 0; i < o->nb_metadata; i++) {
            AVDictionary **m;
            char type, *val;
            const char *stream_spec;
            int index = 0,ret = 0;
            unsigned int j;

            val = strchr((char*)o->metadata[i].u.str, '=');
            if (!val) {
                av_log(NULL, AV_LOG_FATAL, "No '=' character in metadata string %s.\n",
                       o->metadata[i].u.str);
                throwException(1);
            }
            *val++ = 0;

            parse_meta_type(o->metadata[i].specifier, &type, &index, &stream_spec);
            if (type == 's') {
                for (j = 0; j < oc->nb_streams; j++) {
                    ost = paramCtx.output_streams[paramCtx.nb_output_streams - oc->nb_streams + j];
                    if ((ret = check_stream_specifier(oc, oc->streams[j], stream_spec)) > 0) {
                        if (!strcmp((char*)o->metadata[i].u.str, "rotate")) {
                            char *tail;
                            double theta = av_strtod(val, &tail);
                            if (!*tail) {
                                ost->rotate_overridden = 1;
                                ost->rotate_override_value = theta;
                            }
                        } else {
                            av_dict_set(&oc->streams[j]->metadata, (char*)o->metadata[i].u.str, *val ? val : NULL, 0);
                        }
                    } else if (ret < 0)
                        throwException(1);
                }
            }
            else {
                switch (type) {
                case 'g':
                    m = &oc->metadata;
                    break;
                case 'c':
                    if (index < 0 || index >= int(oc->nb_chapters)) {
                        av_log(NULL, AV_LOG_FATAL, "Invalid chapter index %d in metadata specifier.\n", index);
                        throwException(1);
                    }
                    m = &oc->chapters[index]->metadata;
                    break;
                case 'p':
                    if (index < 0 || index >= int(oc->nb_programs)) {
                        av_log(NULL, AV_LOG_FATAL, "Invalid program index %d in metadata specifier.\n", index);
                        throwException(1);
                    }
                    m = &oc->programs[index]->metadata;
                    break;
                default:
                    av_log(NULL, AV_LOG_FATAL, "Invalid metadata specifier %s.\n", o->metadata[i].specifier);
                    throwException(1);
                }
                av_dict_set(m, (char*)o->metadata[i].u.str, *val ? val : NULL, 0);
            }
        }

    return 0;
}

int ConvertThread::init_input_stream(int ist_index)
{
    InputStream *ist = paramCtx.input_streams[ist_index];
    if (ist->decoding_needed)
    {
        //不必搬运
    }

    ist->next_pts = AV_NOPTS_VALUE;
    ist->next_dts = AV_NOPTS_VALUE;

    return 0;
}

InputStream* ConvertThread::get_input_stream(OutputStream *ost)
{
    if (ost->source_index >= 0)
        return paramCtx.input_streams[ost->source_index];
    return NULL;
}

int ConvertThread::init_output_stream_streamcopy(OutputStream *ost)
{
    OutputFile *of = paramCtx.output_files[ost->file_index];
    InputStream *ist = get_input_stream(ost);
    AVCodecParameters *par_dst = ost->st->codecpar;
    AVCodecParameters *par_src = ost->ref_par;
    AVRational sar;
    int  ret; //i,
    uint32_t codec_tag = par_dst->codec_tag;

    //av_assert0(ist && !ost->filter);

    ret = avcodec_parameters_to_context(ost->enc_ctx, ist->st->codecpar);
    if (ret >= 0)
        ret = av_opt_set_dict(ost->enc_ctx, &ost->encoder_opts);
    if (ret < 0) {
        av_log(NULL, AV_LOG_FATAL,
               "Error setting up codec context options.\n");
        return ret;
    }

    ret = avcodec_parameters_from_context(par_src, ost->enc_ctx);
    if (ret < 0) {
        av_log(NULL, AV_LOG_FATAL,
               "Error getting reference codec parameters.\n");
        return ret;
    }

    if (!codec_tag) {
        unsigned int codec_tag_tmp;
        if (!of->ctx->oformat->codec_tag ||
            av_codec_get_id (of->ctx->oformat->codec_tag, par_src->codec_tag) == par_src->codec_id ||
            !av_codec_get_tag2(of->ctx->oformat->codec_tag, par_src->codec_id, &codec_tag_tmp))
            codec_tag = par_src->codec_tag;
    }

    ret = avcodec_parameters_copy(par_dst, par_src);
    if (ret < 0)
        return ret;

    par_dst->codec_tag = codec_tag;

    if (!ost->frame_rate.num)
        ost->frame_rate = ist->framerate;
    ost->st->avg_frame_rate = ost->frame_rate;

    ret = avformat_transfer_internal_stream_timing_info(of->ctx->oformat, ost->st, ist->st, AVFMT_TBCF_AUTO );//copy_tb);
    if (ret < 0)
        return ret;

    // copy timebase while removing common factors
    if (ost->st->time_base.num <= 0 || ost->st->time_base.den <= 0)
        ost->st->time_base = av_add_q(av_stream_get_codec_timebase(ost->st), /*(AVRational)*/{0, 1});

    // copy estimated duration as a hint to the muxer
    if (ost->st->duration <= 0 && ist->st->duration > 0)
        ost->st->duration = av_rescale_q(ist->st->duration, ist->st->time_base, ost->st->time_base);

    // copy disposition
    ost->st->disposition = ist->st->disposition;

    if (ist->st->nb_side_data) {
        //不搬运
    }

    if (ost->rotate_overridden) {
        //不搬运
    }

    switch (par_dst->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
//            if (audio_volume != 256) {
//                av_log(NULL, AV_LOG_FATAL, "-acodec copy and -vol are incompatible (frames are not decoded)\n");
//                throwException(1);
//            }
            if((par_dst->block_align == 1 || par_dst->block_align == 1152 || par_dst->block_align == 576) && par_dst->codec_id == AV_CODEC_ID_MP3)
                par_dst->block_align= 0;
            if(par_dst->codec_id == AV_CODEC_ID_AC3)
                par_dst->block_align= 0;
            break;
    case AVMEDIA_TYPE_VIDEO:
        if (ost->frame_aspect_ratio.num) { // overridden by the -aspect cli option
            sar =
                av_mul_q(ost->frame_aspect_ratio,
                         /*(AVRational)*/{ par_dst->height, par_dst->width });
            av_log(NULL, AV_LOG_WARNING, "Overriding aspect ratio "
                   "with stream copy may produce invalid files\n");
            }
        else if (ist->st->sample_aspect_ratio.num)
            sar = ist->st->sample_aspect_ratio;
        else
            sar = par_src->sample_aspect_ratio;
        ost->st->sample_aspect_ratio = par_dst->sample_aspect_ratio = sar;
        ost->st->avg_frame_rate = ist->st->avg_frame_rate;
        ost->st->r_frame_rate = ist->st->r_frame_rate;
        break;
    }

    ost->mux_timebase = ist->st->time_base;

    return 0;
}

/* open the muxer when all the streams are initialized */
int ConvertThread::check_init_output_file(OutputFile *of, int file_index)
{
    int ret;
    unsigned int i;

    for (i = 0; i < of->ctx->nb_streams; i++) {
        OutputStream *ost = paramCtx.output_streams[of->ost_index + i];
        if (!ost->initialized)
            return 0;
    }

    //of->ctx->interrupt_callback = int_cb;

    ret = avformat_write_header(of->ctx, &of->opts);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR,
               "Could not write header for output file #%d "
               "(incorrect codec parameters ?): %s\n",
               file_index, "av_err2str(ret)");
        return ret;
    }
    //assert_avoptions(of->opts);
    of->header_written = 1;

    av_dump_format(of->ctx, file_index, of->ctx->url, 1);

//    if (sdp_filename || want_sdp)
//        print_sdp();


    /* flush the muxing queues */
    for (i = 0; i < of->ctx->nb_streams; i++) {
        OutputStream *ost = paramCtx.output_streams[of->ost_index + i];

        /* try to improve muxing time_base (only possible if nothing has been written yet) */
        if (!av_fifo_size(ost->muxing_queue))
            ost->mux_timebase = ost->st->time_base;

        while (av_fifo_size(ost->muxing_queue)) {
            //不搬运

            AVPacket pkt;
            av_fifo_generic_read(ost->muxing_queue, &pkt, sizeof(pkt), NULL);
            //write_packet(of, &pkt, ost, 1);
        }
    }

    return 0;
}

int ConvertThread::init_output_stream(OutputStream *ost/*, char *error, int error_len*/)
{
    int ret = 0;

    if(ost->encoding_needed)
    {
        //不必搬运
    }
    else if (ost->stream_copy)
    {
        ret = init_output_stream_streamcopy(ost);
        if (ret < 0)
           return ret;
    }

    // parse user provided disposition, and update stream values
    if (ost->disposition)
    {
        //不搬运
    }

    /* initialize bitstream filters for the output stream
     * needs to be done here, because the codec id for streamcopy is not
     * known until now */
    //ret = init_output_bsfs(ost);  //if (!ost->nb_bitstream_filters)return 0; 直接会返回0不必搬运
    if (ret < 0)
        return ret;

    ost->initialized = 1;

    ret = check_init_output_file(paramCtx.output_files[ost->file_index], ost->file_index);
    if (ret < 0)
        return ret;

    return ret;
}

int ConvertThread::transcode_init()
{
    int i, ret;
    OutputStream *ost;
    char error[1024] = {0};

    /* init framerate emulation */
    //不必搬运

    /* init input streams */
    for (i = 0; i < paramCtx.nb_input_streams; i++)
        init_input_stream(i);  //省略部分搬运

    /* open each encoder */
    for (i = 0; i < paramCtx.nb_output_streams; i++) {
        // skip streams fed from filtergraphs until we have a frame for them
        //if (paramCtx.output_streams[i]->filter)
        //    continue;

        ret = init_output_stream(paramCtx.output_streams[i]/*, error, sizeof(error)*/);
        if (ret < 0)
            goto dump_format;
    }

    /* discard unused programs */
    //不必搬运

    /* write headers for files with no streams */
    //不必搬运

dump_format:
   /* dump the stream mapping */
   av_log(NULL, AV_LOG_INFO, "Stream mapping:\n");

   //no filter


   for (i = 0; i < paramCtx.nb_output_streams; i++) {
       ost = paramCtx.output_streams[i];

       if (ost->attachment_filename) {
           /* an attached file */
           av_log(NULL, AV_LOG_INFO, "  File %s -> Stream #%d:%d\n",
                  ost->attachment_filename, ost->file_index, ost->index);
           continue;
       }

       //no filter

       av_log(NULL, AV_LOG_INFO, "  Stream #%d:%d -> #%d:%d",
              paramCtx.input_streams[ost->source_index]->file_index,
              paramCtx.input_streams[ost->source_index]->st->index,
              ost->file_index,
              ost->index);
       if (ost->sync_ist != paramCtx.input_streams[ost->source_index])
           av_log(NULL, AV_LOG_INFO, " [sync #%d:%d]",
                  ost->sync_ist->file_index,
                  ost->sync_ist->st->index);
       if (ost->stream_copy)
           av_log(NULL, AV_LOG_INFO, " (copy)");
       else {
           const AVCodec *in_codec    = paramCtx.input_streams[ost->source_index]->dec;
           const AVCodec *out_codec   = ost->enc;
           const char *decoder_name   = "?";
           const char *in_codec_name  = "?";
           const char *encoder_name   = "?";
           const char *out_codec_name = "?";
           const AVCodecDescriptor *desc;

           if (in_codec) {
               decoder_name  = in_codec->name;
               desc = avcodec_descriptor_get(in_codec->id);
               if (desc)
                   in_codec_name = desc->name;
               if (!strcmp(decoder_name, in_codec_name))
                   decoder_name = "native";
           }

           if (out_codec) {
               encoder_name   = out_codec->name;
               desc = avcodec_descriptor_get(out_codec->id);
               if (desc)
                   out_codec_name = desc->name;
               if (!strcmp(encoder_name, out_codec_name))
                   encoder_name = "native";
           }

           av_log(NULL, AV_LOG_INFO, " (%s (%s) -> %s (%s))",
                  in_codec_name, decoder_name,
                  out_codec_name, encoder_name);
       }
       av_log(NULL, AV_LOG_INFO, "\n");
   }

   if (ret) {
       av_log(NULL, AV_LOG_ERROR, "%s\n", error);
       return ret;
   }

   //atomic_store(&transcode_init_done, 1);

   return 0;
}

void ConvertThread::close_output_stream(OutputStream *ost)
{
    OutputFile *of = paramCtx.output_files[ost->file_index];

    ost->finished = OSTFinished(ost->finished|ENCODER_FINISHED);
    if (of->shortest) {
        int64_t end = av_rescale_q(ost->sync_opts - ost->first_pts, ost->enc_ctx->time_base, {1, AV_TIME_BASE});//AV_TIME_BASE_Q);
        of->recording_time = FFMIN(of->recording_time, end);
    }
}

/* Return 1 if there remain streams where more output is wanted, 0 otherwise. */
int ConvertThread::need_output()
{
    int i;

    for (i = 0; i < paramCtx.nb_output_streams; i++) {
        OutputStream *ost    = paramCtx.output_streams[i];
        OutputFile *of       = paramCtx.output_files[ost->file_index];
        AVFormatContext *os  = paramCtx.output_files[ost->file_index]->ctx;
		
		if (i == 0)
		{	//搜集音频当前已复制的数据量，计算进度
			paramCtx.transAudioByte = ost->data_size;
			paramCtx.percentage = 1.0 * paramCtx.transAudioByte / paramCtx.totalAudioByte;
		}

        uint64_t value = os->pb && avio_tell(os->pb);
        if (ost->finished ||
            (value >= of->limit_filesize))
            continue;
        if (ost->frame_number >= ost->max_frames) {
            unsigned int j;
            for (j = 0; j < of->ctx->nb_streams; j++)
                close_output_stream(paramCtx.output_streams[of->ost_index + j]);
            continue;
        }

        return 1;
    }

    return 0;
}

/**
 * Select the output stream to process.
 *
 * @return  selected output stream, or NULL if none available
 */
OutputStream *ConvertThread::choose_output()
{
    int i;
    int64_t opts_min = INT64_MAX;
    OutputStream *ost_min = NULL;

    for (i = 0; i < paramCtx.nb_output_streams; i++) {
        OutputStream *ost = paramCtx.output_streams[i];
        int64_t opts = ost->st->cur_dts == AV_NOPTS_VALUE ? INT64_MIN :
                       av_rescale_q(ost->st->cur_dts, ost->st->time_base,
                                    {1, AV_TIME_BASE});//AV_TIME_BASE_Q);
        if (ost->st->cur_dts == AV_NOPTS_VALUE)
            av_log(NULL, AV_LOG_DEBUG,
                "cur_dts is invalid st:%d (%d) [init:%d i_done:%d finish:%d] (this is harmless if it occurs once at the start per stream)\n",
                ost->st->index, ost->st->id, ost->initialized, ost->inputs_done, ost->finished);

        if (!ost->initialized && !ost->inputs_done)
            return ost;

        if (!ost->finished && opts < opts_min) {
            opts_min = opts;
            ost_min  = ost->unavailable ? NULL : ost;
        }
    }
    return ost_min;
}

int ConvertThread::get_input_packet(InputFile *f, AVPacket *pkt)
{
    if (f->rate_emu) {
        int i;
        for (i = 0; i < f->nb_streams; i++) {
            InputStream *ist = paramCtx.input_streams[f->ist_index + i];
            int64_t pts = av_rescale(ist->dts, 1000000, AV_TIME_BASE);
            int64_t now = av_gettime_relative() - ist->start;
            if (pts > now)
                return AVERROR(EAGAIN);
        }
    }

#if HAVE_THREADS
    if (nb_input_files > 1)
        return get_input_packet_mt(f, pkt);
#endif
    return av_read_frame(f->ctx, pkt);
}

void ConvertThread::reset_eagain()
{
    int i;
    for (i = 0; i < paramCtx.nb_input_files; i++)
        paramCtx.input_files[i]->eagain = 0;
    for (i = 0; i < paramCtx.nb_output_streams; i++)
        paramCtx.output_streams[i]->unavailable = 0;
}

void ConvertThread::report_new_stream(int input_index, AVPacket *pkt)
{
    InputFile *file = paramCtx.input_files[input_index];
    AVStream *st = file->ctx->streams[pkt->stream_index];

    if (pkt->stream_index < file->nb_streams_warn)
        return;
    av_log(file->ctx, AV_LOG_WARNING,
           "New %s stream %d:%d at pos:%" PRId64 " and DTS:%ss\n",
           av_get_media_type_string(st->codecpar->codec_type),
           input_index, pkt->stream_index,
           pkt->pos, "av_ts2timestr(pkt->dts, &st->time_base)");
    file->nb_streams_warn = pkt->stream_index + 1;
}

/* sub2video hack:
   Convert subtitles to video with alpha to insert them in filter graphs.
   This is a temporary solution until libavfilter gets real subtitles support.
 */
int ConvertThread::sub2video_get_blank_frame(InputStream *ist)
{
    int ret;
    AVFrame *frame = ist->sub2video.frame;

    av_frame_unref(frame);
    ist->sub2video.frame->width  = ist->dec_ctx->width  ? ist->dec_ctx->width  : ist->sub2video.w;
    ist->sub2video.frame->height = ist->dec_ctx->height ? ist->dec_ctx->height : ist->sub2video.h;
    ist->sub2video.frame->format = AV_PIX_FMT_RGB32;
    if ((ret = av_frame_get_buffer(frame, 32)) < 0)
        return ret;
    memset(frame->data[0], 0, frame->height * frame->linesize[0]);
    return 0;
}

void ConvertThread::sub2video_copy_rect(uint8_t *dst, int dst_linesize, int w, int h, AVSubtitleRect *r)
{
    uint32_t *pal, *dst2;
    uint8_t *src, *src2;
    int x, y;

    if (r->type != SUBTITLE_BITMAP) {
        av_log(NULL, AV_LOG_WARNING, "sub2video: non-bitmap subtitle\n");
        return;
    }
    if (r->x < 0 || r->x + r->w > w || r->y < 0 || r->y + r->h > h) {
        av_log(NULL, AV_LOG_WARNING, "sub2video: rectangle (%d %d %d %d) overflowing %d %d\n",
               r->x, r->y, r->w, r->h, w, h
               );
        return;
    }

    dst += r->y * dst_linesize + r->x * 4;
    src = r->data[0];
    pal = (uint32_t *)r->data[1];
    for (y = 0; y < r->h; y++) {
        dst2 = (uint32_t *)dst;
        src2 = src;
        for (x = 0; x < r->w; x++)
            *(dst2++) = pal[*(src2++)];
        dst += dst_linesize;
        src += r->linesize[0];
    }
}

void ConvertThread::sub2video_push_ref(InputStream *ist, int64_t pts)
{
    AVFrame *frame = ist->sub2video.frame;
    int i;
    //int ret;

    //av_assert1(frame->data[0]);
    ist->sub2video.last_pts = frame->pts = pts;
    for (i = 0; i < ist->nb_filters; i++) {
        //no  filter

//        ret = av_buffersrc_add_frame_flags(ist->filters[i]->filter, frame,
//                                           AV_BUFFERSRC_FLAG_KEEP_REF |
//                                           AV_BUFFERSRC_FLAG_PUSH);
//        if (ret != AVERROR_EOF && ret < 0)
//            av_log(NULL, AV_LOG_WARNING, "Error while add the frame to buffer source(%s).\n",
//                   "av_err2str(ret)");
    }
}

void ConvertThread::sub2video_update(InputStream *ist, AVSubtitle *sub)
{
    AVFrame *frame = ist->sub2video.frame;
    uint8_t *dst;
    int     dst_linesize;
    int num_rects, i;
    int64_t pts, end_pts;

    if (!frame)
        return;
    if (sub) {
        pts       = av_rescale_q(sub->pts + sub->start_display_time * 1000LL,
        {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/, ist->st->time_base);
        end_pts   = av_rescale_q(sub->pts + sub->end_display_time   * 1000LL,
        {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/, ist->st->time_base);
        num_rects = sub->num_rects;
    } else {
        pts       = ist->sub2video.end_pts;
        end_pts   = INT64_MAX;
        num_rects = 0;
    }
    if (sub2video_get_blank_frame(ist) < 0) {
        av_log(ist->dec_ctx, AV_LOG_ERROR,
               "Impossible to get a blank canvas.\n");
        return;
    }
    dst          = frame->data    [0];
    dst_linesize = frame->linesize[0];
    for (i = 0; i < num_rects; i++)
        sub2video_copy_rect(dst, dst_linesize, frame->width, frame->height, sub->rects[i]);
    sub2video_push_ref(ist, pts);
    ist->sub2video.end_pts = end_pts;
}

void ConvertThread::sub2video_heartbeat(InputStream *ist, int64_t pts)
{
    InputFile *infile = paramCtx.input_files[ist->file_index];
    int i, /*j,*/ nb_reqs = 0;
    int64_t pts2;

    /* When a frame is read from a file, examine all sub2video streams in
           the same file and send the sub2video frame again. Otherwise, decoded
           video frames could be accumulating in the filter graph while a filter
           (possibly overlay) is desperately waiting for a subtitle frame. */
    for (i = 0; i < infile->nb_streams; i++) {
        InputStream *ist2 = paramCtx.input_streams[infile->ist_index + i];
        if (!ist2->sub2video.frame)
            continue;
        /* subtitles seem to be usually muxed ahead of other streams;
               if not, subtracting a larger time here is necessary */
        pts2 = av_rescale_q(pts, ist->st->time_base, ist2->st->time_base) - 1;
        /* do not send the heartbeat frame if the subtitle is already ahead */
        if (pts2 <= ist2->sub2video.last_pts)
            continue;
        if (pts2 >= ist2->sub2video.end_pts ||
                (!ist2->sub2video.frame->data[0] && ist2->sub2video.end_pts < INT64_MAX))
            sub2video_update(ist2, NULL);

        //no filter
//        for (j = 0, nb_reqs = 0; j < ist2->nb_filters; j++)
//            nb_reqs += av_buffersrc_get_nb_failed_requests(ist2->filters[j]->filter);

        if (nb_reqs)
            sub2video_push_ref(ist2, pts2);
    }
}

int ConvertThread::check_output_constraints(InputStream *ist, OutputStream *ost)
{
    OutputFile *of = paramCtx.output_files[ost->file_index];
    int ist_index  = paramCtx.input_files[ist->file_index]->ist_index + ist->st->index;

    if (ost->source_index != ist_index)
        return 0;

    if (ost->finished)
        return 0;

    if (of->start_time != AV_NOPTS_VALUE && ist->pts < of->start_time)
        return 0;

    return 1;
}

void ConvertThread::close_all_output_streams(OutputStream *ost, OSTFinished this_stream, OSTFinished others)
{
    int i;
    for (i = 0; i < paramCtx.nb_output_streams; i++) {
        OutputStream *ost2 = paramCtx.output_streams[i];
        ost2->finished =OSTFinished( ost2->finished |(ost == ost2 ? this_stream : others));
    }
}

void ConvertThread::write_packet(OutputFile *of, AVPacket *pkt, OutputStream *ost, int unqueue)
{
    AVFormatContext *s = of->ctx;
    AVStream *st = ost->st;
    int ret;

    /*
         * Audio encoders may split the packets --  #frames in != #packets out.
         * But there is no reordering, so we can limit the number of output packets
         * by simply dropping them here.
         * Counting encoded video frames needs to be done separately because of
         * reordering, see do_video_out().
         * Do not count the packet when unqueued because it has been counted when queued.
         */
    if (!(st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && ost->encoding_needed) && !unqueue) {
        if (ost->frame_number >= ost->max_frames) {
            av_packet_unref(pkt);
            return;
        }
        ost->frame_number++;
    }

    if (!of->header_written) {
        AVPacket tmp_pkt = {0};
        /* the muxer is not initialized yet, buffer the packet */
        if (!av_fifo_space(ost->muxing_queue)) {
            int new_size = FFMIN(2 * av_fifo_size(ost->muxing_queue),
                                 ost->max_muxing_queue_size);
            if (new_size <= av_fifo_size(ost->muxing_queue)) {
                av_log(NULL, AV_LOG_ERROR,
                       "Too many packets buffered for output stream %d:%d.\n",
                       ost->file_index, ost->st->index);
                throwException(1);
            }
            ret = av_fifo_realloc2(ost->muxing_queue, new_size);
            if (ret < 0)
                throwException(1);
        }
        ret = av_packet_make_refcounted(pkt);
        if (ret < 0)
            throwException(1);
        av_packet_move_ref(&tmp_pkt, pkt);
        av_fifo_generic_write(ost->muxing_queue, &tmp_pkt, sizeof(tmp_pkt), NULL);
        return;
    }

    if ((st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && paramCtx.video_sync_method == VSYNC_DROP) ||
            (st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && paramCtx.audio_sync_method < 0))
        pkt->pts = pkt->dts = AV_NOPTS_VALUE;

    if (st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
        int i;
        uint8_t *sd = av_packet_get_side_data(pkt, AV_PKT_DATA_QUALITY_STATS,
                                              NULL);
        ost->quality = sd ? AV_RL32(sd) : -1;
        ost->pict_type = sd ? sd[4] : AV_PICTURE_TYPE_NONE;

        for (i = 0; i<FF_ARRAY_ELEMS(ost->error); i++) {
            if (sd && i < sd[5])
                ost->error[i] = AV_RL64(sd + 8 + 8*i);
            else
                ost->error[i] = -1;
        }

        if (ost->frame_rate.num && ost->is_cfr) {
            if (pkt->duration > 0)
                av_log(NULL, AV_LOG_WARNING, "Overriding packet duration by frame rate, this should not happen\n");
            pkt->duration = av_rescale_q(1, av_inv_q(ost->frame_rate),
                                         ost->mux_timebase);
        }
    }

    av_packet_rescale_ts(pkt, ost->mux_timebase, ost->st->time_base);

    if (!(s->oformat->flags & AVFMT_NOTIMESTAMPS)) {
        if (pkt->dts != AV_NOPTS_VALUE &&
                pkt->pts != AV_NOPTS_VALUE &&
                pkt->dts > pkt->pts) {
            av_log(s, AV_LOG_WARNING, "Invalid DTS: %" PRId64 " PTS: %" PRId64 " in output stream %d:%d, replacing by guess\n",
                   pkt->dts, pkt->pts,
                   ost->file_index, ost->st->index);
            pkt->pts =
                    pkt->dts = pkt->pts + pkt->dts + ost->last_mux_dts + 1
                    - FFMIN3(pkt->pts, pkt->dts, ost->last_mux_dts + 1)
                    - FFMAX3(pkt->pts, pkt->dts, ost->last_mux_dts + 1);
        }
        if ((st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO || st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO || st->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) &&
                pkt->dts != AV_NOPTS_VALUE &&
                !(st->codecpar->codec_id == AV_CODEC_ID_VP9 && ost->stream_copy) &&
                ost->last_mux_dts != AV_NOPTS_VALUE) {
            int64_t max = ost->last_mux_dts + !(s->oformat->flags & AVFMT_TS_NONSTRICT);
            if (pkt->dts < max) {
                int loglevel = max - pkt->dts > 2 || st->codecpar->codec_type == AVMEDIA_TYPE_VIDEO ? AV_LOG_WARNING : AV_LOG_DEBUG;
                av_log(s, loglevel, "Non-monotonous DTS in output stream "
                                    "%d:%d; previous: %" PRId64 ", current: %" PRId64 "; ",
                       ost->file_index, ost->st->index, ost->last_mux_dts, pkt->dts);
                if (paramCtx.exit_on_error) {
                    av_log(NULL, AV_LOG_FATAL, "aborting.\n");
                    throwException(1);
                }
                av_log(s, loglevel, "changing to %" PRId64 ". This may result "
                                                         "in incorrect timestamps in the output file.\n",
                       max);
                if (pkt->pts >= pkt->dts)
                    pkt->pts = FFMAX(pkt->pts, max);
                pkt->dts = max;
            }
        }
    }
    ost->last_mux_dts = pkt->dts;

    ost->data_size += pkt->size;
    ost->packets_written++;

    pkt->stream_index = ost->index;

//    if (debug_ts) {
//        av_log(NULL, AV_LOG_INFO, "muxer <- type:%s "
//                                  "pkt_pts:%s pkt_pts_time:%s pkt_dts:%s pkt_dts_time:%s size:%d\n",
//               av_get_media_type_string(ost->enc_ctx->codec_type),
//               av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, &ost->st->time_base),
//               av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, &ost->st->time_base),
//               pkt->size
//               );
//    }

    ret = av_interleaved_write_frame(s, pkt);
    if (ret < 0) {
        print_error("av_interleaved_write_frame()", ret);
        paramCtx.main_return_code = 1;
        close_all_output_streams(ost, OSTFinished(MUXER_FINISHED | ENCODER_FINISHED), OSTFinished(ENCODER_FINISHED));
    }
    av_packet_unref(pkt);
}

void ConvertThread::output_packet(OutputFile *of, AVPacket *pkt, OutputStream *ost, int eof)
{
    int ret = 0;

    /* apply the output bitstream filters, if any */
    if (ost->nb_bitstream_filters) {
        int idx;

        ret = av_bsf_send_packet(ost->bsf_ctx[0], eof ? NULL : pkt);
        if (ret < 0)
            goto finish;

        eof = 0;
        idx = 1;
        while (idx) {
            /* get a packet from the previous filter up the chain */
            ret = av_bsf_receive_packet(ost->bsf_ctx[idx - 1], pkt);
            if (ret == AVERROR(EAGAIN)) {
                ret = 0;
                idx--;
                continue;
            } else if (ret == AVERROR_EOF) {
                eof = 1;
            } else if (ret < 0)
                goto finish;

            /* send it to the next filter down the chain or to the muxer */
            if (idx < ost->nb_bitstream_filters) {
                ret = av_bsf_send_packet(ost->bsf_ctx[idx], eof ? NULL : pkt);
                if (ret < 0)
                    goto finish;
                idx++;
                eof = 0;
            } else if (eof)
                goto finish;
            else
                write_packet(of, pkt, ost, 0);
        }
    } else if (!eof)
        write_packet(of, pkt, ost, 0);

finish:
    if (ret < 0 && ret != AVERROR_EOF) {
        av_log(NULL, AV_LOG_ERROR, "Error applying bitstream filters to an output "
                                   "packet for stream #%d:%d.\n", ost->file_index, ost->index);
        if(paramCtx.exit_on_error)
            throwException(1);
    }
}

void ConvertThread::do_streamcopy(InputStream *ist, OutputStream *ost, const AVPacket *pkt)
{
    OutputFile *of = paramCtx.output_files[ost->file_index];
    InputFile   *f = paramCtx.input_files [ist->file_index];
    int64_t start_time = (of->start_time == AV_NOPTS_VALUE) ? 0 : of->start_time;
    int64_t ost_tb_start_time = av_rescale_q(start_time, {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/, ost->mux_timebase);
    AVPacket opkt = { 0 };

    av_init_packet(&opkt);

    // EOF: flush output bitstream filters.
    if (!pkt) {
        output_packet(of, &opkt, ost, 1);
        return;
    }

    if ((!ost->frame_number && !(pkt->flags & AV_PKT_FLAG_KEY)) &&
            !ost->copy_initial_nonkeyframes)
        return;

    if (!ost->frame_number && !ost->copy_prior_start) {
        int64_t comp_start = start_time;
        if (paramCtx.copy_ts && f->start_time != AV_NOPTS_VALUE)
            comp_start = FFMAX(start_time, f->start_time + f->ts_offset);
        if (pkt->pts == AV_NOPTS_VALUE ?
                ist->pts < comp_start :
                pkt->pts < av_rescale_q(comp_start, {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/, ist->st->time_base))
            return;
    }

    if (of->recording_time != INT64_MAX &&
            ist->pts >= of->recording_time + start_time) {
        close_output_stream(ost);
        return;
    }

    if (f->recording_time != INT64_MAX) {
        start_time = f->ctx->start_time;
        if (f->start_time != AV_NOPTS_VALUE && paramCtx.copy_ts)
            start_time += f->start_time;
        if (ist->pts >= f->recording_time + start_time) {
            close_output_stream(ost);
            return;
        }
    }

    /* force the input stream PTS */
    if (ost->enc_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
        ost->sync_opts++;

    if (pkt->pts != AV_NOPTS_VALUE)
        opkt.pts = av_rescale_q(pkt->pts, ist->st->time_base, ost->mux_timebase) - ost_tb_start_time;
    else
        opkt.pts = AV_NOPTS_VALUE;

    if (pkt->dts == AV_NOPTS_VALUE)
        opkt.dts = av_rescale_q(ist->dts, {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/, ost->mux_timebase);
    else
        opkt.dts = av_rescale_q(pkt->dts, ist->st->time_base, ost->mux_timebase);
    opkt.dts -= ost_tb_start_time;

    if (ost->st->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && pkt->dts != AV_NOPTS_VALUE) {
        int duration = av_get_audio_frame_duration(ist->dec_ctx, pkt->size);
        if(!duration)
            duration = ist->dec_ctx->frame_size;
        opkt.dts = opkt.pts = av_rescale_delta(ist->st->time_base, pkt->dts,
                                               /*(AVRational)*/{1, ist->dec_ctx->sample_rate}, duration, &ist->filter_in_rescale_delta_last,
                                               ost->mux_timebase) - ost_tb_start_time;
    }

    opkt.duration = av_rescale_q(pkt->duration, ist->st->time_base, ost->mux_timebase);

    opkt.flags    = pkt->flags;

    if (pkt->buf) {
        opkt.buf = av_buffer_ref(pkt->buf);
        if (!opkt.buf)
            throwException(1);
    }
    opkt.data = pkt->data;
    opkt.size = pkt->size;

    av_copy_packet_side_data(&opkt, pkt);

    output_packet(of, &opkt, ost, 0);
}

/* pkt = NULL means EOF (needed to flush decoder buffers) */
int ConvertThread::process_input_packet(InputStream *ist, const AVPacket *pkt, int no_eof)
{
    Q_UNUSED(no_eof)

    int /*ret = 0,*/ i;
    //int repeating = 0;
    int eof_reached = 0;

    AVPacket avpkt;
    if (!ist->saw_first_ts) {
        ist->dts = ist->st->avg_frame_rate.num ? - ist->dec_ctx->has_b_frames * AV_TIME_BASE / av_q2d(ist->st->avg_frame_rate) : 0;
        ist->pts = 0;
        if (pkt && pkt->pts != AV_NOPTS_VALUE && !ist->decoding_needed) {
            ist->dts += av_rescale_q(pkt->pts, ist->st->time_base, {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/);
            ist->pts = ist->dts; //unused but better to set it to a value thats not totally wrong
        }
        ist->saw_first_ts = 1;
    }

    if (ist->next_dts == AV_NOPTS_VALUE)
        ist->next_dts = ist->dts;
    if (ist->next_pts == AV_NOPTS_VALUE)
        ist->next_pts = ist->pts;

    if (!pkt) {
        /* EOF handling */
        av_init_packet(&avpkt);
        avpkt.data = NULL;
        avpkt.size = 0;
    } else {
        avpkt = *pkt;
    }

    if (pkt && pkt->dts != AV_NOPTS_VALUE) {
        ist->next_dts = ist->dts = av_rescale_q(pkt->dts, ist->st->time_base, {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/);
        if (ist->dec_ctx->codec_type != AVMEDIA_TYPE_VIDEO || !ist->decoding_needed)
            ist->next_pts = ist->pts = ist->dts;
    }

    // while we have more to decode or while the decoder did output something on EOF
    //    while (ist->decoding_needed)

    /* handle stream copy */
    if (!ist->decoding_needed && pkt) {
        ist->dts = ist->next_dts;
        switch (ist->dec_ctx->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
            //av_assert1(pkt->duration >= 0);
            if (ist->dec_ctx->sample_rate) {
                ist->next_dts += ((int64_t)AV_TIME_BASE * ist->dec_ctx->frame_size) /
                        ist->dec_ctx->sample_rate;
            } else {
                ist->next_dts += av_rescale_q(pkt->duration, ist->st->time_base, {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/);
            }
            break;
        case AVMEDIA_TYPE_VIDEO:
            if (ist->framerate.num) {
                // TODO: Remove work-around for c99-to-c89 issue 7
                AVRational time_base_q = {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/;
                int64_t next_dts = av_rescale_q(ist->next_dts, time_base_q, av_inv_q(ist->framerate));
                ist->next_dts = av_rescale_q(next_dts + 1, av_inv_q(ist->framerate), time_base_q);
            } else if (pkt->duration) {
                ist->next_dts += av_rescale_q(pkt->duration, ist->st->time_base, {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/);
            } else if(ist->dec_ctx->framerate.num != 0) {
                int ticks= av_stream_get_parser(ist->st) ? av_stream_get_parser(ist->st)->repeat_pict + 1 : ist->dec_ctx->ticks_per_frame;
                ist->next_dts += ((int64_t)AV_TIME_BASE *
                                  ist->dec_ctx->framerate.den * ticks) /
                        ist->dec_ctx->framerate.num / ist->dec_ctx->ticks_per_frame;
            }
            break;
        }
        ist->pts = ist->dts;
        ist->next_pts = ist->next_dts;
    }

    for (i = 0; i < paramCtx.nb_output_streams; i++) {
        OutputStream *ost = paramCtx.output_streams[i];

        if (!check_output_constraints(ist, ost) || ost->encoding_needed)
            continue;

        do_streamcopy(ist, ost, pkt);
    }

    return !eof_reached;
}

void ConvertThread::print_final_stats(int64_t total_size)
{
    uint64_t video_size = 0, audio_size = 0, extra_size = 0, other_size = 0;
    uint64_t subtitle_size = 0;
    int64_t data_size = 0;
    float percent = -1.0;
    int i;
    unsigned int j;
    int pass1_used = 1;

    for (i = 0; i < paramCtx.nb_output_streams; i++) {
        OutputStream *ost = paramCtx.output_streams[i];
        switch (ost->enc_ctx->codec_type) {
        case AVMEDIA_TYPE_VIDEO: video_size += ost->data_size; break;
        case AVMEDIA_TYPE_AUDIO: audio_size += ost->data_size; break;
        case AVMEDIA_TYPE_SUBTITLE: subtitle_size += ost->data_size; break;
        default:                 other_size += ost->data_size; break;
        }
        extra_size += ost->enc_ctx->extradata_size;
        data_size  += ost->data_size;
        if (   (ost->enc_ctx->flags & (AV_CODEC_FLAG_PASS1 | AV_CODEC_FLAG_PASS2))
               != AV_CODEC_FLAG_PASS1)
            pass1_used = 0;
    }

    if (data_size && total_size>0 && total_size >= data_size)
        percent = 100.0 * (total_size - data_size) / data_size;

    av_log(NULL, AV_LOG_INFO, "video:%1.0fkB audio:%1.0fkB subtitle:%1.0fkB other streams:%1.0fkB global headers:%1.0fkB muxing overhead: ",
           video_size / 1024.0,
           audio_size / 1024.0,
           subtitle_size / 1024.0,
           other_size / 1024.0,
           extra_size / 1024.0);
    if (percent >= 0.0)
        av_log(NULL, AV_LOG_INFO, "%f%%", percent);
    else
        av_log(NULL, AV_LOG_INFO, "unknown");
    av_log(NULL, AV_LOG_INFO, "\n");

    /* print verbose per-stream stats */
    for (i = 0; i < paramCtx.nb_input_files; i++) {
        InputFile *f = paramCtx.input_files[i];
        uint64_t total_packets = 0, total_size = 0;

        av_log(NULL, AV_LOG_VERBOSE, "Input file #%d (%s):\n",
               i, f->ctx->url);

        for (int j = 0; j < f->nb_streams; j++) {
            InputStream *ist = paramCtx.input_streams[f->ist_index + j];
            enum AVMediaType type = ist->dec_ctx->codec_type;

            total_size    += ist->data_size;
            total_packets += ist->nb_packets;
#define media_type_string av_get_media_type_string
            av_log(NULL, AV_LOG_VERBOSE, "  Input stream #%d:%d (%s): ",
                   i, j, media_type_string(type));
            av_log(NULL, AV_LOG_VERBOSE, "%" PRIu64 " packets read (%" PRIu64 " bytes); ",
                   ist->nb_packets, ist->data_size);

            if (ist->decoding_needed) {
                av_log(NULL, AV_LOG_VERBOSE, "%" PRIu64 " frames decoded",
                       ist->frames_decoded);
                if (type == AVMEDIA_TYPE_AUDIO)
                    av_log(NULL, AV_LOG_VERBOSE, " (%" PRIu64 " samples)", ist->samples_decoded);
                av_log(NULL, AV_LOG_VERBOSE, "; ");
            }

            av_log(NULL, AV_LOG_VERBOSE, "\n");
        }

        av_log(NULL, AV_LOG_VERBOSE, "  Total: %" PRIu64 " packets (%" PRIu64 " bytes) demuxed\n",
               total_packets, total_size);
    }

    for (i = 0; i < paramCtx.nb_output_files; i++) {
        OutputFile *of = paramCtx.output_files[i];
        uint64_t total_packets = 0, total_size = 0;

        av_log(NULL, AV_LOG_VERBOSE, "Output file #%d (%s):\n",
               i, of->ctx->url);

        for (j = 0; j < of->ctx->nb_streams; j++) {
            OutputStream *ost = paramCtx.output_streams[of->ost_index + j];
            enum AVMediaType type = ost->enc_ctx->codec_type;

            total_size    += ost->data_size;
            total_packets += ost->packets_written;

            av_log(NULL, AV_LOG_VERBOSE, "  Output stream #%d:%d (%s): ",
                   i, j, media_type_string(type));
            if (ost->encoding_needed) {
                av_log(NULL, AV_LOG_VERBOSE, "%" PRIu64 " frames encoded",
                       ost->frames_encoded);
                if (type == AVMEDIA_TYPE_AUDIO)
                    av_log(NULL, AV_LOG_VERBOSE, " (%" PRIu64 " samples)", ost->samples_encoded);
                av_log(NULL, AV_LOG_VERBOSE, "; ");
            }

            av_log(NULL, AV_LOG_VERBOSE, "%" PRIu64 " packets muxed (%" PRIu64 " bytes); ",
                   ost->packets_written, ost->data_size);

            av_log(NULL, AV_LOG_VERBOSE, "\n");
        }

        av_log(NULL, AV_LOG_VERBOSE, "  Total: %" PRIu64 " packets (%" PRIu64 " bytes) muxed\n",
               total_packets, total_size);
    }
    if(video_size + data_size + audio_size + subtitle_size + extra_size == 0){
        av_log(NULL, AV_LOG_WARNING, "Output file is empty, nothing was encoded ");
        if (pass1_used) {
            av_log(NULL, AV_LOG_WARNING, "\n");
        } else {
            av_log(NULL, AV_LOG_WARNING, "(check -ss / -t / -frames parameters if used)\n");
        }
    }
}

static double psnr(double d)
{
    return -10.0 * log10(d);
}
void ConvertThread::print_report(int is_last_report, int64_t timer_start, int64_t cur_time)
{
    AVBPrint buf, buf_script;
    OutputStream *ost;
    AVFormatContext *oc;
    int64_t total_size;
    AVCodecContext *enc;
    int frame_number, vid, i;
    double bitrate;
    double speed;
    int64_t pts = INT64_MIN + 1;
    static int64_t last_time = -1;
    static int qp_histogram[52];
    int hours, mins, secs, us;
    const char *hours_sign;
    int ret;
    float t;

    if (!paramCtx.print_stats && !is_last_report && !paramCtx.progress_avio)
        return;

    if (!is_last_report) {
        if (last_time == -1) {
            last_time = cur_time;
            return;
        }
        if ((cur_time - last_time) < 500000)
            return;
        last_time = cur_time;
    }

    t = (cur_time-timer_start) / 1000000.0;


    oc = paramCtx.output_files[0]->ctx;

    total_size = avio_size(oc->pb);
    if (total_size <= 0) // FIXME improve avio_size() so it works with non seekable output too
        total_size = avio_tell(oc->pb);

    vid = 0;
    av_bprint_init(&buf, 0, AV_BPRINT_SIZE_AUTOMATIC);
    av_bprint_init(&buf_script, 0, AV_BPRINT_SIZE_AUTOMATIC);
    for (i = 0; i < paramCtx.nb_output_streams; i++) {
        float q = -1;
        ost = paramCtx.output_streams[i];
        enc = ost->enc_ctx;
        if (!ost->stream_copy)
            q = ost->quality / (float) FF_QP2LAMBDA;

        if (vid && enc->codec_type == AVMEDIA_TYPE_VIDEO) {
            av_bprintf(&buf, "q=%2.1f ", q);
            av_bprintf(&buf_script, "stream_%d_%d_q=%.1f\n",
                       ost->file_index, ost->index, q);
        }
        if (!vid && enc->codec_type == AVMEDIA_TYPE_VIDEO) {
            float fps;

            frame_number = ost->frame_number;
            fps = t > 1 ? frame_number / t : 0;
            av_bprintf(&buf, "frame=%5d fps=%3.*f q=%3.1f ",
                       frame_number, fps < 9.95, fps, q);
            av_bprintf(&buf_script, "frame=%d\n", frame_number);
            av_bprintf(&buf_script, "fps=%.2f\n", fps);
            av_bprintf(&buf_script, "stream_%d_%d_q=%.1f\n",
                       ost->file_index, ost->index, q);
            if (is_last_report)
                av_bprintf(&buf, "L");
            if (paramCtx.qp_hist) {
                int j;
                int qp = lrintf(q);
                if (qp >= 0 && qp < FF_ARRAY_ELEMS(qp_histogram))
                    qp_histogram[qp]++;
                for (j = 0; j < 32; j++)
                    av_bprintf(&buf, "%X", av_log2(qp_histogram[j] + 1));
            }

            if ((enc->flags & AV_CODEC_FLAG_PSNR) && (ost->pict_type != AV_PICTURE_TYPE_NONE || is_last_report)) {
                int j;
                double error, error_sum = 0;
                double scale, scale_sum = 0;
                double p;
                char type[3] = { 'Y','U','V' };
                av_bprintf(&buf, "PSNR=");
                for (j = 0; j < 3; j++) {
                    if (is_last_report) {
                        error = enc->error[j];
                        scale = enc->width * enc->height * 255.0 * 255.0 * frame_number;
                    } else {
                        error = ost->error[j];
                        scale = enc->width * enc->height * 255.0 * 255.0;
                    }
                    if (j)
                        scale /= 4;
                    error_sum += error;
                    scale_sum += scale;
                    p = psnr(error / scale);
                    av_bprintf(&buf, "%c:%2.2f ", type[j], p);
                    av_bprintf(&buf_script, "stream_%d_%d_psnr_%c=%2.2f\n",
                               ost->file_index, ost->index, type[j] | 32, p);
                }
                p = psnr(error_sum / scale_sum);
                av_bprintf(&buf, "*:%2.2f ", psnr(error_sum / scale_sum));
                av_bprintf(&buf_script, "stream_%d_%d_psnr_all=%2.2f\n",
                           ost->file_index, ost->index, p);
            }
            vid = 1;
        }
        /* compute min output value */
        if (av_stream_get_end_pts(ost->st) != AV_NOPTS_VALUE)
            pts = FFMAX(pts, av_rescale_q(av_stream_get_end_pts(ost->st),
                                          ost->st->time_base, {1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/));
        if (is_last_report)
            paramCtx.nb_frames_drop += ost->last_dropped;
    }

    secs = FFABS(pts) / AV_TIME_BASE;
    us = FFABS(pts) % AV_TIME_BASE;
    mins = secs / 60;
    secs %= 60;
    hours = mins / 60;
    mins %= 60;
    hours_sign = (pts < 0) ? "-" : "";

    bitrate = pts && total_size >= 0 ? total_size * 8 / (pts / 1000.0) : -1;
    speed = t != 0.0 ? (double)pts / AV_TIME_BASE / t : -1;

    if (total_size < 0) av_bprintf(&buf, "size=N/A time=");
    else                av_bprintf(&buf, "size=%8.0fkB time=", total_size / 1024.0);
    if (pts == AV_NOPTS_VALUE) {
        av_bprintf(&buf, "N/A ");
    } else {
        av_bprintf(&buf, "%s%02d:%02d:%02d.%02d ",
                   hours_sign, hours, mins, secs, (100 * us) / AV_TIME_BASE);
    }

    if (bitrate < 0) {
        av_bprintf(&buf, "bitrate=N/A");
        av_bprintf(&buf_script, "bitrate=N/A\n");
    }else{
        av_bprintf(&buf, "bitrate=%6.1fkbits/s", bitrate);
        av_bprintf(&buf_script, "bitrate=%6.1fkbits/s\n", bitrate);
    }

    if (total_size < 0) av_bprintf(&buf_script, "total_size=N/A\n");
    else                av_bprintf(&buf_script, "total_size=%" PRId64 "\n", total_size);
    if (pts == AV_NOPTS_VALUE) {
        av_bprintf(&buf_script, "out_time_us=N/A\n");
        av_bprintf(&buf_script, "out_time_ms=N/A\n");
        av_bprintf(&buf_script, "out_time=N/A\n");
    } else {
        av_bprintf(&buf_script, "out_time_us=%" PRId64 "\n", pts);
        av_bprintf(&buf_script, "out_time_ms=%" PRId64 "\n", pts);
        av_bprintf(&buf_script, "out_time=%s%02d:%02d:%02d.%06d\n",
                   hours_sign, hours, mins, secs, us);
    }

    if (paramCtx.nb_frames_dup || paramCtx.nb_frames_drop)
        av_bprintf(&buf, " dup=%d drop=%d", paramCtx.nb_frames_dup, paramCtx.nb_frames_drop);
    av_bprintf(&buf_script, "dup_frames=%d\n", paramCtx.nb_frames_dup);
    av_bprintf(&buf_script, "drop_frames=%d\n", paramCtx.nb_frames_drop);

    if (speed < 0) {
        av_bprintf(&buf, " speed=N/A");
        av_bprintf(&buf_script, "speed=N/A\n");
    } else {
        av_bprintf(&buf, " speed=%4.3gx", speed);
        av_bprintf(&buf_script, "speed=%4.3gx\n", speed);
    }

    if (paramCtx.print_stats || is_last_report) {
        const char end = is_last_report ? '\n' : '\r';
        if (paramCtx.print_stats==1 && AV_LOG_INFO > av_log_get_level()) {
            fprintf(stderr, "%s    %c", buf.str, end);
        } else
            av_log(NULL, AV_LOG_INFO, "%s    %c", buf.str, end);

        fflush(stderr);
    }
    av_bprint_finalize(&buf, NULL);

    if (paramCtx.progress_avio) {
        av_bprintf(&buf_script, "progress=%s\n",
                   is_last_report ? "end" : "continue");
        avio_write(paramCtx.progress_avio, (const unsigned char *)buf_script.str,
                   FFMIN(buf_script.len, buf_script.size - 1));
        avio_flush(paramCtx.progress_avio);
        av_bprint_finalize(&buf_script, NULL);
        if (is_last_report) {
            if ((ret = avio_closep(&paramCtx.progress_avio)) < 0)
                av_log(NULL, AV_LOG_ERROR,
                       "Error closing progress log, loss of information possible: %s\n", "av_err2str(ret)");
        }
    }

    if (is_last_report)
        print_final_stats(total_size);
}

void ConvertThread::finish_output_stream(OutputStream *ost)
{
    OutputFile *of = paramCtx.output_files[ost->file_index];
    unsigned int i;

    ost->finished = OSTFinished(ENCODER_FINISHED | MUXER_FINISHED);

    if (of->shortest) {
        for (i = 0; i < of->ctx->nb_streams; i++)
            paramCtx.output_streams[of->ost_index + i]->finished = OSTFinished(ENCODER_FINISHED | MUXER_FINISHED);
    }
}

/*
 * Return
 * - 0 -- one packet was read and processed
 * - AVERROR(EAGAIN) -- no packets were available for selected file,
 *   this function should be called again
 * - AVERROR_EOF -- this function should not be called again
 */
int ConvertThread::process_input(int file_index)
{
    InputFile *ifile = paramCtx.input_files[file_index];
    AVFormatContext *is;
    InputStream *ist;
    AVPacket pkt;
    int ret,/* thread_ret,*/ i, j;
    int64_t duration;
    int64_t pkt_dts;

    is  = ifile->ctx;
    ret = get_input_packet(ifile, &pkt);

    if (ret == AVERROR(EAGAIN)) {
        ifile->eagain = 1;
        return ret;
    }

    if (ret < 0 && ifile->loop) {
        //待定
    }

    if (ret < 0) {
        if (ret != AVERROR_EOF) {
            print_error(is->url, ret);
            if (paramCtx.exit_on_error)
                throwException(1);
        }

        for (i = 0; i < ifile->nb_streams; i++) {
            ist = paramCtx.input_streams[ifile->ist_index + i];
            if (ist->decoding_needed) {
                ret = process_input_packet(ist, NULL, 0);
                if (ret>0)
                    return 0;
            }

            /* mark all outputs that don't go through lavfi as finished */
            for (j = 0; j < paramCtx.nb_output_streams; j++) {
                OutputStream *ost = paramCtx.output_streams[j];

                if (ost->source_index == ifile->ist_index + i &&
                    (ost->stream_copy || ost->enc->type == AVMEDIA_TYPE_SUBTITLE))
                    finish_output_stream(ost);
            }
        }

        ifile->eof_reached = 1;
        return AVERROR(EAGAIN);
    }

    reset_eagain();

    if (paramCtx.do_pkt_dump) {
        av_pkt_dump_log2(NULL, AV_LOG_INFO, &pkt, paramCtx.do_hex_dump,
                         is->streams[pkt.stream_index]);
    }

    /* the following test is needed in case new streams appear
       dynamically in stream : we ignore them */
    if (pkt.stream_index >= ifile->nb_streams) {
        report_new_stream(file_index, &pkt);
        goto discard_packet;
    }

    ist = paramCtx.input_streams[ifile->ist_index + pkt.stream_index];

    ist->data_size += pkt.size;
    ist->nb_packets++;

    if (ist->discard)
        goto discard_packet;

    if (pkt.flags & AV_PKT_FLAG_CORRUPT) {
        av_log(NULL, AV_LOG_FATAL,//exit_on_error ? AV_LOG_FATAL : AV_LOG_WARNING,
               "%s: corrupt input packet in stream %d\n", is->url, pkt.stream_index);
        if (paramCtx.exit_on_error)
            throwException(1);
    }

    //if (debug_ts) {}

    //if(!ist->wrap_correction_done && is->start_time != AV_NOPTS_VALUE && ist->st->pts_wrap_bits < 64)

    /* add the stream-global side data to the first packet */
    //if (ist->nb_packets == 1) {

    if (pkt.dts != AV_NOPTS_VALUE)
        pkt.dts += av_rescale_q(ifile->ts_offset,{1, AV_TIME_BASE} /*AV_TIME_BASE_Q*/, ist->st->time_base);
    if (pkt.pts != AV_NOPTS_VALUE)
        pkt.pts += av_rescale_q(ifile->ts_offset, {1, AV_TIME_BASE}/*AV_TIME_BASE_Q*/, ist->st->time_base);

    if (pkt.pts != AV_NOPTS_VALUE)
        pkt.pts *= ist->ts_scale;
    if (pkt.dts != AV_NOPTS_VALUE)
        pkt.dts *= ist->ts_scale;

    pkt_dts = av_rescale_q_rnd(pkt.dts, ist->st->time_base, {1, AV_TIME_BASE}/*AV_TIME_BASE_Q*/, AVRounding(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));

    //    if ((ist->dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO ||
    //             ist->dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) &&
    //            pkt_dts != AV_NOPTS_VALUE && ist->next_dts == AV_NOPTS_VALUE && !copy_ts
    //            && (is->iformat->flags & AVFMT_TS_DISCONT) && ifile->last_ts != AV_NOPTS_VALUE) {

    duration = av_rescale_q(ifile->duration, ifile->time_base, ist->st->time_base);
    if (pkt.pts != AV_NOPTS_VALUE) {
        pkt.pts += duration;
        ist->max_pts = FFMAX(pkt.pts, ist->max_pts);
        ist->min_pts = FFMIN(pkt.pts, ist->min_pts);
    }

    if (pkt.dts != AV_NOPTS_VALUE)
        pkt.dts += duration;

    pkt_dts = av_rescale_q_rnd(pkt.dts, ist->st->time_base,  {1, AV_TIME_BASE}/*AV_TIME_BASE_Q*/, AVRounding(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
    if ((ist->dec_ctx->codec_type == AVMEDIA_TYPE_VIDEO ||
         ist->dec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) &&
            pkt_dts != AV_NOPTS_VALUE && ist->next_dts != AV_NOPTS_VALUE &&
            !paramCtx.copy_ts) {
        int64_t delta   = pkt_dts - ist->next_dts;
        if (is->iformat->flags & AVFMT_TS_DISCONT) {
            if (delta < -1LL*paramCtx.dts_delta_threshold*AV_TIME_BASE ||
                    delta >  1LL*paramCtx.dts_delta_threshold*AV_TIME_BASE ||
                    pkt_dts + AV_TIME_BASE/10 < FFMAX(ist->pts, ist->dts)) {
                ifile->ts_offset -= delta;
                av_log(NULL, AV_LOG_DEBUG,
                       "timestamp discontinuity for stream #%d:%d "
                       "(id=%d, type=%s): %" PRId64 ", new offset= %" PRId64 "\n",
                       ist->file_index, ist->st->index, ist->st->id,
                       av_get_media_type_string(ist->dec_ctx->codec_type),
                       delta, ifile->ts_offset);
                pkt.dts -= av_rescale_q(delta, {1, AV_TIME_BASE}/*AV_TIME_BASE_Q*/, ist->st->time_base);
                if (pkt.pts != AV_NOPTS_VALUE)
                    pkt.pts -= av_rescale_q(delta, {1, AV_TIME_BASE}/*AV_TIME_BASE_Q*/, ist->st->time_base);
            }
        } else {
            if ( delta < -1LL*paramCtx.dts_error_threshold*AV_TIME_BASE ||
                 delta >  1LL*paramCtx.dts_error_threshold*AV_TIME_BASE) {
                av_log(NULL, AV_LOG_WARNING, "DTS %" PRId64 ", next:%" PRId64 " st:%d invalid dropping\n", pkt.dts, ist->next_dts, pkt.stream_index);
                pkt.dts = AV_NOPTS_VALUE;
            }
            if (pkt.pts != AV_NOPTS_VALUE){
                int64_t pkt_pts = av_rescale_q(pkt.pts, ist->st->time_base, {1, AV_TIME_BASE}/*AV_TIME_BASE_Q*/);
                delta   = pkt_pts - ist->next_dts;
                if ( delta < -1LL*paramCtx.dts_error_threshold*AV_TIME_BASE ||
                     delta >  1LL*paramCtx.dts_error_threshold*AV_TIME_BASE) {
                    av_log(NULL, AV_LOG_WARNING, "PTS %" PRId64 ", next:%" PRId64 " invalid dropping st:%d\n", pkt.pts, ist->next_dts, pkt.stream_index);
                    pkt.pts = AV_NOPTS_VALUE;
                }
            }
        }
    }

    if (pkt.dts != AV_NOPTS_VALUE)
        ifile->last_ts = av_rescale_q(pkt.dts, ist->st->time_base, {1, AV_TIME_BASE}/*AV_TIME_BASE_Q*/);

    //if (debug_ts) {

    sub2video_heartbeat(ist, pkt.pts);

    process_input_packet(ist, &pkt, 0);

discard_packet:
    av_packet_unref(&pkt);

    return 0;
}

/**
 * Run a single step of transcoding.
 *
 * @return  0 for success, <0 for error
 */
int ConvertThread::transcode_step()
{
    OutputStream *ost;
    InputStream  *ist = NULL;
    int ret;

    ost = choose_output();
    if (!ost) {


    }

    //no filter
    //if (ost->filter && !ost->filter->graph->graph) {}

    //av_assert0(ost->source_index >= 0);
    ist = paramCtx.input_streams[ost->source_index];

    ret = process_input(ist->file_index);
    if (ret == AVERROR(EAGAIN)) {
        if (paramCtx.input_files[ist->file_index]->eagain)
            ost->unavailable = 1;
        return 0;
    }

    if (ret < 0)
        return ret == AVERROR_EOF ? 0 : ret;

    return 0;//reap_filters(0);
}

int ConvertThread::transcode()
{
    int64_t timer_start;
    InputStream *ist;
    AVFormatContext *os;
    OutputStream *ost;
    int64_t total_packets_written = 0;
    int i;

    int ret = transcode_init();
    if (ret < 0)
        goto fail;

    timer_start = av_gettime_relative();

    #if HAVE_THREADS
        //if ((ret = init_input_threads()) < 0)
        //   goto fail;
    #endif

    //while(!received_sigterm)
    while(true)
    {
        int64_t cur_time= av_gettime_relative();

        //可通过定义局部变量来控制结束这个过程
        //        /* if 'q' pressed, exits */
        //        if (stdin_interaction)
        //            if (check_keyboard_interaction(cur_time) < 0)
        //                break;

        /* check if there's any stream where output is still needed */
        if (!need_output()) {
            av_log(NULL, AV_LOG_VERBOSE, "No more output streams to write to, finishing.\n");
            break;
        }

        ret = transcode_step();
        if (ret < 0 && ret != AVERROR_EOF) {
            av_log(NULL, AV_LOG_ERROR, "Error while filtering: %s\n", "av_err2str(ret)");
            break;
        }

        /* dump report by using the output first video and audio streams */
        print_report(0, timer_start, cur_time);
    }

#if HAVE_THREADS
    //free_input_threads();
#endif

    /* at the end of stream, we must flush the decoder buffers */
    for (i = 0; i < paramCtx.nb_input_streams; i++) {
        ist = paramCtx.input_streams[i];
        if (!paramCtx.input_files[ist->file_index]->eof_reached) {
            process_input_packet(ist, NULL, 0);
        }
    }

    //flush_encoders(); //if (!ost->encoding_needed) continue;

    /* write the trailer if needed and close file */
    for (i = 0; i < paramCtx.nb_output_files; i++) {
        os = paramCtx.output_files[i]->ctx;
        if (!paramCtx.output_files[i]->header_written) {
            av_log(NULL, AV_LOG_ERROR,
                   "Nothing was written into output file %d (%s), because "
                   "at least one of its streams received no packets.\n",
                   i, os->url);
            continue;
        }
        if ((ret = av_write_trailer(os)) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Error writing trailer of %s: %s\n", os->url, "av_err2str(ret)");
            if (paramCtx.exit_on_error)
                throwException(1);
        }
    }

    /* dump report by using the first video and audio streams */
    print_report(1, timer_start, av_gettime_relative());

    /* close each encoder */
    for (i = 0; i < paramCtx.nb_output_streams; i++) {
        ost = paramCtx.output_streams[i];
        if (ost->encoding_needed) {
            av_freep(&ost->enc_ctx->stats_in);
        }
        total_packets_written += ost->packets_written;
    }

    if (!total_packets_written /*&& (abort_on_flags & ABORT_ON_FLAG_EMPTY_OUTPUT)*/) {
        av_log(NULL, AV_LOG_FATAL, "Empty output\n");
        throwException(1);
    }

    /* close each decoder */
    for (i = 0; i < paramCtx.nb_input_streams; i++) {
        ist = paramCtx.input_streams[i];
        if (ist->decoding_needed) {
            avcodec_close(ist->dec_ctx);
            if (ist->hwaccel_uninit)
                ist->hwaccel_uninit(ist->dec_ctx);
        }
    }

//    av_buffer_unref(&paramCtx.hw_device_ctx);
//    hw_device_free_all();

    /* finished ! */
    ret = 0;

fail:
#if HAVE_THREADS
   //free_input_threads();
#endif

    if (paramCtx.output_streams) {
        for (i = 0; i < paramCtx.nb_output_streams; i++) {
            ost = paramCtx.output_streams[i];
            if (ost) {
                if (ost->logfile) {
                    if (fclose(ost->logfile))
                        av_log(NULL, AV_LOG_ERROR,
                               "Error closing logfile, loss of information possible: %s\n",
                               "av_err2str(AVERROR(errno))");
                    ost->logfile = NULL;
                }
                av_freep(&ost->forced_kf_pts);
                av_freep(&ost->apad);
                av_freep(&ost->disposition);
                av_dict_free(&ost->encoder_opts);
                av_dict_free(&ost->sws_dict);
                av_dict_free(&ost->swr_opts);
                av_dict_free(&ost->resample_opts);
            }
        }
    }

    return ret;
}

void ConvertThread::hw_device_free_all()
{
    int i;
    for (i = 0; i < paramCtx.nb_hw_devices; i++) {
        av_freep(&paramCtx.hw_devices[i]->name);
        av_buffer_unref(&paramCtx.hw_devices[i]->device_ref);
        av_freep(&paramCtx.hw_devices[i]);
    }
    av_freep(&paramCtx.hw_devices);
    paramCtx.nb_hw_devices = 0;
}

void ConvertThread::avformat_get_context_defaults(AVFormatContext *s)
{
    memset(s, 0, sizeof(AVFormatContext));

    //s->av_class = &av_format_context_class;

    //s->io_open  = io_open_default;
    //s->io_close = io_close_default;

    av_opt_set_defaults(s);
}

void ConvertThread::finish_group(OptionParseContext *octx, int group_idx, const char *arg)
{
    OptionGroupList *l = &octx->groups[group_idx];
    OptionGroup *g;

    GROW_ARRAY_2(l->groups, l->nb_groups, OptionGroup);
    //l->groups = (OptionGroup *)grow_array(l->groups, sizeof(*l->groups), &l->nb_groups, l->nb_groups + 1);

    g = &l->groups[l->nb_groups - 1];

    *g             = octx->cur_group;
    g->arg         = arg;
    g->group_def   = l->group_def;
    g->sws_dict    = sws_dict;
    g->swr_opts    = swr_opts;
    g->codec_opts  = codec_opts;
    g->format_opts = format_opts;
    g->resample_opts = resample_opts;

    codec_opts  = NULL;
    format_opts = NULL;
    resample_opts = NULL;
    sws_dict    = NULL;
    swr_opts    = NULL;
    init_opts();

    memset(&octx->cur_group, 0, sizeof(octx->cur_group));
}

void ConvertThread::add_opt(OptionParseContext *octx, const OptionDef *opt, const char *key, const char *val)
{
    //int global = !(opt->flags & (OPT_PERFILE | OPT_SPEC | OPT_OFFSET));
    //OptionGroup *g = global ? &octx->global_opts : &octx->cur_group;

    //这里模拟的行为中，没有 loglevel,y 等全局参数，构建的选项都是下一个 output 文件的局部参数选项
    OptionGroup *g = &octx->cur_group;

    GROW_ARRAY_2(g->opts, g->nb_opts, Option);
    //g->opts = (Option*)grow_array(g->opts, sizeof(*g->opts), &g->nb_opts, g->nb_opts + 1);


    g->opts[g->nb_opts - 1].opt = opt;
    g->opts[g->nb_opts - 1].key = key;
    g->opts[g->nb_opts - 1].val = val;
}

void ConvertThread::print_error(const char *filename, int err)
{
    char errbuf[128];
    const char *errbuf_ptr = errbuf;

    if (av_strerror(err, errbuf, sizeof(errbuf)) < 0)
        errbuf_ptr = strerror(AVUNERROR(err));
    av_log(NULL, AV_LOG_ERROR, "%s: %s\n", filename, errbuf_ptr);
}

void ConvertThread::remove_avoptions(AVDictionary **a, AVDictionary *b)
{
    AVDictionaryEntry *t = NULL;

    while ((t = av_dict_get(b, "", t, AV_DICT_IGNORE_SUFFIX))) {
        av_dict_set(a, t->key, NULL, AV_DICT_MATCH_CASE);
    }
}

void ConvertThread::assert_avoptions(AVDictionary *m)
{
    AVDictionaryEntry *t;
    if ((t = av_dict_get(m, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_FATAL, "Option %s not found.\n", t->key);
        throwException(1);
    }
}

int ConvertThread::read_yesno()
{
    int c = getchar();
    int yesno = (av_toupper(c) == 'Y');

    while (c != '\n' && c != EOF)
        c = getchar();

    return yesno;
}

void ConvertThread::assert_file_overwrite(const char *filename)
{
    const char *proto_name = avio_find_protocol_name(filename);

    if (paramCtx.file_overwrite && paramCtx.no_file_overwrite) {
        fprintf(stderr, "Error, both -y and -n supplied. Exiting.\n");
        throwException(1);
    }

    if (!paramCtx.file_overwrite) {
        if (proto_name && !strcmp(proto_name, "file") && avio_check(filename, 0) == 0) {

            fprintf(stderr,"File '%s' already exists. Overwriting ", filename);

            if(false)//不检验是否重写，默认重写就好
            {
                if (paramCtx.stdin_interaction && !paramCtx.no_file_overwrite) {
                    fprintf(stderr,"File '%s' already exists. Overwrite ? [y/N] ", filename);
                    fflush(stderr);
                    //term_exit();
                    //signal(SIGINT, SIG_DFL);
                    if (!read_yesno()) {
                        av_log(NULL, AV_LOG_FATAL, "Not overwriting - exiting\n");
                        throwException(1);
                    }
                    //term_init();
                }
                else {
                    av_log(NULL, AV_LOG_FATAL, "File '%s' already exists. Exiting.\n", filename);
                    throwException(1);
                }
            }
        }
    }

    if (proto_name && !strcmp(proto_name, "file")) {
        for (int i = 0; i < paramCtx.nb_input_files; i++) {
            InputFile *file = paramCtx.input_files[i];
            if (file->ctx->iformat->flags & AVFMT_NOFILE)
                continue;
            if (!strcmp(filename, file->ctx->url)) {
                av_log(NULL, AV_LOG_FATAL, "Output %s same as Input #%d - exiting\n", filename, i);
                av_log(NULL, AV_LOG_WARNING, "FFmpeg cannot edit existing files in-place.\n");
                throwException(1);
            }
        }
    }
}

void ConvertThread::uninit_parse_context(OptionParseContext *octx)
{
    int i, j;

    for (i = 0; i < octx->nb_groups; i++) {
        OptionGroupList *l = &octx->groups[i];

        for (j = 0; j < l->nb_groups; j++) {
            av_freep(&l->groups[j].opts);
            av_dict_free(&l->groups[j].codec_opts);
            av_dict_free(&l->groups[j].format_opts);
            av_dict_free(&l->groups[j].resample_opts);

            av_dict_free(&l->groups[j].sws_dict);
            av_dict_free(&l->groups[j].swr_opts);
        }
        av_freep(&l->groups);
    }
    av_freep(&octx->groups);

    av_freep(&octx->cur_group.opts);
    av_freep(&octx->global_opts.opts);

    uninit_opts();
}



Mp3Editor::Mp3Editor(QObject *parent):QObject(parent)
{
    convertThread = new ConvertThread(this);

    connect(convertThread, &ConvertThread::sig_getEditResult, this, &Mp3Editor::OnGetEditReuslt);

	processTimer.setInterval(100);
	connect(&processTimer, &QTimer::timeout, this, &Mp3Editor::OnSendProcessing);

}

Mp3Editor::~Mp3Editor()
{

}

bool Mp3Editor::CustomizeMp3(QString filePath, const CustomMp3Data &customData)
{
    if(convertThread->isRunning())
        return false;

    CustomMp3Data cusData = customData;
//    cusData.imagePath = "E:/openSourceGit/msvc/bin/x86/test.jpg";
//    cusData.artist = "bsArtist";
//    cusData.album = "bsAlbum";
//    cusData.title = "bsTitle";
//    filePath = "E:/openSourceGit/msvc/bin/x86/a.mp3";

    //TODO:添加转换进度消息

	processTimer.start();
    convertThread->SetConvertedData(filePath, cusData);
    convertThread->start(QThread::Priority::IdlePriority);

    return true;
}

void Mp3Editor::OnGetEditReuslt(bool success, QString path, QString errorTip)
{
    emit sig_getEditResult(success, path, errorTip);
	processTimer.stop();
}

void Mp3Editor::OnSendProcessing()
{
	int p = convertThread->GetProcessingPercentage();
	if (p != nLastPercentage)
	{
		nLastPercentage = p;
		emit sig_processChange(p);
	}
}
