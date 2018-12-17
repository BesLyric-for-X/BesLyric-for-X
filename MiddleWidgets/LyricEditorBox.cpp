#include "global.h"
#include "LyricEditorBox.h"
#include "BesScaleUtil.h"
#include "BesMessageBox.h"


LyricEditorBox::LyricEditorBox(QWidget *parent):BesFramelessDialog(parent)
{
    this->setMinimumHeight(720* BesScaleUtil::mscale());
    this->setMinimumWidth(800* BesScaleUtil::mscale());

    initLayout();
    initConnection();
}

void LyricEditorBox::initLayout()
{
    widgetMessageBoxContainer = new QWidget(this);
    widgetMessageBoxContainer->setMouseTracking(true);
    widgetMessageBoxContainer->setObjectName("widgetMessageBoxContainer");

    QVBoxLayout * vMainLayout0 = new QVBoxLayout(this);
    vMainLayout0->setMargin(8);
    vMainLayout0->setSpacing(0);

    //由于设置样式要求有边缘，再内嵌一个QWidget
    widgetBaseContainer= new QWidget(widgetMessageBoxContainer);
    widgetBaseContainer->setObjectName("widgetMessageBoxContainer");
    widgetBaseContainer->setMouseTracking(true);

    QVBoxLayout * vMainLayout = new QVBoxLayout(widgetMessageBoxContainer);
    vMainLayout->setMargin(1);
    vMainLayout->setSpacing(0);
    vMainLayout->addWidget(widgetBaseContainer);

    vMainLayout0->addWidget(widgetMessageBoxContainer);

    widgetHeader = new QWidget(widgetBaseContainer);
    widgetBody = new QWidget(widgetBaseContainer);
    widgetFooter = new QWidget(widgetBaseContainer);
    widgetHeader->setObjectName("widgetHeader");
    widgetBody->setObjectName("widgetBody");
    widgetFooter->setObjectName("widgetFooter");
    widgetHeader->setMouseTracking(true);
    widgetBody->setMouseTracking(true);
    widgetFooter->setMouseTracking(true);

    widgetHeader->setMinimumHeight(45* BesScaleUtil::mscale());
    widgetHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    widgetBody->setMinimumHeight(400* BesScaleUtil::mscale());
    widgetBody->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widgetFooter->setMinimumHeight(60* BesScaleUtil::mscale());
    widgetFooter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout * vLayout = new QVBoxLayout(widgetBaseContainer);
    vLayout->setMargin(0);
    vLayout->setSpacing(0);
    vLayout->addWidget(widgetHeader);
    vLayout->addWidget(widgetBody);
    vLayout->addWidget(widgetFooter);

    QHBoxLayout *hHeaderLayout = new QHBoxLayout(widgetHeader);

    labelMessageTitle = new QLabel(widgetHeader);
    labelMessageTitle->setObjectName("labelMessageTitle");
    labelMessageTitle->setText(tr("歌词编辑"));
    btnMessageClose = new BesButton(widgetHeader);
    btnMessageClose->setObjectName("btnMessageClose");

    hHeaderLayout->addWidget(labelMessageTitle);
    hHeaderLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::Expanding, QSizePolicy::Fixed));
    hHeaderLayout->addWidget(btnMessageClose);

    QVBoxLayout *vBodyLayout = new QVBoxLayout(widgetBody);

    labelLyricFinishPart = new QLabel(widgetBody);
    labelLyricFinishPart->setText(tr("已制作部分"));
    labelLyricFinishPart->setMinimumSize(100* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    labelLyricFinishPart->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    labelLyricUnfinishPart = new QLabel(widgetBody);
    labelLyricUnfinishPart->setText(tr("未制作部分"));
    labelLyricUnfinishPart->setMinimumSize(100* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    labelLyricUnfinishPart->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    btnCopySelected = new BesButton(widgetBody);
    btnCopySelected->setText(tr("复制已选行"));
    btnCopySelected->setMinimumSize(100* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnCopySelected->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    btnPastSelected = new BesButton(widgetBody);
    btnPastSelected->setText(tr("粘贴"));
    btnPastSelected->setMinimumSize(100* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnPastSelected->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    btnPastSelected->setEnabled(false);

    btnRemakeLastLine = new BesButton(widgetBody);
    btnRemakeLastLine->setText(tr("重制最后一行"));
    btnRemakeLastLine->setMinimumSize(100* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnRemakeLastLine->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    btnRemakeLastLine->setVisible(false); //暂时不考虑该功能

    tableLyric =  new FinishLrcTableView(widgetBody);
    tableLyric->setObjectName("tableLyric");
    editUnfinishPart = new QTextEdit(widgetBody);

    QHBoxLayout *hLayout1 = new QHBoxLayout();
    hLayout1->addWidget(labelLyricFinishPart);
    hLayout1->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding, QSizePolicy::Fixed));
    hLayout1->addWidget(btnCopySelected);

    QHBoxLayout *hLayout2 = new QHBoxLayout();
    hLayout2->addWidget(labelLyricUnfinishPart);
    hLayout2->addSpacerItem(new QSpacerItem(20,20,QSizePolicy::Expanding, QSizePolicy::Fixed));
    hLayout2->addWidget(btnRemakeLastLine);
    hLayout2->addWidget(btnPastSelected);

    vBodyLayout->addLayout(hLayout1);
    vBodyLayout->addWidget(tableLyric);
    vBodyLayout->addLayout(hLayout2);
    vBodyLayout->addWidget(editUnfinishPart);

    QHBoxLayout *hBottomLayout = new QHBoxLayout(widgetFooter);

    btnOK = new BesButton(widgetFooter);
    btnCancel = new BesButton(widgetFooter);
    btnOK->setObjectName("btnOK");
    btnCancel->setObjectName("btnCancel");
    btnOK->setText(tr("确定"));
    btnCancel->setText(tr("取消"));
    btnOK->setMinimumSize(75* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnCancel->setMinimumSize(75* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnOK->setMaximumSize(200* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnCancel->setMaximumSize(200* BesScaleUtil::mscale(),30* BesScaleUtil::mscale());
    btnOK->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    btnCancel->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);


    hBottomLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::Expanding, QSizePolicy::Fixed));
    hBottomLayout->addWidget(btnOK);
    hBottomLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::Fixed, QSizePolicy::Fixed));
    hBottomLayout->addWidget(btnCancel);
    hBottomLayout->addSpacerItem(new QSpacerItem(20* BesScaleUtil::mscale(),20,QSizePolicy::Expanding, QSizePolicy::Fixed));
}

