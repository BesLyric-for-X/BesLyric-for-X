#include "global.h"
#include "SubPageDownloadLyric.h"
#include "BesMessageBox.h"
#include "SettingManager.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QStringList>
#include <QHeaderView>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItemModel>

SubPageDownloadLyric::SubPageDownloadLyric(QWidget *parent)
    : QWidget(parent)
{
    this->setMouseTracking(true);

    initLayout();
    initConnection();
}

SubPageDownloadLyric::~SubPageDownloadLyric()
{
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
    labelSearchLyricSong->setMinimumSize(120,30);
    labelSearchLyricArtist->setMinimumSize(120,30);
    labelSearchLyricSong->setMaximumSize(150,30);
    labelSearchLyricArtist->setMaximumSize(150,30);
    labelSearchLyricSong->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelSearchLyricArtist->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelSearchLyricSong->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSearchLyricArtist->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelSearchLyricSong->setText(tr("歌曲名："));
    labelSearchLyricArtist->setText(tr("歌手："));

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
    btnSearchInProgram->setFocusPolicy(Qt::NoFocus);
    btnSearchByBaidu->setFocusPolicy(Qt::NoFocus);
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
    showTipLabel(false);
    QHBoxLayout* hLayout3 = new QHBoxLayout();
    hLayout3->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Fixed,  QSizePolicy::Fixed));
    hLayout3->addWidget(labelLyricResultTip1);
    hLayout3->addWidget(labelLyricResultTip2);
    hLayout3->addWidget(labelLyricResultTip3);
    hLayout3->addWidget(labelLyricResultTip4);
    hLayout3->addWidget(labelLyricResultTip5);
    hLayout3->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::MinimumExpanding,  QSizePolicy::Fixed));

    //搜索结果
    tableLyricSearch = new BesLrcTableView(this);
    tableLyricSearch->setObjectName("tableLyricSearch");

    //原歌词面板
    widgetRawLyricBoard = new QWidget(this);

    labelRawLyricPanelSavePath = new QLabel(widgetRawLyricBoard);
    labelRawLyricPanelSong = new QLabel(widgetRawLyricBoard);
    labelRawLyricPanelArtist = new QLabel(widgetRawLyricBoard);
    editRawLyricPanelSavePath= new BesFileLineEdit(BesFileTypeFloder,widgetRawLyricBoard);
    editRawLyricPanelSong= new QLineEdit(widgetRawLyricBoard);
    editRawLyricPanelArtist= new QLineEdit(widgetRawLyricBoard);
    btnRawLyricPanelSelect = new BesButton(widgetRawLyricBoard);
    btnRawLyricPanelSave = new BesButton(widgetRawLyricBoard);
    editTextRawLyric = new QTextEdit(widgetRawLyricBoard);

    labelRawLyricPanelSavePath->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelRawLyricPanelSong->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelRawLyricPanelArtist->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelRawLyricPanelSavePath->setText(tr("保存路径："));
    labelRawLyricPanelSong->setText(tr("音乐："));
    labelRawLyricPanelArtist->setText(tr("歌手："));
    labelRawLyricPanelSavePath->setMinimumSize(100,30);
    labelRawLyricPanelSong->setMinimumSize(100,30);
    labelRawLyricPanelArtist->setMinimumSize(100,30);

    labelRawLyricPanelSavePath->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelRawLyricPanelArtist->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelRawLyricPanelSong->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);

    editRawLyricPanelSavePath->setMinimumHeight(30);
    editRawLyricPanelSong->setMinimumHeight(30);
    editRawLyricPanelArtist->setMinimumHeight(30);
    btnRawLyricPanelSelect->setMinimumHeight(30);
    btnRawLyricPanelSave->setMinimumHeight(30);

    editRawLyricPanelSavePath->setFocusPolicy(Qt::NoFocus);
    editRawLyricPanelSavePath->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editRawLyricPanelSavePath->setText(SettingManager::GetInstance().data().defaultLyricPath);

    btnRawLyricPanelSelect->setMinimumSize(80,30);
    btnRawLyricPanelSave->setMinimumSize(120,30);
    btnRawLyricPanelSelect->setText("选择");
    btnRawLyricPanelSave->setText("保存原歌词");
    btnRawLyricPanelSelect->setFocusPolicy(Qt::NoFocus);
    btnRawLyricPanelSave->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout* hLayout4 = new QHBoxLayout();
    hLayout4->addWidget(labelRawLyricPanelSavePath);
    hLayout4->addWidget(editRawLyricPanelSavePath);
    hLayout4->addWidget(btnRawLyricPanelSelect);

    QHBoxLayout* hLayout5 = new QHBoxLayout();
    hLayout5->addWidget(labelRawLyricPanelSong);
    hLayout5->addWidget(editRawLyricPanelSong);
    hLayout5->addWidget(labelRawLyricPanelArtist);
    hLayout5->addWidget(editRawLyricPanelArtist);
    hLayout5->addWidget(btnRawLyricPanelSave);

    QVBoxLayout* vLayoutRawPanel = new QVBoxLayout(widgetRawLyricBoard);
    vLayoutRawPanel->addLayout(hLayout4);
    vLayoutRawPanel->addLayout(hLayout5);
    vLayoutRawPanel->addWidget(editTextRawLyric);

    //LRC歌词面板
    widgetLrcLyricBoard = new QWidget(this);

    labelLrcLyricPanelSavePath = new QLabel(widgetLrcLyricBoard);
    labelLrcLyricPanelSong = new QLabel(widgetLrcLyricBoard);
    labelLrcLyricPanelArtist = new QLabel(widgetLrcLyricBoard);
    editLrcLyricPanelSavePath= new BesFileLineEdit(BesFileTypeFloder,widgetLrcLyricBoard);
    editLrcLyricPanelSong= new QLineEdit(widgetLrcLyricBoard);
    editLrcLyricPanelArtist= new QLineEdit(widgetLrcLyricBoard);
    btnLrcLyricPanelSelect = new BesButton(widgetLrcLyricBoard);
    btnLrcLyricPanelSave = new BesButton(widgetLrcLyricBoard);
    editTextLrcLyric = new QTextEdit(widgetLrcLyricBoard);

    labelLrcLyricPanelSavePath->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelLrcLyricPanelSong->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelLrcLyricPanelArtist->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    labelLrcLyricPanelSavePath->setText(tr("保存路径："));
    labelLrcLyricPanelSong->setText(tr("音乐："));
    labelLrcLyricPanelArtist->setText(tr("歌手："));
    labelLrcLyricPanelSavePath->setMinimumSize(100,30);
    labelLrcLyricPanelSong->setMinimumSize(100,30);
    labelLrcLyricPanelArtist->setMinimumSize(100,30);

    labelLrcLyricPanelSavePath->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelLrcLyricPanelArtist->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
    labelLrcLyricPanelSong->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);

    editLrcLyricPanelSavePath->setMinimumHeight(30);
    editLrcLyricPanelSong->setMinimumHeight(30);
    editLrcLyricPanelArtist->setMinimumHeight(30);
    btnLrcLyricPanelSelect->setMinimumHeight(30);
    btnLrcLyricPanelSave->setMinimumHeight(30);

    editLrcLyricPanelSavePath->setFocusPolicy(Qt::NoFocus);
    editLrcLyricPanelSong->setFocusPolicy(Qt::NoFocus);
    editLrcLyricPanelArtist->setFocusPolicy(Qt::NoFocus);
    editLrcLyricPanelSavePath->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    editLrcLyricPanelSavePath->setText(SettingManager::GetInstance().data().defaultOutputPath);

    btnLrcLyricPanelSelect->setMinimumSize(80,30);
    btnLrcLyricPanelSave->setMinimumSize(120,30);
    btnLrcLyricPanelSelect->setText("选择");
    btnLrcLyricPanelSave->setText("保存LRC歌词");
    btnLrcLyricPanelSelect->setFocusPolicy(Qt::NoFocus);
    btnLrcLyricPanelSave->setFocusPolicy(Qt::NoFocus);

    QHBoxLayout* hLayout6 = new QHBoxLayout();
    hLayout6->addWidget(labelLrcLyricPanelSavePath);
    hLayout6->addWidget(editLrcLyricPanelSavePath);
    hLayout6->addWidget(btnLrcLyricPanelSelect);

    QHBoxLayout* hLayout7 = new QHBoxLayout();
    hLayout7->addWidget(labelLrcLyricPanelSong);
    hLayout7->addWidget(editLrcLyricPanelSong);
    hLayout7->addWidget(labelLrcLyricPanelArtist);
    hLayout7->addWidget(editLrcLyricPanelArtist);
    hLayout7->addWidget(btnLrcLyricPanelSave);

    QVBoxLayout* vLayoutLrcPanel = new QVBoxLayout(widgetLrcLyricBoard);
    vLayoutLrcPanel->addLayout(hLayout6);
    vLayoutLrcPanel->addLayout(hLayout7);
    vLayoutLrcPanel->addWidget(editTextLrcLyric);


    //tab 搜索歌词结果页面
    tabpageLyricResult = new QTabWidget(this);
    tabpageLyricResult->setObjectName("tabpageLyricResult");
    tabpageLyricResult->setFocusPolicy(Qt::NoFocus);
    tabpageLyricResult->addTab(tableLyricSearch,"歌词列表");
    tabpageLyricResult->addTab(widgetRawLyricBoard,"原歌词");
    tabpageLyricResult->addTab(widgetLrcLyricBoard,"LRC歌词");
    tabpageLyricResult->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    vMainLayour->addWidget(labelTitleSearchLyric);
    vMainLayour->addSpacerItem(new QSpacerItem(20,10,QSizePolicy::Fixed,  QSizePolicy::Fixed));
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
    connect(tableLyricSearch, SIGNAL(sig_showRawLyric(LyricInfo)), this,SLOT(OnShowRawLyric(LyricInfo)));
    connect(tableLyricSearch, SIGNAL(sig_showLrcLyric(LyricInfo)), this,SLOT(OnShowLrcLyric(LyricInfo)));

    connect(btnRawLyricPanelSelect, SIGNAL(clicked(bool)),this, SLOT(OnSelectRawLyricSavePath()));
    connect(btnRawLyricPanelSave, SIGNAL(clicked(bool)),this, SLOT(OnSaveRawLyric()));

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

    strLastSongName = song;
    strLastArtistName = artist;

    showTipLabel(true);

    labelLyricResultTip2->setText("“" + song + "”");
    labelLyricResultTip4->setText("“" + artist + "”");
    labelLyricResultTip5->setText(tr("正在搜索中..."));
    btnSearchInProgram->setEnabled(false);

    searchThread.StartSearchLyric(artist, song);
}


