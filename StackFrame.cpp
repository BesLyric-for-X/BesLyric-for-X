#include "StackFrame.h"
#include <QVBoxLayout>
#include "AppHelper.h"
#include "SettingManager.h"

StackFrame::StackFrame(QApplication *pApplication,QWidget *parent)
    : BesFramelessWidget(parent),mainWidget(nullptr),skinBoxWidget(nullptr),addItemWidget(nullptr)
{
    pApp = pApplication;
    this->setMouseTracking(true);

    //先初始化设置（由于没有读取皮肤设置，先设置一个默认的皮肤）
    SetSkin("black");
    initSetting();

    QString skinName = SettingManager::GetInstance().data().skinName;

    //先设置基本的皮肤样式，再初始化控件，这样在出错弹框提示时，样式能够起作用
    SetSkin(skinName);

    setBorderMain(8);
    initLayout();
    initConnection();

    //再设置特殊的皮肤，这一步需要初始化好控件，所以放在 initLayout 之后
    SetSpecialSkin(skinName);

    //初始化实体类
    initEntity();
}

StackFrame::~StackFrame()
{

}

void StackFrame::initSetting()
{
    //载入设置
    SettingManager::GetInstance().loadSettingData();

}

void StackFrame::initLayout()
{
    mainWidget = new MainWidget(this);

    transparentLayer = new BesTransparentLayer(this);       //透明层，用于放置在浮动框和主窗口之间
    skinBoxWidget = new SkinBoxWidget(this);
    addItemWidget= new AddLyricItemWidget(this);

    addItemWidgetLeft = 250;
    addItemWidgetTop = 108;

    mainWidget->raise();
    isMainOnTop = true;
}

void StackFrame::initConnection()
{
    connect(mainWidget->topWidget, SIGNAL(OnDoubleClick()),this, SLOT(toggleMaxRestoreStatus()));

    connect(mainWidget->topWidget->btnMax, SIGNAL(clicked(bool)), this, SLOT(toggleMaxRestoreStatus()));
    connect(mainWidget->topWidget->btnRestore, SIGNAL(clicked(bool)), this, SLOT(toggleMaxRestoreStatus()));

    connect(mainWidget->topWidget->btnSkinBox, SIGNAL(clicked(bool)), this, SLOT(toggleSkinBox()));
    connect(mainWidget->middleWidget->pageLyricList->headerListCreated,SIGNAL(sig_addButtonClicked())
            ,this,SLOT(toggleAddItemWidget()));

    connect(mainWidget->topWidget->btnMini, SIGNAL(clicked(bool)), this, SLOT(showMinimized()));
    connect(mainWidget->topWidget->btnClose, SIGNAL(clicked(bool)), this, SLOT(close()));

    connect(transparentLayer,SIGNAL(sig_layerPressed()),this,SLOT(bringMainToTop()));

    connect(addItemWidget,SIGNAL(sig_toggleAddWidget()),this,SLOT(bringMainToTop()));
    connect(addItemWidget,SIGNAL(sig_addListItem(QString)),
            mainWidget->middleWidget->pageLyricList, SLOT(OnAddNewListItem(QString)));
    
    //注意：onSkinClick 这里有2处会触发皮肤的变换，一处是在这里；另一处是这里改变 SkinBoxWidget 触发了纯颜色皮肤设置
    // 这里后连接，所以前面纯颜色的设置不影响这里的效果 ：全局搜索【FLAG_SETTING_SKIN】查看相关逻辑
    connect(skinBoxWidget->btnBlack, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnRed, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));

    connect(skinBoxWidget->btnPureColor1, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor2, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor3, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor4, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor5, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor6, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor7, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor8, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor9, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor10, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor11, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));
    connect(skinBoxWidget->btnPureColor12, SIGNAL(onSkinClick(QString)),this,SLOT(SetSkin(QString)));

    connect(skinBoxWidget, SIGNAL(signalSetCustomSkin(QString)),this,SLOT(SetSkin(QString)));

    connect(this,SIGNAL(onFinalSkinNameChanged(QString)),
            this->mainWidget->middleWidget->pagePreviewLyric->lyricViewer, SLOT(skinNameChanged(QString)));
}

void StackFrame::initEntity()
{

}

void StackFrame::setBorderMain(int border)
{
    borderMain = border;
    BesFramelessWidget::SetFrameBorder(borderMain);
}

void StackFrame::mouseReleaseEvent(QMouseEvent *event)
{
    BesFramelessWidget::mouseReleaseEvent(event);
}

void StackFrame::mouseMoveEvent(QMouseEvent *event)
{
    //所有的mouseMove 都希望传给 BesFramelessWidget处理
    BesFramelessWidget::mouseMoveEvent(event);
}

