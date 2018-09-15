#include "LyricListManager.h"
#include "BesMessageBox.h"

LyricListManager &LyricListManager::GetInstance()
{
    static LyricListManager* instance = nullptr;
    if(instance == nullptr)
        instance = new LyricListManager();

    return *instance;
}

LyricListData LyricListManager::getLyricListData()
{
    if(!bDataLoaded)
        loadFromSettingDir();

    return listData;
}

bool LyricListManager::saveLyricListData(LyricListData data)
{
    QString path = MakeSureConfigPathAvailable();
    if(path.size() == 0)
        return false;



    //成功保存后，赋值给listData
    listData = data;
    return true;
}


void LyricListManager::loadFromSettingDir()
{
    QString path = MakeSureConfigPathAvailable();
    if(path.size() == 0)
        return;

    if(!QFile::exists(path))  //不存在，自动创建歌词单文件，并保存
    {
        saveLyricListData(listData);
    }
    else //存在，读取配置
    {
//        if(!LoadAppConfig())
//            BesMessageBox::information(tr("提示"),tr("载入歌词单文件失败")+":" + confPath);
    }

    bDataLoaded = true;
}

QString LyricListManager::MakeSureConfigPathAvailable()
{
    QString StrSettingsDir = QCoreApplication::applicationDirPath() + "/settings";

    //如果settings 目录不存在则创建目录
    QDir SettingDir(StrSettingsDir);
    if(!SettingDir.exists())
    {
        if(!SettingDir.mkpath(StrSettingsDir))
        {
            BesMessageBox::information(tr("提示"),tr("无法为配置创建目录")+":" + StrSettingsDir);
            return "";
        }
    }

    //得到目标路径
    return StrSettingsDir + "/lyricList.xml";
}













////获得打数 (如果未加载自动去文件读取，已加载则使用已载入的参数。bForceLoadFromFile 强制从文件获得最新参数)
//bool  AppConfig::GetAppParameter(AppConfigParameter& param, bool bForceLoadFromFile)
//{
//    if(!m_hasLoad || bForceLoadFromFile)
//    {
//        QString confPath;
//        if(!MakeSureConfigPathAvailable(confPath))  //获得路径
//            return false;

//        if(!QFile::exists(confPath))  //不存在，自动创建配置，并提示
//        {
//            param = AppConfigParameter();

//            if(SetAppParameter(param))
//                QMessageBox::information(nullptr,tr("提示"),tr("配置文件不存在，已自动创建")+":" + confPath,
//                                        QMessageBox::Yes,QMessageBox::Yes);
//            else
//                QMessageBox::information(nullptr,tr("提示"),tr("配置文件不存在，自动创建失败")+":" + confPath,
//                                        QMessageBox::Yes,QMessageBox::Yes);

//            m_param = param;

//        }
//        else //存在，读取配置
//        {
//            if(!LoadAppConfig())
//                QMessageBox::information(nullptr,tr("提示"),tr("载入配置文件失败")+":" + confPath,
//                                        QMessageBox::Yes,QMessageBox::Yes);
//        }

//        m_hasLoad = true;
//    }

//    param = m_param;

//    return true;
//}

////设置参数
//bool AppConfig::SetAppParameter(const AppConfigParameter& param)
//{
//    m_param = param;

//    return SaveAppConfig();
//}

// //判断当前配置是否是有效的“只显示一张图片”
//bool AppConfig::IsValidOnlyShowOneImage()
//{
//    if(m_param.onlyShowOneImage) //有设置onluyShowOneImage
//    {
//        if(m_param.initPath.size() != 0)
//        {
//            QFileInfo fi(m_param.initPath);
//            if(fi.isFile())     //传入的路径确实是一个文件
//                return true;
//        }
//    }

//    return false;
//}

////从默认路径中加载打印参数
//bool AppConfig::LoadAppConfig()
//{
//    QString confPath;
//    if(!MakeSureConfigPathAvailable(confPath))  //确保配置路径可用，并获得了路径
//        return false;

