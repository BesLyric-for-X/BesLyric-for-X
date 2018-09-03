#include "global.h"
#include "SubPageDownloadLyric.h"
#include "BesMessageBox.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStringList>
#include <QHeaderView>
#include <QStandardItemModel>

SubPageDownloadLyric::SubPageDownloadLyric(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);
    initEntity();
    initLayout();
    initConnection();
}

SubPageDownloadLyric::~SubPageDownloadLyric()
{

}

void SubPageDownloadLyric::initEntity()
{
    qRegisterMetaType<LyricSearchResult>("LyricSearchResult");
}

void SubPageDownloadLyric::initLayout()
{
    QVBoxLayout* vMainLayour= new QVBoxLayout(this);

    //标题
    labelTitleSearchLyric = new QLabel(this);
    labelTitleSearchLyric->setObjectName("labelTitleSearchLyric");
    labelTitleSearchLyric->setText(tr("歌词搜索"));

    //搜索按钮
    labelSearchLyricSong = new QLabel(this);
    labelSearchLyricArtist = new QLabel(this);
    labelSearchLyricSong->setMinimumSize(150,30);
    labelSearchLyricArtist->setMinimumSize(150,30);
    labelSearchLyricSong->setMaximumSize(150,30);
    labelSearchLyricArtist->setMaximumSize(150,30);
    labelSearchLyricSong->setAlignment(Qt::AlignRight);
    labelSearchLyricArtist->setAlignment(Qt::AlignRight);
    labelSearchLyricSong->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSearchLyricArtist->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSearchLyricSong->setText(tr("歌曲名"));
    labelSearchLyricArtist->setText(tr("歌手"));

    editSearchLyricSong = new QLineEdit(this);
    editSearchLyricArtist = new QLineEdit(this);
    editSearchLyricSong->setPlaceholderText(tr("必填"));
    editSearchLyricArtist->setPlaceholderText(tr("选填"));
    editSearchLyricSong->setMinimumSize(200,30);
    editSearchLyricArtist->setMinimumSize(200,30);
    editSearchLyricSong->setMaximumSize(300,30);
    editSearchLyricArtist->setMaximumSize(300,30);
    editSearchLyricSong->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editSearchLyricArtist->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    btnSearchInProgram  = new BesButton(this);
    btnSearchByBaidu = new BesButton(this);
    btnSearchInProgram->setObjectName("btnSearchInProgram");
    btnSearchByBaidu->setObjectName("btnSearchByBaidu");
    btnSearchInProgram->setText(tr("直接搜索"));
    btnSearchByBaidu->setText(tr("百度搜索"));
    btnSearchByBaidu->setMinimumSize(150,30);
    btnSearchInProgram->setMinimumSize(150,30);
    btnSearchByBaidu->setMaximumSize(150,30);
    btnSearchInProgram->setMaximumSize(150,30);
    btnSearchByBaidu->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btnSearchInProgram->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QHBoxLayout* hLayout1 = new QHBoxLayout();
    QHBoxLayout* hLayout2 = new QHBoxLayout();
    hLayout1->addWidget(labelSearchLyricSong);
    hLayout1->addWidget(editSearchLyricSong);
    hLayout1->addWidget(btnSearchInProgram);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding,  QSizePolicy::Fixed));
    hLayout2->addWidget(labelSearchLyricArtist);
    hLayout2->addWidget(editSearchLyricArtist);
    hLayout2->addWidget(btnSearchByBaidu);
    hLayout2->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding,  QSizePolicy::Fixed));

    //搜索结果提示
    labelLyricResultTip1= new QLabel(this);
    labelLyricResultTip2= new QLabel(this);
    labelLyricResultTip3= new QLabel(this);
    labelLyricResultTip4= new QLabel(this);
    labelLyricResultTip5= new QLabel(this);
    labelLyricResultTip1->setMinimumHeight(30);
    labelLyricResultTip2->setMinimumHeight(30);
    labelLyricResultTip3->setMinimumHeight(30);
    labelLyricResultTip4->setMinimumHeight(30);
    labelLyricResultTip5->setMinimumHeight(30);
    labelLyricResultTip1->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelLyricResultTip2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelLyricResultTip3->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelLyricResultTip4->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelLyricResultTip5->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed );
    labelLyricResultTip2->setObjectName("labelLyricResultTip2");
    labelLyricResultTip4->setObjectName("labelLyricResultTip4");
    labelLyricResultTip1->setText(tr("搜索歌曲名"));
    labelLyricResultTip2->setText(tr("\"我们的歌\""));
    labelLyricResultTip3->setText(tr("、歌手"));
    labelLyricResultTip4->setText(tr("\"王力宏\""));
    labelLyricResultTip5->setText(tr("找到15个歌词文件。"));
    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout3->addWidget(labelLyricResultTip1);
    hLayout3->addWidget(labelLyricResultTip2);
    hLayout3->addWidget(labelLyricResultTip3);
    hLayout3->addWidget(labelLyricResultTip4);
    hLayout3->addWidget(labelLyricResultTip5);
    hLayout3->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding,  QSizePolicy::Fixed));

    //搜索结果
    tableLyricSearch = new BesTableView(this);

    //原歌词面板
    widgetRawLyricBoard = new QWidget(this);
    //LRC歌词面板
    widgetLrcLyricBoard = new QWidget(this);

    //tab 搜索歌词结果页面
    tabpageLyricResult = new QTabWidget(this);
    tabpageLyricResult->setObjectName("tabpageLyricResult");
    tabpageLyricResult->addTab(tableLyricSearch,"歌词列表");
    tabpageLyricResult->addTab(widgetRawLyricBoard,"原歌词");
    tabpageLyricResult->addTab(widgetLrcLyricBoard,"LRC歌词");
    tabpageLyricResult->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    vMainLayour->addWidget(labelTitleSearchLyric);
    vMainLayour->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed,  QSizePolicy::Fixed));
    vMainLayour->addLayout(hLayout1);
    vMainLayour->addLayout(hLayout2);
    vMainLayour->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed,  QSizePolicy::Fixed));
    vMainLayour->addLayout(hLayout3);
    vMainLayour->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed,  QSizePolicy::Fixed));
    vMainLayour->addWidget(tabpageLyricResult);

}