void SubPageDownloadLyric::OnLyricResultChanged(LyricSearchResult result)
{
    tabpageLyricResult->setCurrentIndex(0);

    //删除列表中所有的已有数据
    static int nLyricCount = 0;
    if(!result.bAppendToList)
    {
        nLyricCount = 0;
        tableLyricSearch->deleteAllItems();
    }

    QString strResultTip;
    if(result.bShowUnexpectedResultTip)
        strResultTip = result.strUnexpectedResultTip;

    //显示搜索结果
    nLyricCount += result.vecLyricInfoTotal.size();
    labelLyricResultTip5->setText(QString().sprintf("，找到%d个歌词文件。",nLyricCount)+strResultTip);
    labelLyricResultTip5->setVisible(true);

    if(result.vecLyricInfoTotal.size()!=0)
        tableLyricSearch->appendItems(result.vecLyricInfoTotal);

    if(result.bCurrentSearchDone) //搜索结束时才能恢复按钮
    {
        //恢复按钮，可以下一次继续搜索
        btnSearchInProgram->setEnabled(true);

        //m_txtIsSearchingLyricTip->SetVisible(FALSE,TRUE); //隐藏正在搜索提示
    }
}

void SubPageDownloadLyric::OnShowRawLyric(const LyricInfo &info)
{
    tabpageLyricResult->setCurrentIndex(1);
    editRawLyricPanelSong->setText(info.strSong);
    editRawLyricPanelArtist->setText(info.strArtist);
    editTextRawLyric->setText(info.strPlaneText);
}