//    //打开文件
//    QFile file(confPath);
//    if (!file.open(QFile::ReadOnly | QFile::Text)) { // 只读模式打开文件
//            qDebug() << QString("Cannot read file %1(%2).").arg(confPath).arg(file.errorString());
//            return false;
//    }

//    AppConfigParameter param;

//    //读取示例参考: https://blog.csdn.net/liang19890820/article/details/52808829

//    //将配置从xml文件读出
//    QXmlStreamReader reader(&file);

//    QString strElementName = "";

//    // 解析 XML，直到结束
//    while (!reader.atEnd()) {

//        // 读取下一个元素
//        QXmlStreamReader::TokenType nType = reader.readNext();

//        if (nType == QXmlStreamReader::StartElement)
//        {
//            strElementName = reader.name().toString();
//            if (QString::compare(strElementName, "appConfig") == 0) // 根元素
//            {
//                parseConfig(reader, param);  //解析 printConfig
//            }
//        }
//    }

//    if (reader.hasError()) {  // 解析出错
//           qDebug() << QObject::tr("错误信息：%1  行号：%2  列号：%3  字符位移：%4").arg(reader.errorString()).arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.characterOffset());
//    }

//    file.close();  // 关闭文件

//    m_param = param;

//    return true;
//}

////解析config
//void AppConfig::parseConfig(QXmlStreamReader &reader, AppConfigParameter &param)
//{
//    while (!reader.atEnd()) {

//        reader.readNext();
//        if (reader.isStartElement()) {  // 开始元素
//            QString strElementName = reader.name().toString();
//            if(strElementName == "user")
//            {
//                parseUser(reader, param);
//            }
//            else if(strElementName == "default")
//            {
//               parseDefault(reader, param);
//            }
//        }
//        else if(reader.isEndElement()) {

//            QString strElementName = reader.name().toString();
//            if (QString::compare(strElementName, "appConfig") == 0) {
//                break;  // 跳出
//            }
//        }
//    }
//}


////解析user
//void AppConfig::parseUser(QXmlStreamReader &reader, AppConfigParameter &param)
//{
//    while (!reader.atEnd()) {

//        reader.readNext();
//        if (reader.isStartElement()) {  // 开始元素
//            QString strElementName = reader.name().toString();
//            if(strElementName == "functionMode")
//            {
//                param.functionMode = (FunctionMode)reader.readElementText().toInt();
//            }
//            else if(strElementName == "appTitle")
//            {
//                param.appTitle = reader.readElementText();
//            }
//            else if(strElementName == "iconPath")
//            {
//                param.iconPath = reader.readElementText();
//            }
//            else if(strElementName == "backgroundImagePath")
//            {
//                param.backgroundImagePath = reader.readElementText();
//            }
//            else if(strElementName == "initPath")
//            {
//                param.initPath = reader.readElementText();
//            }
//            else if(strElementName == "onlyShowOneImage")
//            {
//                param.onlyShowOneImage = reader.readElementText().toInt()== 0 ? false:true;
//            }
//            else if(strElementName == "closeDirTreeOnOpen")
//            {
//                param.closeDirTreeOnOpen = reader.readElementText().toInt() == 0 ? false:true;
//            }
//        }
//        else if(reader.isEndElement()) {

//            QString strElementName = reader.name().toString();
//            if (QString::compare(strElementName, "user") == 0) {
//                break;  // 跳出
//            }
//        }
//    }
//}

////解析default
//void AppConfig::parseDefault(QXmlStreamReader &reader, AppConfigParameter &param)
//{
//    while (!reader.atEnd()) {

//        reader.readNext();
//        if (reader.isStartElement()) {  // 开始元素
//            QString strElementName = reader.name().toString();