void SubPageDownloadLyric::initConnection()
{
    connect(&searchThread, SIGNAL(lyricResultChanged(LyricSearchResult)), this,SLOT(OnLyricResultChanged(LyricSearchResult)));
    connect(btnSearchInProgram, SIGNAL(clicked(bool)),this,SLOT(OnSearchInProgram()) );
}

void SubPageDownloadLyric::OnSearchInProgram()
{
    QString song = editSearchLyricSong->text().trimmed();
    QString artist = editSearchLyricArtist->text().trimmed();

    if(song.isEmpty())
    {
        BesMessageBox::information(tr("提示"),tr("歌曲名不能为空"));
        return;
    }

    labelLyricResultTip2->setText("“" + song + "”");
    labelLyricResultTip4->setText("“" + artist + "”");
    labelLyricResultTip5->setText(tr("正在搜索中..."));
    btnSearchInProgram->setEnabled(false);

    searchThread.StartSearchLyric(artist, song);
}


void SubPageDownloadLyric::OnLyricResultChanged(LyricSearchResult result)
{
    //删除列表中所有的已有数据
    static int nLyricCount = 0;
    if(!result.bAppendToList)
    {
        nLyricCount = 0;
        //m_lrcListAdapter->DeleteAllItem();
    }

    QString strResultTip;
    if(result.bShowUnexpectedResultTip)
        strResultTip = result.strUnexpectedResultTip;

    //显示搜索结果
    nLyricCount += result.vecLyricInfoTotal.size();
    labelLyricResultTip5->setText(QString().sprintf("，找到%d个歌词文件。",nLyricCount)+strResultTip);
    labelLyricResultTip5->setVisible(true);

    for(auto iter = result.vecLyricInfoTotal.begin(); iter != result.vecLyricInfoTotal.end(); iter++)
    {
        //m_lrcListAdapter->AddItem( iter->strSong, iter->strArtist, iter->strLyricFrom,iter->strPlaneText, iter->strLabelText);
        //m_lrcListAdapter->notifyDataSetChanged();
    }

    if(result.bCurrentSearchDone) //搜索结束时才能恢复按钮
    {
        //恢复按钮，可以下一次继续搜索
        btnSearchInProgram->setEnabled(true);

        //m_txtIsSearchingLyricTip->SetVisible(FALSE,TRUE); //隐藏正在搜索提示
    }
}


