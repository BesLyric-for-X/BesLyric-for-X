#include "StackFrame.h"
#include <QVBoxLayout>
#include "AppHelper.h"
#include "SettingManager.h"
#include "BesScaleUtil.h"

#include <QScreen>

StackFrame::StackFrame(QApplication *pApplication,QWidget *parent)
    : BesFramelessWidget(parent),mainWidget(nullptr),skinBoxWidget(nullptr),addItemWidget(nullptr)
{
    pApp = pApplication;
    this->setMouseTracking(true);

    //先初始化设置（由于没有读取皮肤设置，先设置一个默认的皮肤，好在出错弹框时有一个默认皮肤）【第1次设置】
    SetSkin("black");
    showVersionInfo();
    initSetting();

    QString skinName = SettingManager::GetInstance().data().skinName;

    setBorderMain(8);
    initLayout();
    initConnection();

    //再设置特殊的皮肤，这一步需要初始化好控件，所以放在 initLayout 之后 【第2次设置】
	//同时，指定 bFirstInit 为 true, 以用于初始化 skinBoxWidget 中的所有控件皮肤
    SetSpecialSkin(skinName, true);

	//由于 skinBoxWidget 中，改变初始化控件 滑动条Slider改变值会触发 signalSetCustomSkin ,在6中默认皮肤时，其发送的颜色是不准的
	//		为了能够使用 "black" "red" 等默认皮肤，再设置一次 【第3次设置】
	//全局搜索【FLAG_SETTING_SKIN】查看相关逻辑
	SetSkin(skinName);

    //初始化实体类
    initEntity();
}

StackFrame::~StackFrame()
{

    // QThread: Destroyed while thread is still running
    // 现在的问题是，能否做到立即结束线程。
    login.exit(1);
    login.wait();
    checkUpdate.exit(1);
    checkUpdate.wait();
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

    addItemWidgetLeft = 250 * BesScaleUtil::scale();
    addItemWidgetTop = 108 * BesScaleUtil::scale();

    mainWidget->raise();
    isMainOnTop = true;
}

void StackFrame::initConnection()
{
    connect(mainWidget->topWidget, &TopWidget::OnDoubleClick, this, &StackFrame::toggleMaxRestoreStatus);

    connect(mainWidget->topWidget->btnMax, &BesButton::clicked, this, &StackFrame::toggleMaxRestoreStatus);
    connect(mainWidget->topWidget->btnRestore, &BesButton::clicked, this, &StackFrame::toggleMaxRestoreStatus);

    connect(mainWidget->topWidget->btnSkinBox, &BesButton::clicked, this, &StackFrame::toggleSkinBox);
    connect(mainWidget->middleWidget->pageLyricList->headerListCreated, &BesListHeader::sig_addButtonClicked, this, &StackFrame::toggleAddItemWidget);

    connect(mainWidget->topWidget->btnMini, &BesButton::clicked, this, &StackFrame::showMinimized);
    connect(mainWidget->topWidget->btnClose, &BesButton::clicked, this, &StackFrame::close);

    connect(transparentLayer, &BesTransparentLayer::sig_layerPressed, this, &StackFrame::bringMainToTop);

    connect(addItemWidget, &AddLyricItemWidget::sig_toggleAddWidget, this, &StackFrame::bringMainToTop);
    connect(addItemWidget, &AddLyricItemWidget::sig_addListItem, mainWidget->middleWidget->pageLyricList, &PageLyricList::OnAddNewListItem);
    
    //注意：onSkinClick 这里有2处会触发皮肤的变换，一处是在这里；另一处是这里改变 SkinBoxWidget 触发了纯颜色皮肤设置
    // 这里后连接，所以前面纯颜色的设置不影响这里的效果 ：全局搜索【FLAG_SETTING_SKIN】查看相关逻辑
    connect(skinBoxWidget->btnBlack, &ButtonTheme::onSkinClick, [=](QString skinName) { SetSkin(skinName); });
    connect(skinBoxWidget->btnRed, &ButtonTheme::onSkinClick, [=](QString skinName) { SetSkin(skinName); });
    connect(skinBoxWidget->btnPink, &ButtonTheme::onSkinClick, [=](QString skinName) { SetSkin(skinName); });
    connect(skinBoxWidget->btnBlue, &ButtonTheme::onSkinClick, [=](QString skinName) { SetSkin(skinName); });
    connect(skinBoxWidget->btnGreen, &ButtonTheme::onSkinClick, [=](QString skinName) { SetSkin(skinName); });
    connect(skinBoxWidget->btnGold, &ButtonTheme::onSkinClick, [=](QString skinName) { SetSkin(skinName); });

    connect(skinBoxWidget->btnPureColor1, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor2, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor3, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor4, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor5, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor6, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor7, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor8, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor9, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor10, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor11, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });
    connect(skinBoxWidget->btnPureColor12, &ButtonPureColor::onSkinClick, [=](QString skinName) {SetSkin(skinName); });

    connect(skinBoxWidget, &SkinBoxWidget::signalSetCustomSkin, [=](QString skinName) {SetSkin(skinName); });

    connect(this, &StackFrame::onFinalSkinNameChanged, mainWidget->middleWidget->pagePreviewLyric->lyricViewer, &LyricViewer::skinNameChanged);
    connect(this, &StackFrame::onFinalBaseColorChanged, mainWidget->middleWidget->pageLyricList, &PageLyricList::baseColorChanged);
    connect(this, &StackFrame::onFinalBaseColorChanged, mainWidget->middleWidget->pageLyricList->tableLrcList, &BesLListTableView::baseColorChanged);
   }

