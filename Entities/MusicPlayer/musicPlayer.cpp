#include "musicPlayer.h"
#include <QDebug>
#include <qmutex.h>

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

//Use SDL
#define USE_SDL 1

//挂起毫秒时间
void _millisecondSleep(uint msecond)
{
#ifdef _WIN32
    //Windows
    _sleep(msecond);
#else
    //Linux...
    usleep(msecond*1000);
#endif
}


//Buffer:
//|-----------|-------------|
//chunk-------pos---len-----|
static  Uint8  *audio_chunk;
static  Uint32  audio_len;
static  Uint8  *audio_pos;

static bool oldPlayWay = false;         //之前尝试的旧的播放方法，可以后退，快进av_seek_frame 会出错
static bool outputWav = false;          //用于测试输出wav

static bool logAudio = false;

FILE *pFile;
Uint64 pcmLen;

/* wav header */

//下面这四个结构体是为了分析wav头的
typedef struct {
    uint magic;      /* 'RIFF' */
    uint length;     /* filelen */
    uint type;       /* 'WAVE' */
} WaveHeader;

typedef struct {
    ushort format;       /* see WAV_FMT_* */
    ushort channels;
    uint sample_fq;      /* frequence of sample */
    uint byte_p_sec;
    ushort byte_p_spl;   /* samplesize; 1 or 2 bytes */
    ushort bit_p_spl;    /* 8, 12 or 16 bit */
} WaveFmtBody;

typedef struct {
    uint type;        /* 'data' */
    uint length;      /* samplecount */
} WaveChunkHeader;

#define COMPOSE_ID(a,b,c,d) ((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define WAV_RIFF COMPOSE_ID('R','I','F','F')
#define WAV_WAVE COMPOSE_ID('W','A','V','E')
#define WAV_FMT COMPOSE_ID('f','m','t',' ')
#define WAV_DATA COMPOSE_ID('d','a','t','a')
int insert_wave_header(FILE* fp, long data_len)
{
    int len;
    WaveHeader* header;
    WaveChunkHeader* chunk;
    WaveFmtBody* body;

    fseek(fp, 0, SEEK_SET);        //写到wav文件的开始处

    len = sizeof(WaveHeader)+sizeof(WaveFmtBody)+sizeof(WaveChunkHeader)*2;
    char* buf = (char*)malloc(len);
    header = (WaveHeader*)buf;
    header->magic = WAV_RIFF;
    header->length = data_len + sizeof(WaveFmtBody)+sizeof(WaveChunkHeader)*2 + 4;
    header->type = WAV_WAVE;

    chunk = (WaveChunkHeader*)(buf+sizeof(WaveHeader));
    chunk->type = WAV_FMT;
    chunk->length = 16;

    body = (WaveFmtBody*)(buf+sizeof(WaveHeader)+sizeof(WaveChunkHeader));
    body->format = (ushort)0x0001;      //编码方式为pcm
    body->channels = (ushort)0x02;      //声道数为2
    body->sample_fq = 44100;             //采样频率为44.1k
    body->byte_p_sec = 176400;           //每秒所需字节数 44100*2*2=采样频率*声道*采样位数
    body->byte_p_spl = (ushort)0x4;     //对齐无意义
    body->bit_p_spl = (ushort)16;       //采样位数16bit=2Byte


    chunk = (WaveChunkHeader*)(buf+sizeof(WaveHeader)+sizeof(WaveChunkHeader)+sizeof(WaveFmtBody));
    chunk->type = WAV_DATA;
    chunk->length = data_len;

    fwrite(buf, 1, len, fp);
    free(buf);
    return 0;
}


/////////////////////////////////////////////



QMutex g_threadMutex;		//保证线程 Run 内初始化队列 和 销毁队列等操作不同时进行

/////////////////////////////////////////////////
/* 包队列相关操作 */

static bool g_isQuit=false; //清空了