void LyricEditorBox::initConnection()
{
    connect(btnMessageClose, &QPushButton::clicked,[=](){this->close(); btnResult= QMessageBox::StandardButton::Cancel;});
    connect(btnOK, &QPushButton::clicked,[=](){this->accept();btnResult= QMessageBox::StandardButton::Ok;});
    connect(btnCancel, &QPushButton::clicked,[=](){this->accept();btnResult= QMessageBox::StandardButton::Cancel;});

    connect(btnCopySelected,SIGNAL(clicked(bool)),this,SLOT(onCopySelected()));
    connect(btnPastSelected,SIGNAL(clicked(bool)),this,SLOT(onPastSelected()));

}


bool LyricEditorBox::isResultOK()
{
    return btnResult == QMessageBox::StandardButton::Ok;
}


void LyricEditorBox::setLyricData(const QVector<QString>& _rawLines, const QVector<QPair<quint64, QString>>& _lrcLines)
{
    rawLines = _rawLines;
    lrcLines = _lrcLines;

    tableLyric->setLyricData(&lrcLines);

    //取出未制作的部分显示
    int rawCount = 0;
    for(auto&line:_lrcLines)
    {
        rawCount += line.second.isEmpty()?0:1;
    }

    QString unfinish;
    if(rawCount < rawLines.size())
    {
        for(int i=rawCount; i< rawLines.size(); i++ )
        {
            unfinish += rawLines[i];
            unfinish += "\n";
        }
    }

    editUnfinishPart->setText(unfinish);
}


void LyricEditorBox::getLyricData(QVector<QString>& _rawLines, QVector<QPair<quint64, QString>>& _lrcLines)
{
    //重新收集原歌词
    _rawLines.clear();
    for(const auto& lrc:lrcLines)
    {
        QString line = lrc.second.trimmed();
        if(!line.isEmpty())
            _rawLines.push_back(line);
    }

    //从edit 收集剩下的原歌词
    QString content = editUnfinishPart->toPlainText().trimmed();
    QStringList strs = content.split("\n");
    for(auto line:strs)
    {
        line = line.trimmed();
        if(!line.isEmpty())
            _rawLines.push_back(line);
    }

    _lrcLines = lrcLines;
}


void LyricEditorBox::onCopySelected()
{
    QVector<QString> lrcs = tableLyric->getSelectedLrc();
    if(lrcs.size() == 0)
    {
        BesMessageBox::information(tr("提示"),tr("当前未选择任何歌词"));
        btnPastSelected->setEnabled(false);
    }
    else
    {
        lineSelected = lrcs;
        btnPastSelected->setEnabled(true);
    }
}

void LyricEditorBox::onPastSelected()
{
    QString textContent;
    textContent += "\n";
    for(auto line:lineSelected)
    {
        textContent += line;
        textContent += "\n";
    }
    textContent += "\n";

    editUnfinishPart->insertPlainText(textContent);

    btnPastSelected->setEnabled(false);
}