void StackFrame::initEntity()
{
    qRegisterMetaType<CheckUpgradeResult>("CheckUpgradeResult");

    connect(&checkUpdate, &ThreadCheckUpdate::sig_haveCheckResult, this, &StackFrame::onUpdateResultFound);

    //登录记录
    login.SendLogin();

    if(SettingManager::GetInstance().data().autoCheckForUpgrade)
    {
        //静默(未找到更新时静默)检测升级
        checkUpdate.checkForUpdate(true, QThread::NormalPriority);
    }

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
    QRect skinBoxRect = QRect(this->width()- 400* BesScaleUtil::scale()- 20* BesScaleUtil::scale(),
                              58* BesScaleUtil::scale(),
                              400* BesScaleUtil::scale(),330* BesScaleUtil::scale());

    QRect transWidgetRect(mainWidgetRect);
    transWidgetRect.setTop(transWidgetRect.top()+65* BesScaleUtil::scale());  //上侧标题控件不要阻挡

    QRect addItemWidgetRect = QRect(addItemWidgetLeft,addItemWidgetTop,350* BesScaleUtil::scale(),280* BesScaleUtil::scale());

    mainWidget->setGeometry(mainWidgetRect);
    transparentLayer->setGeometry(transWidgetRect);
    skinBoxWidget->setGeometry(skinBoxRect);
    addItemWidget->setGeometry(addItemWidgetRect);
}


void StackFrame::SetSkin(QString skinName, bool bFirstInit)
{
    AppHelper::SetStyle(pApp, skinName);

    SetSpecialSkin(skinName, bFirstInit);

    BesMessageBox::setIsBlackTheme(skinName == "black");
}

void StackFrame::SetSpecialSkin(QString skinName ,bool bFirstInit)
{
    if(skinBoxWidget)
        skinBoxWidget->setFinalSkinName(skinName, bFirstInit);

    if(addItemWidget)
        addItemWidget->setFinalSkinName(skinName);

    if(mainWidget)
    {
        mainWidget->middleWidget->pageMain->setFinalSkinName(skinName);

        mainWidget->middleWidget->pageMain->boxPageLyricList->setFinalSkinName(skinName);

        mainWidget->middleWidget->pagePreviewLyric->setWheterToUseBlackMask( skinName == "black");

        {
        mainWidget->middleWidget->pageLyricList->lyricListHistory->setFinalSkinName( skinName);
        mainWidget->middleWidget->pageLyricList->lyricListCreated->setFinalSkinName( skinName);
        }
        mainWidget->middleWidget->pageSetting->settingWidget->settingLeftNavigator->setFinalSkinName(skinName);
    }

    emit onFinalSkinNameChanged(skinName);

    if(!bFirstInit)
        emit onFinalBaseColorChanged(AppHelper::GetBaseColorFromStyleName(skinName));
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
        //在桌面左上角不为 (0, 0) 时，这个就不能用了
//        setGeometry(-borderMain, -borderMain, width()+2*borderMain, height()+ 2*borderMain);

        //https://stackoverflow.com/questions/2641193/qt-win-showmaximized-overlapping-taskbar-on-a-frameless-window
        //要获取真实的可用范围，而不是写死

        //'availableGeometry' is deprecated: Use QGuiApplication::screens()
//        QRect availableGeometry(QApplication::desktop()->availableGeometry());
        QRect availableGeometry(QGuiApplication::screens().first()->availableGeometry());

        availableGeometry.adjust(-borderMain, -borderMain, borderMain, borderMain);
        setGeometry(availableGeometry);

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

void StackFrame::onUpdateResultFound(CheckUpgradeResult result)
{

    QString strUpdate;
    if(result.infoList.size() == 0)
        strUpdate = "没有具体的更新信息";
    else
    {
        strUpdate += "<p>";
        for(auto line: result.infoList)
        {
            strUpdate += line;
            strUpdate += "<br/>";
        }
        strUpdate += "</p>";
    }

    BesMessageBox::information(tr("有新版本  ") + result.versionNum, strUpdate);

}

void StackFrame::showVersionInfo(){
    qDebug() << "VERSION: " << VERSION_NUMBER << ", COMMIT: " << GIT_COMMIT_SHA1;
#if !defined(RELEASE_VERSION)
    BesMessageBox::information(
        tr("这是测试版"),
        tr("版本： %1\n提交： %2").arg(VERSION_NUMBER).arg(GIT_COMMIT_SHA1));
#endif
}