// 包队列初始化
void packet_queue_init(PacketQueue* q)
{
    memset(q, 0, sizeof(PacketQueue));
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

// 放入packet到队列中，不带头指针的队列
int packet_queue_put(PacketQueue*q, AVPacket *pkt)
{
    AVPacketList *pktl;
    if (av_dup_packet(pkt) < 0)
        return -1;

    pktl = (AVPacketList*)av_malloc(sizeof(AVPacketList));
    if (!pktl)
        return -1;

    pktl->pkt = *pkt;
    pktl->next = nullptr;

    SDL_LockMutex(q->mutex);

    if (!q->last_pkt) // 队列为空，新插入元素为第一个元素
        q->first_pkt = pktl;
    else // 插入队尾
        q->last_pkt->next = pktl;

    q->last_pkt = pktl;

    q->nb_packets++;
    q->size += pkt->size;

    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);
    return 0;
}

// 从队列中取出packet
int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block)
{
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for (;;)
    {
		if (g_isQuit) //由外界通知退出
		{
			SDL_UnlockMutex(q->mutex);  //解锁退出
			return -1;
		}
        pkt1 = q->first_pkt;
        if (pkt1) {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt) {
                q->last_pkt = NULL;
            }
            q->nb_packets--;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else
        {
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

void packet_queue_flush(PacketQueue *q)
{
    SDL_LockMutex(q->mutex);
    AVPacketList *pkt, *pkt1;
    for(pkt = q->first_pkt; pkt != NULL; pkt = pkt1)
    {
        pkt1 = pkt->next;
        if(pkt1->pkt.data != (uint8_t *)"FLUSH")
        {
            ;
        }
        av_free_packet(&pkt->pkt);
        av_freep(&pkt);

    }
    q->last_pkt = NULL;
    q->first_pkt = NULL;
    q->nb_packets = 0;
    q->size = 0;
    SDL_UnlockMutex(q->mutex);
}

void destroy_queue_context(PacketQueue* q)
{
    SDL_DestroyMutex(q->mutex);
    SDL_DestroyCond(q->cond);
}

//////////////////////////////////////////////


int PlayThread::audio_decode_frame(mediaState* MS, uint8_t* audio_buf)
{
    static AVFrame *pframe=NULL;        //一帧
    static SwrContext*pSwr_ctx=NULL;    //转换
    AVPacket packet;                    //包

    int decodeLen=0;
    int data_size = 0;
    int got_frame = 0;
    int audio_pkt_size=0;
    uint8_t* audio_pkt_data=NULL;

    if(pframe)
       av_frame_free(&pframe);
    pframe=av_frame_alloc();

    while (true)
    {
        if (packet_queue_get(&MS->audioq, &packet, 0) < 0)
        {
            return -1;
        }
        audio_pkt_data =packet.data;
        audio_pkt_size = packet.size;

        if (packet.pts != AV_NOPTS_VALUE)
        {

			//方式一:
			//packet->pts 时间基于  AVStream->time_base units
			//外部时间基于 1/AV_TIME_BASE 即 1微秒
			//使用 av_rescale_q 转换得到 微秒时间
			AVRational aVRational = { 1, AV_TIME_BASE };
			int64_t res = av_rescale_q(packet.pts, pFormatCtx->streams[audioStream]->time_base, aVRational);

			MS->audio_clock = res * 1.0 / 1000000;

			static int64_t lastRes = 0;		//用于记录最后一次的时间
			static int64_t tryTimes = 0;

			if (lastRes != res)				//与上次时间不同时，发送位置改变信号
			{
				emit positionChanged(res);
				lastRes = res;
				tryTimes = 0;
			}
			else
			{
				tryTimes++;
				if (tryTimes >= 100000000LL)
				{
					qDebug() << "no data in list for 1e8 times access";
					AGStatus = AGS_FINISH;
				}
			}

			//方式二：
            //MS->audio_clock = (double)av_q2d(MS->aStream->time_base) * (double)packet.pts;
            //emit  positionChanged(MS->audio_clock * 1000000);

            if(logAudio)
            {
                logAudio = false;
                qDebug() << "to " <<MS->audio_clock ;
            }
        }

        while (audio_pkt_size > 0)
        {
            decodeLen = avcodec_decode_audio4(MS->acct, pframe, &got_frame, &packet);
            if (decodeLen < 0) // 出错，跳过
                break;

            audio_pkt_data += decodeLen;
            audio_pkt_size -= decodeLen;

            if (got_frame)
                data_size = av_samples_get_buffer_size(NULL, MS->acct->channels,pframe->nb_samples, MS->acct->sample_fmt, 0);

            if (pframe->channels > 0 && pframe->channel_layout == 0)
                pframe->channel_layout = av_get_default_channel_layout(pframe->channels);
            else if (pframe->channels == 0 && pframe->channel_layout > 0)
                pframe->channels = av_get_channel_layout_nb_channels(pframe->channel_layout);


            if (pSwr_ctx)
                swr_free(&pSwr_ctx);
            pSwr_ctx = swr_alloc_set_opts(nullptr, MS->wanted_frame->channel_layout,
                                         (AVSampleFormat)MS->wanted_frame->format,
                                         MS->wanted_frame->sample_rate,
                                         pframe->channel_layout,
                                         (AVSampleFormat)pframe->format,
                                         pframe->sample_rate, 0, nullptr);
            swr_init(pSwr_ctx);


            int dst_nb_samples = av_rescale_rnd(swr_get_delay(pSwr_ctx, pframe->sample_rate) + pframe->nb_samples, pframe->sample_rate, pframe->sample_rate, AVRounding(1));
            int len2 = swr_convert(pSwr_ctx, &audio_buf, dst_nb_samples,(const uint8_t**)pframe->data, pframe->nb_samples);
            if (len2 < 0)
                break;

            //int resampled_data_size = len2 * MS->wanted_frame->channels* av_get_bytes_per_sample((AVSampleFormat)MS->wanted_frame->format);
            //int n = 2 * MS->acct->channels;
            //MS->audio_clock += (double)resampled_data_size/(double)(n * MS->acct->sample_rate);
            //emit  positionChanged(MS->audio_clock * 1000000);

            av_free_packet(&packet);
            return MS->wanted_frame->channels * len2 * av_get_bytes_per_sample((AVSampleFormat)MS->wanted_frame->format);
        }
    }
    return -1;
}



//////////////////////////////////////////////



void PlayThread::run()
{
    g_threadMutex.lock();       //保证 SDL 播放设备互斥访问

    ResetToInitAll();           //重置以初始化所有状态

    if(initDeviceAndFfmpegContext())    //尝试初始化播放设备 和 ffmpeg 上下文
    {
        bIsDeviceInit = true;
        emit durationChanged(pFormatCtx->duration);
    }
    else
    {
        //释放所有分配的内存，解锁
        ReleaseAll();
        g_threadMutex.unlock();
        return;
    }

    playDevice();               //初始化后需要启用设备

    generateAudioDataLoop();    //数据生产循环

    //释放所有分配的内存，解锁
    ReleaseAll();
	g_threadMutex.unlock();
}

//尝试初始化播放设备 和 ffmpeg 上下文
bool PlayThread::initDeviceAndFfmpegContext()
{
    char url[1024];
    index = 0;

    //char url[]="xiaoqingge.mp3";              //test
    //char url[]="小情歌.mp3";
    //char url[]="Janice Morning.mp3";

    //url = "A - Something's Going On.mp3";
    //char url[] ="A - Starbucks.mp3";
    //char url[] ="A - The Distance.mp3";
    //char url[] ="Acreix - Visions.mp3";
	//A - Starbucks.wav

    strcpy(url, "A - Starbucks.wav");
    strcpy(url, "xiaoqingge.mp3");
    strcpy(url, "A - Something's Going On.wav");
    strcpy(url, "HOPE-T,接个吻，开一枪 - 锦里.mp3");

    strcpy(url, musicPath.toUtf8());            //播放路径

    //得到 wav 输出的路径
    pcmLen = 0;
    if(outputWav)
    {
        char wavUrl[1024] = {0};
        int i = 0;
        int lastSlash = -1;

        //找到最后一个 \ 或者 /
        for( i = 0; url[i] != '\0' && url[i] != '.'; i++)
            if(url[i] == '\\' || url[i] == '/')
                lastSlash = i;

        for(i= lastSlash+1;url[i] != '\0' && url[i] != '.'; i++)
            wavUrl[i] = url[i];

        wavUrl[i++] = '.';
        wavUrl[i++] = 'w';
        wavUrl[i++] = 'a';
        wavUrl[i++] = 'v';
        wavUrl[i] = '\0';

        pFile=fopen(wavUrl, "wb");
    }


    av_register_all();
    avformat_network_init();
    m_MS.fct = pFormatCtx = avformat_alloc_context();
    //Open
    if(avformat_open_input(&pFormatCtx,url,NULL,NULL)!=0){
        printf("Couldn't open input stream.\n");
        emit errorOccur(-1,"Couldn't open input stream.");
        return false;
    }
    // Retrieve stream information
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        printf("Couldn't find stream information.\n");
        emit errorOccur(-1,"Couldn't find stream information.");
        return false;
    }

    //读取专辑等头文字信息
    //读取metadata中所有的tag
    AVDictionaryEntry *tag = NULL;
//    while ((tag = av_dict_get(pFormatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))){
//       printf("[Tag:%s , Value: %s]\n", tag->key, tag->value);
//    }

    tag = av_dict_get(pFormatCtx->metadata, "album", tag, AV_DICT_MATCH_CASE);
    if(tag)
    {
        album = tag->value;
        emit albumFound(album);
    }
    tag = av_dict_get(pFormatCtx->metadata, "title", tag, AV_DICT_MATCH_CASE);
    if(tag)
    {
         title = tag->value;
        emit titleFound(title);
    }

    tag = av_dict_get(pFormatCtx->metadata, "artist", tag, AV_DICT_MATCH_CASE);
    if(tag)
    {
         artist = tag->value;
        emit artistFound(artist);
    }

	if (strcmp(pFormatCtx->iformat->name, "wav") == 0 || strcmp(pFormatCtx->iformat->name, "mp3") == 0)
	{
		//读取音频的专辑图片
		// read the format headers
		if (pFormatCtx->iformat->read_header(pFormatCtx) < 0) {
			printf("No header format");
			//return;
		}

		//读取专辑图片
		picture = QPixmap();
        for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
			if (pFormatCtx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC) {
				AVPacket pkt = pFormatCtx->streams[i]->attached_pic;
				//使用QImage读取完整图片数据（注意，图片数据是为解析的文件数据，需要用QImage::fromdata来解析读取）
				QImage img = QImage::fromData((uchar*)pkt.data, pkt.size);
				picture = QPixmap::fromImage(img);

				emit pictureFound(picture);

				break;
			}
		}
	}


    // Dump valid information onto standard error
    av_dump_format(pFormatCtx, 0, url, false);

    // Find the first audio stream
    audioStream=-1;
    for(i=0; i < pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
            m_MS.aStream = pFormatCtx->streams[i];
            audioStream=i;
            break;
        }

	videoStream = -1;
	for (i = 0; i < pFormatCtx->nb_streams; i++)
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			m_MS.vStream = pFormatCtx->streams[i];
			videoStream = i;
			break;
		}


    if(audioStream==-1){
        printf("Didn't find a audio stream.\n");
        emit errorOccur(-1,"Didn't find a audio stream.");
        return false;
    }


    // Get a pointer to the codec context for the audio stream
    m_MS.acct =  pCodecCtx=pFormatCtx->streams[audioStream]->codec;

    // Find the decoder for the audio stream
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
        printf("Codec not found.\n");
        emit errorOccur(-1,"Codec not found.");
        return false;
    }

    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
        printf("Could not open codec.\n");
        emit errorOccur(-1,"Could not open codec.");
        return false;
    }


    //Out Audio Param
    out_channel_layout=AV_CH_LAYOUT_STEREO;
    //nb_samples: AAC-1024 MP3-1152
    out_nb_samples=pCodecCtx->frame_size;
    out_sample_fmt=AV_SAMPLE_FMT_S16;
    out_sample_rate=44100;
    out_channels=av_get_channel_layout_nb_channels(out_channel_layout);
    //Out Buffer Size
    out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,out_nb_samples,out_sample_fmt, 1);

    out_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);

