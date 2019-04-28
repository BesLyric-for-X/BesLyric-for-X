#include "SubPageDownloadSong.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "BesScaleUtil.h"

SubPageDownloadSong::SubPageDownloadSong(QWidget *parent)
    : QWidget(parent), nCurrentDownloadCount(0)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释
    initLayout();
    initConnection();
}

SubPageDownloadSong::~SubPageDownloadSong()
{

}

void SubPageDownloadSong::initLayout()
{
    QVBoxLayout* vMainLayour= new QVBoxLayout(this);

    //标题
    labelTitleSearchSong = new QLabel(this);
    labelTitleSearchSong->setObjectName("labelTitleSearchSong");
    labelTitleSearchSong->setText(tr("歌曲搜索"));

    //搜索按钮
    labelSearchNcmSong = new QLabel(this);
    labelSearchNcmArtist = new QLabel(this);
    labelSearchNcmSong->setMinimumSize(120*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    labelSearchNcmArtist->setMinimumSize(120*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    labelSearchNcmSong->setMaximumSize(150*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    labelSearchNcmArtist->setMaximumSize(150*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    labelSearchNcmSong->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelSearchNcmArtist->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelSearchNcmSong->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSearchNcmArtist->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSearchNcmSong->setText(tr("歌曲名："));
    labelSearchNcmArtist->setText(tr("歌手："));

    editSearchNcmSong = new QLineEdit(this);
    editSearchNcmArtist = new QLineEdit(this);
    editSearchNcmSong->setPlaceholderText(tr("必填"));
    editSearchNcmArtist->setPlaceholderText(tr("选填"));
    editSearchNcmSong->setMinimumSize(200*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    editSearchNcmArtist->setMinimumSize(200*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    editSearchNcmSong->setMaximumSize(300*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    editSearchNcmArtist->setMaximumSize(300*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    editSearchNcmSong->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editSearchNcmArtist->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    btnSearchNcmSong  = new BesButton(this);
    btnSearchNcmSong->setObjectName("btnSearchNcmSong");
    btnSearchNcmSong->setText(tr("搜索"));
    btnSearchNcmSong->setMinimumSize(150*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    btnSearchNcmSong->setMaximumSize(150*BesScaleUtil::scale(),30*BesScaleUtil::mscale());
    btnSearchNcmSong->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout1->addWidget(labelSearchNcmSong);
    hLayout1->addWidget(editSearchNcmSong);
    hLayout1->addWidget(btnSearchNcmSong);
    hLayout1->addSpacerItem(new QSpacerItem(20*BesScaleUtil::scale(),20,QSizePolicy::MinimumExpanding,  QSizePolicy::Fixed));
    hLayout2->addWidget(labelSearchNcmArtist);
    hLayout2->addWidget(editSearchNcmArtist);
    hLayout2->addSpacerItem(new QSpacerItem(20*BesScaleUtil::scale(),20,QSizePolicy::MinimumExpanding,  QSizePolicy::Fixed));

    //搜索结果提示
    labelNcmSongResultTip1= new QLabel(this);
    labelNcmSongResultTip2= new QLabel(this);
    labelNcmSongResultTip3= new QLabel(this);
    labelNcmSongResultTip4= new QLabel(this);
    labelNcmSongResultTip5= new QLabel(this);
    labelNcmSongResultTip1->setMinimumHeight(30*BesScaleUtil::mscale());
    labelNcmSongResultTip2->setMinimumHeight(30*BesScaleUtil::mscale());
    labelNcmSongResultTip3->setMinimumHeight(30*BesScaleUtil::mscale());
    labelNcmSongResultTip4->setMinimumHeight(30*BesScaleUtil::mscale());
    labelNcmSongResultTip5->setMinimumHeight(30*BesScaleUtil::mscale());
    labelNcmSongResultTip1->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelNcmSongResultTip2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelNcmSongResultTip3->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelNcmSongResultTip4->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelNcmSongResultTip5->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelNcmSongResultTip2->setObjectName("labelNcmSongResultTip2");
    labelNcmSongResultTip4->setObjectName("labelNcmSongResultTip4");
    labelNcmSongResultTip1->setText(tr("搜索歌曲名"));
    labelNcmSongResultTip2->setText(tr("\"我们的歌\""));
    labelNcmSongResultTip3->setText(tr("、歌手"));
    labelNcmSongResultTip4->setText(tr("\"王力宏\""));
    labelNcmSongResultTip5->setText(tr("找到15条歌曲记录。"));
    showTipLabel(false);
    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout3->addSpacerItem(new QSpacerItem(20*BesScaleUtil::scale(),20,QSizePolicy::Fixed,  QSizePolicy::Fixed));
    hLayout3->addWidget(labelNcmSongResultTip1);
    hLayout3->addWidget(labelNcmSongResultTip2);
    hLayout3->addWidget(labelNcmSongResultTip3);
    hLayout3->addWidget(labelNcmSongResultTip4);
    hLayout3->addWidget(labelNcmSongResultTip5);
    hLayout3->addSpacerItem(new QSpacerItem(20*BesScaleUtil::scale(),20,QSizePolicy::MinimumExpanding,  QSizePolicy::Fixed));

    //搜索结果
    tableNcmSongSearch = new BesNcmSongTableView(this);
    tableNcmSongSearch->setObjectName("tableNcmSongSearch");


    //tab 搜索歌词结果页面
    tabpageNcmSongResult = new QTabWidget(this);
    tabpageNcmSongResult->setObjectName("tabpageNcmSongResult");
    tabpageNcmSongResult->setFocusPolicy(Qt::NoFocus);
    tabpageNcmSongResult->addTab(tableNcmSongSearch,"歌曲结果");
    tabpageNcmSongResult->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    vMainLayour->addWidget(labelTitleSearchSong);
    vMainLayour->addSpacerItem(new QSpacerItem(20,10*BesScaleUtil::scale(),QSizePolicy::Fixed,  QSizePolicy::Fixed));
    vMainLayour->addLayout(hLayout1);
    vMainLayour->addLayout(hLayout2);
    vMainLayour->addSpacerItem(new QSpacerItem(20,20*BesScaleUtil::scale(),QSizePolicy::Fixed,  QSizePolicy::Fixed));
    vMainLayour->addLayout(hLayout3);
    vMainLayour->addSpacerItem(new QSpacerItem(20,20*BesScaleUtil::scale(),QSizePolicy::Fixed,  QSizePolicy::Fixed));
    vMainLayour->addWidget(tabpageNcmSongResult);
}

void SubPageDownloadSong::initConnection()
{
     connect(&searchThread, SIGNAL(songResultChanged(LyricSearchResult)), this,SLOT(OnSongResultChanged(LyricSearchResult)));
     connect(btnSearchNcmSong, SIGNAL(clicked(bool)),this,SLOT(OnSearchSong()) );

     connect(tableNcmSongSearch,SIGNAL(sig_oneDownloadStarted()),this,SLOT(onStartOneDownload()));
     connect(tableNcmSongSearch,SIGNAL(sig_oneDownloadFinished()),this,SLOT(onFinishOneDownload()));
}

void SubPageDownloadSong::searchNcmDirectly(const QString &artists, const QString &song)
{
    editSearchNcmSong->setText(song);
    editSearchNcmArtist->setText(artists);

    if(btnSearchNcmSong->isEnabled() == false)
    {
        //正在下载中，提示稍后再搜索
        BesMessageBox::information(tr("提示"),tr("有歌曲正在下载中，请稍后搜索"));
        return;
    }

    OnSearchSong();
}


void SubPageDownloadSong::OnSearchSong()
{
    QString song = editSearchNcmSong->text().trimmed();
    QString artist = editSearchNcmArtist->text().trimmed();

    if(song.isEmpty())
    {
        BesMessageBox::information(tr("提示"),tr("歌曲名不能为空"));
        return;
    }

    strLastSongName = song;
    strLastArtistName = artist;

    showTipLabel(true);

    labelNcmSongResultTip2->setText("“" + song + "”");
    labelNcmSongResultTip4->setText("“" + artist + "”");
    labelNcmSongResultTip5->setText(tr("正在搜索中..."));
    btnSearchNcmSong->setEnabled(false);

    searchThread.StartSearchNcm(artist, song);
}

void SubPageDownloadSong::OnSongResultChanged(LyricSearchResult result)
{
    //显示搜索结果
    int nCount = result.vecSongInfo.size();
    QString strResultTip = result.strUnexpectedResultTip;
    labelNcmSongResultTip5->setText(QString().sprintf("，找到%d条歌曲记录。",nCount)+strResultTip);
    labelNcmSongResultTip5->setVisible(true);

    if(result.vecSongInfo.size()!=0)
        tableNcmSongSearch->setItems(result.vecSongInfo);

    if(result.bCurrentSearchDone) //搜索结束时才能恢复按钮
    {
        //恢复按钮，可以下一次继续搜索
        btnSearchNcmSong->setEnabled(true);
    }
}


void SubPageDownloadSong::onStartOneDownload()
{
    nCurrentDownloadCount++;
    btnSearchNcmSong->setEnabled(false);
}

void SubPageDownloadSong::onFinishOneDownload()
{
    nCurrentDownloadCount--;
    if(nCurrentDownloadCount == 0)
        btnSearchNcmSong->setEnabled(true);
}


void SubPageDownloadSong::showTipLabel(bool bShow)
{
    labelNcmSongResultTip1->setVisible(bShow);
    labelNcmSongResultTip2->setVisible(bShow);
    labelNcmSongResultTip3->setVisible(bShow);
    labelNcmSongResultTip4->setVisible(bShow);
    labelNcmSongResultTip5->setVisible(bShow);
}
