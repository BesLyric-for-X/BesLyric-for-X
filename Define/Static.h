#pragma once
#include <QtGlobal>
#include <QString>

/* 全局变量定义 */
/* 版本格式说明：X.Y.Z
X表示主版本，架构性修改时更新
Y表示次版本，较大修改时更新
Z表示修改号，小问题时更新
*/
static const QString VERSION_NUMBER = APP_VERSION;

static const QString LINK_SEND_LOGIN = "http://beslyric.320.io/BesBlog/beslyric/login.action";

//windows 平台
#ifdef Q_OS_WIN32

static const QString OS_NAME = "windows";
static const QString  LINK_VERSION_LOG= "http://files.cnblogs.com/files/BensonLaur/versionLog.zip";			//链接，指向版本日志文件
static const QString  LINK_LAST_VERSION_INFO= "https://files-cdn.cnblogs.com/files/BensonLaur/lastVersion-windows.zip";		//链接，指向最后版本信息的文件

#endif

//linux 平台
#ifdef Q_OS_LINUX

static const QString OS_NAME = "linux";
static const QString  LINK_VERSION_LOG= "http://files.cnblogs.com/files/BensonLaur/versionLog.zip";			//链接，指向版本日志文件
static const QString  LINK_LAST_VERSION_INFO= "https://files-cdn.cnblogs.com/files/BensonLaur/lastVersion-linux.zip";		//链接，指向最后版本信息的文件

#endif

//mac 平台
#ifdef Q_OS_MAC

static const QString OS_NAME = "mac";
static const QString  LINK_VERSION_LOG= "http://files.cnblogs.com/files/BensonLaur/versionLog.zip";			//链接，指向版本日志文件
static const QString  LINK_LAST_VERSION_INFO= "https://files-cdn.cnblogs.com/files/BensonLaur/lastVersion-mac.zip";		//链接，指向最后版本信息的文件

#endif

static const QString VERSION_NAME = "beslyric-for-" + OS_NAME;




static const QString SERVER_FILE_EXTENTION = ".zip";							//定义上传到 cnblog服务器的文件拓展名

static const QString LINK_SERVER_PATH = "http://files.cnblogs.com/files/BensonLaur/";		//链接，服务器地址