//SDL------------------
#if USE_SDL
    //Init
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
        const char* errorString = SDL_GetError();
        printf( "Could not initialize SDL - %s\n", errorString);
        emit errorOccur(-1,QString("Could not initialize SDL - %s.").arg(errorString));
        return false;
    }
    //SDL_AudioSpec
    wanted_spec.freq = out_sample_rate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = out_channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = out_nb_samples;
    wanted_spec.callback = fillAudio;
    wanted_spec.userdata =  &m_MS;

    if (SDL_OpenAudio(&wanted_spec, NULL)<0){
        printf("Can't open audio.\n");
        emit errorOccur(-1,"Can't open audio.");
        return false;
    }
#endif

    //FIX:Some Codec's Context Information is missing
    in_channel_layout=av_get_default_channel_layout(pCodecCtx->channels);
    //Swr

    if(oldPlayWay)
    {
        au_convert_ctx = swr_alloc();
        au_convert_ctx=swr_alloc_set_opts(au_convert_ctx,out_channel_layout, out_sample_fmt, out_sample_rate,
            in_channel_layout,pCodecCtx->sample_fmt , pCodecCtx->sample_rate,0, NULL);
        swr_init(au_convert_ctx);
    }
    else
    {
        m_MS.wanted_frame=av_frame_alloc();
        m_MS.wanted_frame->channel_layout = out_channel_layout;
        m_MS.wanted_frame->format = out_sample_fmt;
        m_MS.wanted_frame->sample_rate = out_sample_rate;
        m_MS.wanted_frame->channels = out_channels;

        m_MS.playThread = this;
    }

    return true;
}



