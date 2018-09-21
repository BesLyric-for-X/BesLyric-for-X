#include "global.h"
#include "BesNcmSongTableView.h"
#include "BesNcmSongTableModel.h"
#include "BesNcmSongButtonDelegate.h"
#include <QHeaderView>

#include <QDesktopServices>
#include <QUrl>
#include "NetAccess.h"
#include "BesMessageBox.h"
#include "SettingManager.h"

BesNcmSongTableView::BesNcmSongTableView(QWidget *parent) :
    QTableView(parent)
{
    this->setMouseTracking(true);//详见 BesFramelessWidget.h 注释

    initEntity();
    initConnection();
    BaseInit();
}

BesNcmSongTableView::~BesNcmSongTableView()
{
    delete m_model;
}

void BesNcmSongTableView::deleteAllItems()
{
    m_model->deleteAllItems();
    emit m_model->layoutChanged();
    this->update();
}

void BesNcmSongTableView::appendItems(const QVector<SONGINFO>& infos)
{
    QVector<SONGINFO> infos_dealed = initSongInfo(infos);

    m_model->appendItems(infos_dealed);
    emit m_model->layoutChanged();
    this->update();
}

void BesNcmSongTableView::setItems(const QVector<SONGINFO> &infos)
{
    QVector<SONGINFO> infos_dealed = initSongInfo(infos);

    m_model->deleteAllItems();
    m_model->appendItems(infos_dealed);
    emit m_model->layoutChanged();
    this->update();
}

void BesNcmSongTableView::OnDownloadNcmMusic(SONGINFO songInfo)
{
    //先查看配置是否同意下载声明
    if(!SettingManager::GetInstance().data().agreeDownloadDeclaration
            || SettingManager::GetInstance().data().musicDowloadPath.size() == 0)
    {
        BesMessageBox::information(tr("提示"),tr("请先在 “设置 -> 歌曲下载” 下 “选择” 歌曲下载目录"));
        return;
    }

    QFile file(SettingManager::GetInstance().data().musicDowloadPath);
    if(!file.exists())
    {
        BesMessageBox::information(tr("提示"),tr("设置的路径无效，请先在 “设置 -> 歌曲下载” 下 “选择” 歌曲下载目录"));
        return;
    }

    if(songInfo.nPercentage == -1) //只有从未下载过时，才尝试下载
    {
        emit sig_oneDownloadStarted();

        OnProgressChanged("0 kB/s", 5,QVariant(songInfo.nID)); //先发一个信号，让界面有所显示

        QString strId = QString().number(songInfo.nID);
        QString strLink = "http://music.163.com/song/media/outer/url?id="+ strId +".mp3";
        QString strSavePath = SettingManager::GetInstance().data().musicDowloadPath + '/'
                +songInfo.strSong +"-"+ songInfo.strArtists + ".mp3";

        net.DownloadFile(strLink, strSavePath, songInfo.nID);
    }
    else if(songInfo.nPercentage == -2)
    {
        //已经尝试过，404 找不到音乐，应该是没版权
        BesMessageBox::information(tr("提示"),tr("由于版权保护等原因，无法获取该音乐"));
    }
    else if(songInfo.nPercentage == 100)
    {
        //直接选择歌曲到制作页面，并自动跳转页面
        QString strSavePath = SettingManager::GetInstance().data().musicDowloadPath + '/'
                +songInfo.strSong +"-"+ songInfo.strArtists + ".mp3";

        emit sig_setMusicPathToMakingPage(strSavePath);
    }
}

void BesNcmSongTableView::OnFinishedDownload(QVariant data, DOWNLOAD_FINISH_STATUS status)
{
    emit sig_oneDownloadFinished();

    int nSongId = data.toInt();

    QVector<SONGINFO>& infos =m_model->DataVector();
    for(SONGINFO& info: infos)
    {
        if(info.nID == nSongId)
        {
            if(status == DOWNLOAD_FINISH_STATUS::NORMAL)
                info.nPercentage = 100;
            else
                info.nPercentage = -2;

            update();
        }
    }
}

