#include "PagePreviewLyric.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ImageFilter.h"
#include "BesScaleUtil.h"

//参考：https://blog.csdn.net/caoshangpa/article/details/62421334

ThreadCalcBackgroundImage::~ThreadCalcBackgroundImage()
{
    // 请求终止
    requestInterruption();
    quit();
    wait();
}

void ThreadCalcBackgroundImage::run()
{
    // 是否请求终止
    while (!isInterruptionRequested())
    {
        bool bPicFound = false;
        QPixmap pixmapToDeal;

        {
            QMutexLocker locker(&m_mutex);
            if(!vecPic.empty())
            {
                bPicFound = true;
                pixmapToDeal = vecPic.back();
                vecPic.clear();
            }
        }
        // locker超出范围并释放互斥锁

        if(bPicFound)
        {
            QPixmap newPixmap = ImageFilter::BlurImage(pixmapToDeal, 50, 80);

            bPicFound = false;
            {
                QMutexLocker locker(&m_mutex);
                if(vecPic.empty())      //在没有新图片需要计算时才发出图片,保证发出的总是最后一次计算
                    emit(ready(newPixmap));
            }
            // locker超出范围并释放互斥锁
        }
        else
            msleep(2000);
    }
}

void ThreadCalcBackgroundImage::showPic(QPixmap pic)
{
    QMutexLocker locker(&m_mutex);
    vecPic.push_back(pic);
}

PagePreviewLyric::PagePreviewLyric(QWidget *parent):
QWidget(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    initLayout();
    initEntity();
    initConnection();
    finishInit();
}

PagePreviewLyric::~PagePreviewLyric()
{

}

void PagePreviewLyric::initLayout()
{
    widgetMainPreview = new QWidget(this);
    widgetMainPreview->setObjectName("widgetMainPreview");
    widgetMainPreview->setMouseTracking(true);//详见 BesFramelessWidget.h 注释


    QHBoxLayout* hLayout = new QHBoxLayout(widgetMainPreview);
    hLayout->setMargin(0);
    phonagraph = new Phonograph(widgetMainPreview);
    phonagraph->setMinimumSize(480 * BesScaleUtil::scale(),650* BesScaleUtil::scale());
    phonagraph->setMaximumSize(480 * BesScaleUtil::scale(),650* BesScaleUtil::scale());
    phonagraph->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    lyricViewer = new LyricViewer(widgetMainPreview);
    lyricViewer->setMinimumSize(550* BesScaleUtil::scale(),650* BesScaleUtil::scale());
    lyricViewer->setMaximumSize(550* BesScaleUtil::scale(),650* BesScaleUtil::scale());
    lyricViewer->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QVBoxLayout * rightVlayout = new QVBoxLayout();
    rightVlayout->setMargin(20* BesScaleUtil::scale());
    btnPackupLyricBox = new BesButton(widgetMainPreview);
    btnPackupLyricBox->setObjectName("btnPackupLyricBox");
    btnPackupLyricBox->setMaximumSize(40* BesScaleUtil::scale(),30* BesScaleUtil::scale());
    btnPackupLyricBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightVlayout->addWidget(btnPackupLyricBox);
    rightVlayout->addSpacerItem(new QSpacerItem(50* BesScaleUtil::scale(),50* BesScaleUtil::scale(),QSizePolicy::Fixed,QSizePolicy::MinimumExpanding));

    hLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20* BesScaleUtil::scale(),QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout->addWidget(phonagraph);
    hLayout->addWidget(lyricViewer);
    hLayout->addLayout(rightVlayout);
    hLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20* BesScaleUtil::scale(),QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));


    QVBoxLayout* layoutMain = new QVBoxLayout(this);
    layoutMain->setMargin(0);
    layoutMain->addWidget(widgetMainPreview);
    layoutMain->addSpacerItem(new QSpacerItem(20* BesScaleUtil::scale(),20* BesScaleUtil::scale(),QSizePolicy::Fixed,QSizePolicy::MinimumExpanding));
}

void PagePreviewLyric::initEntity()
{
    calPicThread = new ThreadCalcBackgroundImage(this);

    //初始化图片
    useBlackMask = true;
    blurbackgroudImage = QPixmap(":/resource/image/default_preview_background.png");
    whiteMaskImage = QPixmap(":/resource/image/album_background_white_mask.png");
    blackMaskImage = QPixmap(":/resource/image/album_background_black_mask.png");

}

void PagePreviewLyric::initConnection()
{
    connect(calPicThread,SIGNAL(ready(QPixmap)),this,SLOT(setNewBackgroundPixmap(QPixmap)));
}

void PagePreviewLyric::finishInit()
{
    calPicThread->start(QThread::Priority::HighPriority);
}

void PagePreviewLyric::calcNewBackgroundImage(QPixmap pixmap)
{
    calPicThread->showPic(pixmap);
}

//设置是否使用黑色mask图层
void PagePreviewLyric::setWheterToUseBlackMask(bool useBlack)
{
    if(useBlackMask != useBlack)
    {
        useBlackMask = useBlack;
        update();
    }
}

void PagePreviewLyric::playPhonagraph()
{
    phonagraph->play();
}

void PagePreviewLyric::stopPhonagraph()
{
    phonagraph->stop();
}

void PagePreviewLyric::AlbumImageChanged(QPixmap newPixmap)
{
    phonagraph->setAlbumCover(QPixmap(newPixmap));
    calcNewBackgroundImage(QPixmap(newPixmap));
}

void PagePreviewLyric::setToDefaultAlbumImage()
{
    //不要每次都计算了，直接使用预定义的图片
    //AlbumImageChanged(QPixmap(":/resource/image/AlbumCover1.jpg"));

    phonagraph->setAlbumCover(QPixmap(":/resource/image/AlbumCover1.jpg"));
    setNewBackgroundPixmap(QPixmap(":/resource/image/default_preview_background.png"));
}


void PagePreviewLyric::setNewBackgroundPixmap(QPixmap newPixmap)
{
    blurbackgroudImage = newPixmap;
    update();
}


void PagePreviewLyric::mousePressEvent(QMouseEvent* event)
{
     QWidget::mousePressEvent(event);
}

void PagePreviewLyric::paintEvent(QPaintEvent* event)
{
    QPixmap maskLayer;
    if(useBlackMask)
        maskLayer = blackMaskImage;
    else
        maskLayer = whiteMaskImage;

    QPainter painter(this);

    QRect outerRect(0,0,this->width()-1,this->height()-1);
    painter.fillRect(outerRect, QBrush("#ffffff"));                   //绘制背景颜色
    QWidget::paintEvent(event);

    QSize windowSize = this->size();

    QSize halfSize = blurbackgroudImage.size();
    halfSize.setHeight(halfSize.height()*2.5/4);
    QPixmap half =  blurbackgroudImage.scaled(halfSize);  //高度先压缩为一半

    painter.drawPixmap(0,0,half.scaled(windowSize,Qt::KeepAspectRatioByExpanding));
    painter.drawPixmap(0,0,maskLayer.scaled(windowSize));

}