void PlayThread::playDevice()
{
    //Play
    SDL_PauseAudio(0);

    emit audioPlay();
    emit volumeChanged(m_MS.volume);
}

void PlayThread::pauseDevice()
{
    //pause
    SDL_PauseAudio(1);

    emit audioPause();
}

void PlayThread::generateAudioDataLoop()
{
    AGStatus = AGS_PLAYING;
    int nRet = 0;
    AVPacket packet;


    AVPacket *ppacket = nullptr;  //分配用于转换的数据包(输入)
    AVFrame	*pFrame = nullptr;    //分配用于转换的数据包(解码输出)

    if(oldPlayWay)
    {
        ppacket=(AVPacket *)av_malloc(sizeof(AVPacket));
        av_init_packet(ppacket);

        pFrame=av_frame_alloc(); //分配一个 frame
    }


    while(!g_isQuit)
    {
        switch (AGStatus) {
        case AGS_PLAYING:
        {
            if(oldPlayWay)
            {
                nRet = av_read_frame(pFormatCtx, ppacket);

                if(nRet < 0)
                {
                    AGStatus = AGS_FINISH;
                }
                else
                {
                    if(ppacket->stream_index==audioStream)
                    {
                        ret = avcodec_decode_audio4( pCodecCtx, pFrame,&got_picture, ppacket);
                        if ( ret < 0 ) {
                            printf("Error in decoding audio frame.\n");
                            emit errorOccur(-1,"Error in decoding audio frame.");

                            return;
                        }
                        if ( got_picture > 0 ){
                            swr_convert(au_convert_ctx,&out_buffer, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)pFrame->data , pFrame->nb_samples);
            #if 1
                            printf("index:%5d\t pts:%lld\t packet size:%d\n",index,ppacket->pts,ppacket->size);

                            //packet->pts 时间基于  AVStream->time_base units
                            //外部时间基于 1/AV_TIME_BASE 即 1微秒
                            //使用 av_rescale_q 转换得到 微秒时间
                            AVRational aVRational = {1, AV_TIME_BASE};
                            int64_t res = av_rescale_q(ppacket->pts,pFormatCtx->streams[audioStream]->time_base, aVRational);
                            emit positionChanged(res);
            #endif

                            index++;
                        }

            #if USE_SDL
                         if(!outputWav){
                            while(audio_len>0)//Wait until finish
                                SDL_Delay(1);
                         }

                        //Set audio buffer (PCM data)
                        audio_chunk = (Uint8 *) out_buffer;
                        //Audio buffer length
                        audio_len =out_buffer_size;
                        audio_pos = audio_chunk;

                        if(outputWav)
                        {
                            pcmLen += out_buffer_size;
                            //Write PCM
                            fwrite(out_buffer, 1, out_buffer_size, pFile);
                        }


            #endif
                    }
                    av_free_packet(ppacket);    //释放packet内容
                }
            }
            else
            {
                //SDL_Delay(10); 

                int result=av_read_frame(m_MS.fct, &packet);
                if(0==result)
                {
                     if (packet.stream_index == audioStream)
                         packet_queue_put(&m_MS.audioq, &packet);//添加到队列中去
                     else
                         av_free_packet(&packet);
                }
                else
                {
                    int curTime = getCurrentTime()*0.001;
                    int durat = getDuration()*0.000001;
                    if(curTime >= durat)//播放到尾端
                    {
						qDebug() << "ending reached";
						AGStatus = AGS_FINISH;
                        break;
                    }
                }

            }

        }
            break;

        case AGS_SEEK:
        {
            if(oldPlayWay)
            {
                AVRational aVRational = {1, 1000};
                int64_t res = av_rescale_q(millisecondToSeek ,aVRational,pFormatCtx->streams[audioStream]->time_base);

                nRet = av_seek_frame(pFormatCtx, audioStream, res ,AVSEEK_FLAG_BACKWARD);

                if(nRet < 0)
                {
                    printf("Error to seek audio frame.\n");
                    emit errorOccur(-1,"Error to seek audio frame.");
                    return;
                }

                avcodec_flush_buffers(pCodecCtx); //清空解码器的缓存
            }
            else
            {
                AVRational aVRational = {1, 1000};
                int64_t res = av_rescale_q(millisecondToSeek ,aVRational,pFormatCtx->streams[audioStream]->time_base);

                SDL_PauseAudio(1);
               //block here
                if (av_seek_frame(m_MS.fct, audioStream, res, AVSEEK_FLAG_ANY) < 0)
                {
                    //printf("Error to seek audio frame.\n");
                    qDebug()<<"seek error";
                }
                else
                {
                    logAudio = true;
                    qDebug()<<"seek successful  " << "  from " << m_MS.audio_clock << " to :";
                    if (audioStream!=-1) //audio
                    {
						avcodec_flush_buffers(pCodecCtx);

                        packet_queue_flush(&m_MS.audioq); //清除队列
                    }
                }
                SDL_PauseAudio(0);

            }
            AGStatus = AGS_PLAYING;
        }
            break;

        case AGS_FINISH:
			g_isQuit = true;

            break;
        default:
            break;
        }
    }


    if(outputWav)
    {
        insert_wave_header(pFile, pcmLen);
        fclose(pFile);
    }


    if(ppacket)             //释放 packet 本身
        av_free(ppacket);
    if(pFrame)              //释放 frame 本身
       av_frame_free(&pFrame);
}