void BesNcmSongTableView::OnProgressChanged(QString speed, int percentage, QVariant data)
{
    Q_UNUSED(speed);

    int nSongId = data.toInt();

    QVector<SONGINFO>& infos =m_model->DataVector();
    for(SONGINFO& info: infos)
    {
        if(info.nID == nSongId)
        {
            info.nPercentage = percentage < 5 ? 5: percentage;  //为了用户不因网络卡顿而看不到任何进度，最小显示5

            update();
        }
    }
}

void BesNcmSongTableView::initEntity()
{
    m_model = new BesNcmSongTableModel();
    this->setModel(m_model);

    QStringList headers;
    headers << "" << tr("操作")<< tr("音乐标题")<< tr("歌手")<< tr("专辑")<< tr("时长");
    m_model->setHorizontalHeader(headers);
    m_buttonDelegate = new BesNcmSongButtonDelegate(this);
    this->setItemDelegateForColumn(1, m_buttonDelegate);
}

void BesNcmSongTableView::initConnection()
{
    connect(m_buttonDelegate, SIGNAL(sig_rowClicked(int)),this,SLOT(selectRow(int)));
    connect(m_buttonDelegate,&BesNcmSongButtonDelegate::sig_preview_ncm_song, [=](int row){
       QString strLink = "http://music.163.com/#/song?id=" + QString().number(m_model->DataVector().at(row).nID);
       QDesktopServices::openUrl(QUrl(strLink));
    });
    connect(m_buttonDelegate,&BesNcmSongButtonDelegate::sig_listen_ncm_song, [=](int row){
       QString strLink = "http://music.163.com/song/media/outer/url?id=" + QString().number(m_model->DataVector().at(row).nID);
       QDesktopServices::openUrl(QUrl(strLink));
    });
    connect(m_buttonDelegate,&BesNcmSongButtonDelegate::sig_download_ncm_song, [=](int row){
        OnDownloadNcmMusic( m_model->DataVector().at(row));
    });
    connect(m_buttonDelegate, &BesNcmSongButtonDelegate::sig_setMusicPathToMakingPage, [=](QString musicPath)
            {emit sig_setMusicPathToMakingPage(musicPath);});

    connect(&net, SIGNAL(sig_finishDownload(QVariant,DOWNLOAD_FINISH_STATUS)),
            this, SLOT(OnFinishedDownload(QVariant,DOWNLOAD_FINISH_STATUS)));
    connect(&net,SIGNAL(sig_progressChanged(QString,int,QVariant)),
            this,SLOT(OnProgressChanged(QString,int,QVariant)));

}

 //基础的初始化
void BesNcmSongTableView::BaseInit()
{
    this->setFocusPolicy(Qt::NoFocus);
    this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->setColumnWidth(0,50);
    this->setColumnWidth(1,100);
    this->setColumnWidth(5,120);
    this->verticalHeader()->setVisible(true);
    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeMode::Fixed);
    this->horizontalHeader()->setMinimumSectionSize(50);
    this->horizontalHeader()->setHighlightSections(false);
    this->setGridStyle(Qt::PenStyle::NoPen);
    this->setShowGrid(false);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//自动调整大小
void BesNcmSongTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);

    double widthLeft = this->width() - 50- 100 - 120 - 15;
    if(widthLeft <= 0)
        return;

    this->setColumnWidth(2,widthLeft * 3/7);
    this->setColumnWidth(3,widthLeft * 2/7);
    this->setColumnWidth(4,widthLeft * 2/7);
}

QVector<SONGINFO> BesNcmSongTableView::initSongInfo(const QVector<SONGINFO> &infos)
{
    QVector<SONGINFO> infos_dealed = infos;
    for(SONGINFO& info:infos_dealed)
        info.nPercentage = -1;

    return infos_dealed;
}