void SubPageDownloadLyric::OnShowLrcLyric(const LyricInfo &info)
{
    tabpageLyricResult->setCurrentIndex(2);
    editLrcLyricPanelSong->setText(info.strSong);
    editLrcLyricPanelArtist->setText(info.strArtist);
    editTextLrcLyric->setText(info.strLabelText);
}

void SubPageDownloadLyric::OnSelectRawLyricSavePath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择保存原歌词路径"),
                                     SettingManager::GetInstance().data().defaultLyricPath,
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(dir.size() !=0)
        editRawLyricPanelSavePath->setText(dir);
}

void SubPageDownloadLyric::OnSelectLrcLyricSavePath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("选择保存LRC歌词路径"),
                                     SettingManager::GetInstance().data().defaultOutputPath,
                                     QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(dir.size() !=0)
        editLrcLyricPanelSavePath->setText(dir);
}

void SubPageDownloadLyric::OnSaveRawLyric()
{
    //检查保存路径和是否有要保存的内容
    if(editRawLyricPanelSavePath->text().size() == 0)
    {
        BesMessageBox::information(tr("提示"),tr("请先选择保存路径"));
        return;
    }

    if(editTextRawLyric->toPlainText().trimmed().size() == 0)
    {
        BesMessageBox::information(tr("提示"),tr("歌词内容为空"));
        return;
    }

    QString fileName;
    QString content = editTextRawLyric->toPlainText();

    //构建 fileName
    QString song = editRawLyricPanelSong->text();
    QString artist = editRawLyricPanelArtist->text();
    if(song.size() == 0 && artist.size() == 0)
    {
        song = strLastSongName;
        artist = strLastArtistName;
    }

    if(song.size() == 0 && artist.size() == 0) //尝试构建失败
    {
        BesMessageBox::information(tr("提示"),tr("歌名和歌手为空，无法保存"));
        return;
    }

    if(song.size() == 0)
        song = "XXX";

    if(artist.size() == 0)
        artist = "XXX";

    fileName = editRawLyricPanelSavePath->text() + "/"
            + song + " - " + artist + ".txt";

    //提示是否保存到路径
    if(QMessageBox::StandardButton::Ok ==
      BesMessageBox::question(tr("保存确认"), tr("是否保存到路径：")+fileName))
    {
        QFile fileOut(fileName);
        if (! fileOut.open(QFile::WriteOnly | QFile::Truncate))
        {
            BesMessageBox::information(tr("失败提示"), tr("无法保存文件:")+ fileName
                                       +"\n\n可能是程序没有写权限");
            return;
        }

        QTextStream streamFileOut(&fileOut);
        streamFileOut.setCodec("UTF-8");
        streamFileOut.setGenerateByteOrderMark(false);
        streamFileOut << content;
        streamFileOut.flush();

        fileOut.close();

        if(QMessageBox::StandardButton::Ok ==
                BesMessageBox::question(tr("自动选择"), tr("歌词已成功保存到路径：")+fileName
                                        +"\n\n" + tr("是否立刻用于制作歌词?")))
        {
            emit sig_autoSelectRawLyric(fileName);
        }
    }
}