void PlayThread::clearContextAndCloseDevice()
{
    //释放所有可能分配的上下文内存

    if(oldPlayWay && au_convert_ctx)
        swr_free(&au_convert_ctx);

    av_free(out_buffer);

    pCodec = nullptr;

    if(pCodecCtx)
        avcodec_close(pCodecCtx);

    if(pFormatCtx)
        avformat_close_input(&pFormatCtx);

    if(!oldPlayWay)
        packet_queue_flush(&m_MS.audioq);


    //关闭 SDL 设备
#if USE_SDL

    SDL_CloseAudio();//Close SDL
    SDL_Quit();

#endif

}

//获得设备状态
SDL_AudioStatus PlayThread::GetDeviceStatus()
{
    return SDL_GetAudioStatus();
}

//重置以初始化所有状态
void PlayThread::ResetToInitAll()
{
    g_isQuit= false;                        //退出标志重置

    if (!oldPlayWay)
    {
        m_MS.clear();                       //重设音频相关的上下文状态
        packet_queue_init(&m_MS.audioq);    //初始化队列
    }

    //重置所有指针
    pFormatCtx = nullptr;
    pCodecCtx = nullptr;
    pCodec = nullptr;
    out_buffer = nullptr;
    au_convert_ctx = nullptr;
}