void StackFrame::mousePressEvent(QMouseEvent *event)
{
    //由于 BesFramelessWidget 要求获得所有鼠标信息，处理鼠标图案和拖动功能，但是
    //          这里只希望处理标题拖动 和 边缘resize 操作，所以判断是否在标题 或 边缘，决定是否处理press
    QPoint currentPoint = event->pos();
    if(mainWidget->topWidget->geometry().contains(currentPoint)||
            !mainWidget->geometry().contains(currentPoint))
        BesFramelessWidget::mousePressEvent(event);
    else
        QWidget::mousePressEvent(event);
}

void StackFrame::resizeEvent(QResizeEvent *event)
{
    BesFramelessWidget::resizeEvent(event);

    QRect mainWidgetRect = QRect(borderMain ,borderMain,
                                 this->width()- 2*borderMain, this->height()-2*borderMain);
    QRect skinBoxRect = QRect(this->width()-420,58,400,330);

    QRect transWidgetRect(mainWidgetRect);
    transWidgetRect.setTop(transWidgetRect.top()+65);  //上侧标题控件不要阻挡

    QRect addItemWidgetRect = QRect(addItemWidgetLeft,addItemWidgetTop,350,280);

    mainWidget->setGeometry(mainWidgetRect);
    transparentLayer->setGeometry(transWidgetRect);
    skinBoxWidget->setGeometry(skinBoxRect);
    addItemWidget->setGeometry(addItemWidgetRect);
}


void StackFrame::SetSkin(QString skinName)
{
    AppHelper::SetStyle(pApp, skinName);

    SetSpecialSkin(skinName);
}

void StackFrame::SetSpecialSkin(QString skinName)
{
    if(skinBoxWidget)
        skinBoxWidget->setFinalSkinName(skinName);

    if(addItemWidget)
        addItemWidget->setFinalSkinName(skinName);

    if(mainWidget && mainWidget->middleWidget && mainWidget->middleWidget->pageMain)
        mainWidget->middleWidget->pageMain->setFinalSkinName(skinName);

    if(mainWidget && mainWidget->middleWidget && mainWidget->middleWidget->pageMain
            && mainWidget->middleWidget->pageMain->boxPageLyricList)
         mainWidget->middleWidget->pageMain->boxPageLyricList->setFinalSkinName(skinName);

    if(mainWidget && mainWidget->middleWidget && mainWidget->middleWidget->pagePreviewLyric)
        mainWidget->middleWidget->pagePreviewLyric->setWheterToUseBlackMask( skinName == "black");

    emit onFinalSkinNameChanged(skinName);
}


//切换最大化和最小化
void  StackFrame::toggleMaxRestoreStatus()
{
    if(isMaximized())
    {
        showNormal();

        mainWidget->topWidget->btnMax->setVisible(true);
        mainWidget->topWidget->btnRestore->setVisible(false);
    }
    else
    {
        showMaximized();

        setGeometry(-borderMain, -borderMain, width()+2*borderMain, height()+ 2*borderMain);

        mainWidget->topWidget->btnMax->setVisible(false);
        mainWidget->topWidget->btnRestore->setVisible(true);
    }
}

//显示或隐藏皮肤盒
void StackFrame::toggleSkinBox()
{
    if(isMainOnTop)
    {
        transparentLayer->raise();
        skinBoxWidget->raise();
        isMainOnTop = false;
    }
    else
    {
        mainWidget->raise();
        isMainOnTop = true;
    }
}

void StackFrame::toggleAddItemWidget()
{
    if(isMainOnTop)
    {
        transparentLayer->raise();
        addItemWidget->raise();
        isMainOnTop = false;
    }
    else
    {
        mainWidget->raise();
        isMainOnTop = true;
    }
}

bool StackFrame::mousePressFilter(QMouseEvent *event)
{
    QPoint currentPoint = event->pos();

	static bool  ignoreNextPress = false;

	if (ignoreNextPress)
	{
		ignoreNextPress = false;
		return false;
	}

    if(skinBoxWidget->geometry().contains(currentPoint))
    {
        ignoreNextPress = true;  //不知为何，落在目标浮动窗的鼠标事件结束后，会再收到一个坐标不同的
        return false; //落在任意浮动窗内，不处理
    }

    return bringMainToTop();
}

//将主程序控件提到最前（隐藏所有浮动框）(生效则返回true)
bool StackFrame::bringMainToTop()
{
    if(!isMainOnTop)
    {
        mainWidget->raise();
        isMainOnTop = true;

        return true;
    }

    return false;
}