void SubPageDownloadLyric::OnSavectLrcLyric()
{
    //检查保存路径和是否有要保存的内容
    if(editLrcLyricPanelSavePath->text().size() == 0)
    {
        BesMessageBox::information(tr("提示"),tr("请先选择保存路径"));
        return;
    }

    if(editTextLrcLyric->toPlainText().trimmed().size() == 0)
    {
        BesMessageBox::information(tr("提示"),tr("歌词内容为空"));
        return;
    }

    QString fileName;
    QString content = editTextLrcLyric->toPlainText();

    //构建 fileName
    QString song = editLrcLyricPanelSong->text();
    QString artist = editLrcLyricPanelArtist->text();
    if(song.size() == 0 && artist.size() == 0)
    {
        song = strLastSongName;
        artist = strLastArtistName;
    }

    if(song.size() == 0 && artist.size() == 0) //尝试构建失败
    {
        BesMessageBox::information(tr("提示"),tr("歌名和歌手为空，无法保存"));
        return;
    }

    if(song.size() == 0)
        song = "XXX";

    if(artist.size() == 0)
        artist = "XXX";

    fileName = editLrcLyricPanelSavePath->text() + "/"
            + song + " - " + artist + ".lrc";

    //提示是否保存到路径
    if(QMessageBox::StandardButton::Ok ==
      BesMessageBox::question(tr("保存确认"), tr("是否保存到路径：")+fileName))
    {
        QFile fileOut(fileName);
        if (! fileOut.open(QFile::WriteOnly | QFile::Truncate))
        {
            BesMessageBox::information(tr("失败提示"), tr("无法保存文件:")+ fileName
                                       +"\n\n可能是程序没有写权限");
            return;
        }

        QTextStream streamFileOut(&fileOut);
        streamFileOut.setCodec("UTF-8");
        streamFileOut.setGenerateByteOrderMark(false);
        streamFileOut << content;
        streamFileOut.flush();

        fileOut.close();

        BesMessageBox::information(tr("提示"), tr("歌词已成功保存到路径：")+fileName );
    }
}

void SubPageDownloadLyric::searchLyricDirectly(const QString& artists, const QString& song)
{
    editSearchLyricSong->setText(song);
    editSearchLyricArtist->setText(artists);

    OnSearchInProgram();
}



void SubPageDownloadLyric::showTipLabel(bool bShow)
{
    labelLyricResultTip1->setVisible(bShow);
    labelLyricResultTip2->setVisible(bShow);
    labelLyricResultTip3->setVisible(bShow);
    labelLyricResultTip4->setVisible(bShow);
    labelLyricResultTip5->setVisible(bShow);
}