//释放所有可能分配的内存
void PlayThread::ReleaseAll()
{
    emit audioFinish();		 //音频播放结束信号
    emit durationChanged(0); //播放结束，总长重置为 0
    emit positionChanged(0); //播放结束，起始位置也重置为 0

    clearContextAndCloseDevice();           //重置播放器上下文，并关闭设备

    if(!oldPlayWay)
        destroy_queue_context(&m_MS.audioq);

    bIsDeviceInit = false;
}


/* The audio function callback takes the following parameters:
 * stream: A pointer to the audio buffer to be filled
 * len: The length (in bytes) of the audio buffer
*/
void  PlayThread::fillAudio(void *udata,Uint8 *stream,int len){

    if(oldPlayWay)
    {
        //SDL 2.0
        SDL_memset(stream, 0, len);
        if(audio_len==0)
            return;

        len=((Uint32)len>audio_len?audio_len:len);	/*  Mix  as  much  data  as  possible  */


        SDL_MixAudio(stream,audio_pos,len,SDL_MIX_MAXVOLUME);
        audio_pos += len;
        audio_len -= len;
    }
    else
    {
        mediaState* MS = (mediaState*)udata;
        uint8_t audio_buff[MAX_AUDIO_FRAME_SIZE];
        memset(stream, 0, len);
        static int audio_buf_pos=0;
        static int audio_buf_size=0;
        //使用静态变量的原因是audio_buf_size一次数据长度为4608,mp3哈，回调的len又太小，不能让它每执行audio_callback函数就来执行audio_decode_frame解码函数

        while(len>0) //要读len这么长的数据长能退出
        {
            if(audio_buf_pos>=audio_buf_size)//如果完成
            {
                audio_buf_size = MS->playThread->audio_decode_frame(MS, audio_buff);
                if (audio_buf_size < 0)
                    return;//-1 为退出标志
                audio_buf_pos=0;
            }

           int audio_len= audio_buf_size-audio_buf_pos;
           if(audio_len>len)
               audio_len=len;


            SDL_MixAudio(stream, audio_buff+audio_buf_pos, audio_len, MS->volume);
            len-=audio_len;
            audio_buf_pos+=audio_len;
            stream+=audio_len;
         }
    }
}
//-----------------