//            if(strElementName == "defaultBackgroundPath")
//            {
//                param.defaultBackgroundPath = reader.readElementText();
//            }
//            else if(strElementName == "besimageBlackIcon")
//            {
//                param.besimageBlackIcon = reader.readElementText();
//            }
//            else if(strElementName == "besimageWhiteIcon")
//            {
//                param.besimageWhiteIcon = reader.readElementText();
//            }
//            else if(strElementName == "isWindowHeaderColorBlack")
//            {
//                param.isWindowHeaderColorBlack = reader.readElementText().toInt()== 0 ? false:true;
//            }
//            else if(strElementName == "bgFillMode")
//            {
//                param.bgFillMode = (BGFillMode)reader.readElementText().toInt();
//            }
//        }
//        else if(reader.isEndElement()) {

//            QString strElementName = reader.name().toString();
//            if (QString::compare(strElementName, "default") == 0) {
//                break;  // 跳出
//            }
//        }
//    }
//}


////保存到打印参数
//bool AppConfig::SaveAppConfig()
//{
//    QString confPath;
//    if(!MakeSureConfigPathAvailable(confPath))  //确保配置路径可用，并获得了路径
//        return false;

//    //打开文件
//    QFile file(confPath);
//    if (!file.open(QFile::WriteOnly | QFile::Text)) { // 只写模式打开文件
//            qDebug() << QString("Cannot write file %1(%2).").arg(confPath).arg(file.errorString());
//            return false;
//    }

//    //将配置写入xml文件
//    QXmlStreamWriter writer(&file);

//    writer.setAutoFormatting(true); // 自动格式化
//    writer.writeStartDocument("1.0", true);  // 开始文档（XML 声明）

//    writer.writeStartElement("appConfig"); // 开始元素<appConfig>

//    writer.writeStartElement("user"); // 开始元素<user>

//    writer.writeTextElement("functionMode", QString::number((int)m_param.functionMode));
//    writer.writeTextElement("appTitle", m_param.appTitle);
//    writer.writeTextElement("iconPath", m_param.iconPath);
//    writer.writeTextElement("backgroundImagePath", m_param.backgroundImagePath);
//    writer.writeTextElement("initPath", m_param.initPath);
//    writer.writeTextElement("onlyShowOneImage", QString::number(m_param.onlyShowOneImage ? 1 : 0));
//    writer.writeTextElement("closeDirTreeOnOpen", QString::number(m_param.closeDirTreeOnOpen? 1 : 0));

//    writer.writeEndElement();  // 结束子元素 </user>

//    writer.writeStartElement("default"); // 开始元素<default>

//    writer.writeTextElement("defaultBackgroundPath", m_param.defaultBackgroundPath);
//    writer.writeTextElement("besimageBlackIcon", m_param.besimageBlackIcon);
//    writer.writeTextElement("besimageWhiteIcon", m_param.besimageWhiteIcon);
//    writer.writeTextElement("isWindowHeaderColorBlack", QString::number(m_param.isWindowHeaderColorBlack ? 1 : 0));
//    writer.writeTextElement("bgFillMode", QString::number((int)m_param.bgFillMode));

//    writer.writeEndElement();  // 结束子元素 </default>

//    writer.writeEndElement();  // 结束子元素 </appConfig>

//    writer.writeEndDocument();  // 结束文档

//    file.close();  // 关闭文件

//    return true;
//}

////确保配置路径可用，返回配置路径
//bool AppConfig::MakeSureConfigPathAvailable(QString &path)
//{
//    QString StrSettingsDir = QCoreApplication::applicationDirPath() + "/settings";

//    //如果settings 目录不存在则创建目录
//    QDir SettingDir(StrSettingsDir);
//    if(!SettingDir.exists())
//    {
//        if(!SettingDir.mkpath(StrSettingsDir))
//        {
//            QMessageBox::information(nullptr,tr("提示"),tr("无法为配置创建目录")+":" + StrSettingsDir,
//                                    QMessageBox::Yes,QMessageBox::Yes);
//        }
//    }

//    //得到目标路径
//    path = StrSettingsDir + "/app.config";
//    return  true;
//}
