#include "PagePreviewLyric.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ImageFilter.h"


void ThreadCalcBackgroundImage::run()
{
    QPixmap newPixmap = ImageFilter::BlurImage(this->pixmapToDeal, 100, 80);
    emit(ready(newPixmap));
}

PagePreviewLyric::PagePreviewLyric(QWidget *parent):
QWidget(parent)
{
    this->setMouseTracking(true);

    initLayout();
    connectAll();

    //初始化图片
    useBlackMask = true;
    blurbackgroudImage = QPixmap(":/resource/image/default_preview_background.png");
    whiteMaskImage = QPixmap(":/resource/image/album_background_white_mask.png");
    blackMaskImage = QPixmap(":/resource/image/album_background_black_mask.png");
}

PagePreviewLyric::~PagePreviewLyric()
{

}

void PagePreviewLyric::initLayout()
{
    widgetMainPreview = new QWidget(this);
    widgetMainPreview->setObjectName("widgetMainPreview");
    widgetMainPreview->setMouseTracking(true);


    QHBoxLayout* hLayout = new QHBoxLayout(widgetMainPreview);
    hLayout->setMargin(0);
    phonagraph = new Phonograph(widgetMainPreview);
    phonagraph->setMinimumSize(500,600);
    phonagraph->setMaximumSize(500,600);
    phonagraph->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    lyricViewer = new LyricViewer(widgetMainPreview);
    lyricViewer->setMinimumSize(500,600);
    lyricViewer->setMaximumSize(500,600);
    lyricViewer->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QVBoxLayout * rightVlayout = new QVBoxLayout();
    rightVlayout->setMargin(20);
    btnPackupLyricBox = new BesButton(widgetMainPreview);
    btnPackupLyricBox->setObjectName("btnPackupLyricBox");
    btnPackupLyricBox->setMaximumSize(60,60);
    btnPackupLyricBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightVlayout->addWidget(btnPackupLyricBox);
    rightVlayout->addSpacerItem(new QSpacerItem(50,50,QSizePolicy::Fixed,QSizePolicy::MinimumExpanding));

    hLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    hLayout->addWidget(phonagraph);
    hLayout->addWidget(lyricViewer);
    hLayout->addLayout(rightVlayout);
    hLayout->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));


    QVBoxLayout* layoutMain = new QVBoxLayout(this);
    layoutMain->setMargin(0);
    layoutMain->addWidget(widgetMainPreview);
    layoutMain->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed,QSizePolicy::MinimumExpanding));
}

void PagePreviewLyric::connectAll()
{

}

void PagePreviewLyric::calcNewBackgroundImage(QPixmap pixmap)
{
    ThreadCalcBackgroundImage* thread = new ThreadCalcBackgroundImage(pixmap, this);

    connect(thread,SIGNAL(ready(QPixmap)),this,SLOT(setNewBackgroundPixmap(QPixmap)));

    thread->start();
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

void PagePreviewLyric::setNewBackgroundPixmap(QPixmap newPixmap)
{
    blurbackgroudImage = newPixmap;
    update();
}


void PagePreviewLyric::mousePressEvent(QMouseEvent* event)
{
    static bool flag = true;
     if(phonagraph->isPlaying())
         phonagraph->stop();
     else
     {
         if(flag)
         {
            phonagraph->setAlbumCover(QPixmap(":/resource/image/AlbumCover2.jpg"));
            calcNewBackgroundImage(QPixmap(":/resource/image/AlbumCover2.jpg"));

         }else
         {
             phonagraph->setAlbumCover(QPixmap(":/resource/image/AlbumCover1.jpg"));
             calcNewBackgroundImage(QPixmap(":/resource/image/AlbumCover1.jpg"));
         }
         flag = !flag;

         phonagraph->play();
     }

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