MusicPlayer::MusicPlayer(QObject* parent):QObject(parent)
{
    playThread = new PlayThread(this);

    connect(playThread, &PlayThread::audioPlay,[=](){emit audioPlay();});
    connect(playThread, &PlayThread::audioPause,[=](){emit audioPause();});
    connect(playThread, &PlayThread::audioFinish,[=](){emit audioFinish();});
    connect(playThread, &PlayThread::volumeChanged,[=](uint8_t volume){
        emit volumeChanged(volume);
    }
    );
    connect(playThread, &PlayThread::durationChanged,[=](qint64 duration){emit durationChanged(duration/1000);});
    connect(playThread, &PlayThread::positionChanged,[=](qint64 position){
       this->m_position = position/1000; });

    connect(playThread, &PlayThread::albumFound,[=](QString album){
        m_album=album; emit albumFound(album);}
    );
    connect(playThread, &PlayThread::artistFound,[=](QString artist){m_artist=artist; emit artistFound(artist);});
    connect(playThread, &PlayThread::titleFound,[=](QString title){m_title=title; emit titleFound(title);});
    connect(playThread, &PlayThread::pictureFound,[=](QPixmap picture){m_picture=picture; emit pictureFound(picture);});

    m_interval = 1;
    m_positionUpdateTimer.setInterval(m_interval);
    connect(&m_positionUpdateTimer,SIGNAL(timeout()),this, SLOT(sendPosChangedSignal() ));

    m_position = 0;
}

MusicPlayer::~MusicPlayer() {
    playThread->AGStatus = AGS_FINISH; //置结束位，并等待线程退出才结束，否则 playThread 的释放会导致访问异常
    while (playThread->isRunning())
        _millisecondSleep(5);
}


void MusicPlayer::setMusicPath(QString path)
{
    playThread->musicPath = musicPath = path;

}

QString MusicPlayer::getMusicPath()
{
    return musicPath;
}

//音乐文件信息
QString MusicPlayer::getTitle()
{
    return m_title;
}

QString MusicPlayer::getArtist()
{
    return m_artist;
}

QString MusicPlayer::getAlbum()
{
    return m_album;
}

QPixmap MusicPlayer::getPicture()
{
    return m_picture;
}

//stop() 并 play();
void MusicPlayer::reload()
{
    stop();

    while(playThread->isRunning())
        _millisecondSleep(10); //等待结束

    play();

    if(!m_positionUpdateTimer.isActive())
        m_positionUpdateTimer.start();
}


//播放控制
void MusicPlayer::play()
{
	if (!playThread->bIsDeviceInit)
	{
		playThread->start();
	}
    else
        playThread->playDevice();

    if(!m_positionUpdateTimer.isActive())
        m_positionUpdateTimer.start();
}

void MusicPlayer::pause()
{
    if(state() == PlayingState)
        playThread->pauseDevice();

}

void MusicPlayer::stop()
{
	playThread->AGStatus = AGS_FINISH;

	if (oldPlayWay)
	{
		if (state() == PausedState)          //要停止设备，如果正在暂停，为了使得 generateAudioDataLoop 继续执行以按流程退出，启动设备
			playThread->playDevice();
	}

    if(m_positionUpdateTimer.isActive())
    {
        m_positionUpdateTimer.stop();
        emit positionChanged(0);       //停止了timer ，自己发送0时间
    }
}

//跳到时间点播放（单位 毫秒）
void MusicPlayer::seek(quint64 pos)
{
    //先获得总长
    quint64 total = duration();
    if(pos > total)
        pos = total;

    playThread->millisecondToSeek = pos;
    playThread->AGStatus= AGS_SEEK;
}

//往后跳（单位 毫秒）
void MusicPlayer::forwordSeek(quint64 step)
{
    seek(m_position + step);
}

//往回跳（单位 毫秒）
void MusicPlayer::backwardSeek(quint64 step)
{
    seek(step > m_position ? 0: m_position - step);
}

 //音量大小范围 0-128
void MusicPlayer::setVolume(int volume)
{
    if(volume > 128)
        volume = 128;

    if(volume < 0)
        volume = 0;

    playThread->m_MS.volume =(uint8_t)volume;
    emit volumeChanged(volume);
}

int MusicPlayer::getVolume()
{
    return (int)playThread->m_MS.volume;
}

//获得当总时长（单位 毫秒）
quint64 MusicPlayer::duration()
{
    if(playThread && (playThread->pFormatCtx == nullptr || !playThread->bIsDeviceInit))
        return 0;
    else
        return playThread->pFormatCtx->duration/1000;
}

//获得当总位置（单位 毫秒）
quint64 MusicPlayer::position()
{
    return m_position;
}

//设置通知间隔（歌曲位置进度）
void MusicPlayer::setNotifyInterval(int msec)
{
    m_interval = msec;
    m_positionUpdateTimer.setInterval(m_interval);
}

void MusicPlayer::sendPosChangedSignal()
{
    emit positionChanged(m_position);
}


MusicPlayer::Status MusicPlayer::state()
{
    SDL_AudioStatus deviceStatus = playThread->GetDeviceStatus();
    switch (deviceStatus) {
    case SDL_AUDIO_PLAYING:
        return PlayingState;
        break;
    case SDL_AUDIO_PAUSED:
        return PausedState;
        break;
    case SDL_AUDIO_STOPPED:
    default:
        return StoppedState;
        break;
    }
}

