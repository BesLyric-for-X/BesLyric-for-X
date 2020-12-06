#include <QStandardPaths>

#include "SettingManager.h"
#include "BesMessageBox.h"

SettingManager &SettingManager::GetInstance()
{
    static SettingManager* instance = nullptr;
    if(instance == nullptr)
        instance = new SettingManager();

    return *instance;
}

void SettingManager::loadSettingData()
{
    QString path = MakeSureConfigPathAvailable();
    if(path.size() == 0)
        return;

    if(!QFile::exists(path))  //不存在，自动创建设置并保存
    {
        if(!saveSettingData())
            BesMessageBox::information(tr("提示"), tr("尝试创建默认设置失败，可能是程序没有写权限"));
    }
    else //存在，读取配置
    {
        if (!LoadSettingData(path))
        {
            BesMessageBox::information(tr("提示"), tr("载入设置失败，可能是程序没有写权限")
                     + " : \n\n" + path + "\n\n"+ "将自动为您重建默认设置 :)");

            //自动重写
            if (!saveSettingData())
                BesMessageBox::information(tr("提示"), tr("尝试创建默认设置失败 :("));
        }
    }

    preProcessSetting();
}

bool SettingManager::saveSettingData()
{
    QString path = MakeSureConfigPathAvailable();
    if(path.size() == 0)
        return false;

    //打开文件
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Text)) { // 只写模式打开文件
            qDebug() << QString("Cannot write file %1(%2).").arg(path).arg(file.errorString());
            return false;
    }

    {
        //将配置写入xml文件
        QXmlStreamWriter writer(&file);

        writer.setAutoFormatting(true); // 自动格式化
        writer.writeStartDocument("1.0", true);  // 开始文档（XML 声明）

        writer.writeStartElement("settings"); // 开始元素<settings>

        writer.writeStartElement("lyricMaker");
        writer.writeTextElement("shiftTime",QString().number(settingData.shiftTime));
        writer.writeEndElement();

        writer.writeStartElement("defaultPath");
        writer.writeTextElement("music",settingData.defaultMusicPath);
        writer.writeTextElement("lyric",settingData.defaultLyricPath);
        writer.writeTextElement("output",settingData.defaultOutputPath);
        writer.writeEndElement();


        writer.writeStartElement("musicDownload");
        writer.writeTextElement("path",settingData.musicDowloadPath);
        writer.writeTextElement("agreeDeclaration",QString().number(settingData.agreeDownloadDeclaration ? 1:0));
        writer.writeTextElement("nameFormatStyle",QString().number((int)settingData.nameFormatStyle));
        writer.writeEndElement();


        writer.writeStartElement("login");
        writer.writeTextElement("anonymous",QString().number(settingData.loginAnonymously ? 1:0));
        writer.writeEndElement();

        writer.writeStartElement("upgrade");
        writer.writeTextElement("autoCheck",QString().number(settingData.autoCheckForUpgrade ? 1:0));
        writer.writeEndElement();


        writer.writeStartElement("other");
        writer.writeTextElement("skinName",settingData.skinName);
        writer.writeTextElement("volume",QString().number(settingData.volume));
        writer.writeTextElement("isMute",QString().number(settingData.isMute?1:0));
        writer.writeTextElement("playMode",QString().number(settingData.playMode));
        writer.writeEndElement();

        writer.writeEndElement();  // 结束子元素 </settings>

        writer.writeEndDocument();  // 结束文档

        file.close();  // 关闭文件
    }

    return true;
}


SettingData &SettingManager::data()
{
    return settingData;
}

QString SettingManager::SettingManager::MakeSureBaseDataPathAvailable()
{
    QString StrDataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    //如果settings 目录不存在则创建目录
    QDir DataDir(StrDataDir);
    if(!DataDir.exists())
    {
        if(!DataDir.mkpath(StrDataDir))
        {
            BesMessageBox::information(tr("提示"),tr("无法为配置创建目录")+":" + StrDataDir);
            return "";
        }
    }

    //得到目标路径
    return StrDataDir;
}


void SettingManager::loadFromDataDir()
{
    QString path = MakeSureConfigPathAvailable();
    if(path.size() == 0)
        return;

    if(!QFile::exists(path))  //不存在，自动创建设置，并保存
    {
        if(!saveSettingData())
            BesMessageBox::information(tr("提示"), tr("尝试创建默认设置失败"));
    }
    else //存在，读取配置
    {
        if (!LoadSettingData(path))
        {
            BesMessageBox::information(tr("提示"), tr("载入设置失败")
                                       + " : \n\n" + path + "\n\n"+ "将自动为您重建默认设置 :)");

            //自动重写
            if (!saveSettingData())
                BesMessageBox::information(tr("提示"), tr("尝试创建默认设置失败 ，可能是程序没有写权限:("));
		}
    }

}

bool SettingManager::LoadSettingData(QString filePath)
{
    //打开文件
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) { // 只读模式打开文件
            qDebug() << QString("Cannot read file %1(%2).").arg(filePath).arg(file.errorString());
            return false;
    }


    //读取示例参考: https://blog.csdn.net/liang19890820/article/details/52808829

    //将配置从xml文件读出
    QXmlStreamReader reader(&file);

    QString strElementName = "";

    // 解析 XML，直到结束
    while (!reader.atEnd()) {

        // 读取下一个元素
        QXmlStreamReader::TokenType nType = reader.readNext();

        if (nType == QXmlStreamReader::StartElement)
        {
            strElementName = reader.name().toString();
            if (QString::compare(strElementName, "settings") == 0) // 根元素
            {
                parseAll(reader);
            }
        }
    }

    if (reader.hasError()) {  // 解析出错
       qDebug() << QObject::tr("错误信息：%1  行号：%2  列号：%3  字符位移：%4").arg(reader.errorString()).arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.characterOffset());
       return false;
    }

    file.close();  // 关闭文件

    return true;
}

bool SettingManager::parseAll(QXmlStreamReader &reader)
{
    while (!reader.atEnd()) {

        reader.readNext();
        if (reader.isStartElement()) {  // 开始元素
            QString strElementName = reader.name().toString();
            if(strElementName == "lyricMaker")
            {
                 while (!reader.atEnd())
                 {
                     reader.readNext();
                     QString strElementName = reader.name().toString();
                   if (reader.isStartElement()) {  // 开始元素
                       if(strElementName == "shiftTime")
                       {
                           settingData.shiftTime = reader.readElementText().toInt();
                       }
                   }
                   else if(reader.isEndElement()) {

                       QString strElementName = reader.name().toString();
                       if (QString::compare(strElementName, "lyricMaker") == 0) {
                           break;  // 跳出
                       }
                   }
                }
            }
            else if(strElementName == "defaultPath")
            {
                while (!reader.atEnd())
                {
                    reader.readNext();
                    QString strElementName = reader.name().toString();
                  if (reader.isStartElement()) {  // 开始元素
                      if(strElementName == "music")
                      {
                          settingData.defaultMusicPath = reader.readElementText();
                      }
                      else if (strElementName == "lyric")
                      {
                          settingData.defaultLyricPath = reader.readElementText();
                      }
                      else if (strElementName == "output")
                      {
                          settingData.defaultOutputPath = reader.readElementText();
                      }
                  }
                  else if(reader.isEndElement()) {

                      QString strElementName = reader.name().toString();
                      if (QString::compare(strElementName, "defaultPath") == 0) {
                          break;  // 跳出
                      }
                  }
               }
            }
            else if(strElementName == "musicDownload")
            {
                while (!reader.atEnd())
                {
                    reader.readNext();
                    QString strElementName = reader.name().toString();
                  if (reader.isStartElement()) {  // 开始元素
                      if(strElementName == "path")
                      {
                          settingData.musicDowloadPath = reader.readElementText();
                      }
                      else if (strElementName == "agreeDeclaration")
                      {
                          settingData.agreeDownloadDeclaration = reader.readElementText().toInt() == 1;
                      }
                      else if(strElementName == "nameFormatStyle")
                      {
                          settingData.nameFormatStyle = (NameFormatStyle)reader.readElementText().toInt();
                      }
                  }
                  else if(reader.isEndElement()) {

                      QString strElementName = reader.name().toString();
                      if (QString::compare(strElementName, "musicDownload") == 0) {
                          break;  // 跳出
                      }
                  }
               }
            }
            else if(strElementName == "login")
            {
                while (!reader.atEnd())
                {
                    reader.readNext();
                    QString strElementName = reader.name().toString();
                  if (reader.isStartElement()) {  // 开始元素
                      if(strElementName == "anonymous")
                      {
                          settingData.loginAnonymously = reader.readElementText().toInt() == 1;
                      }
                  }
                  else if(reader.isEndElement()) {

                      QString strElementName = reader.name().toString();
                      if (QString::compare(strElementName, "login") == 0) {
                          break;  // 跳出
                      }
                  }
               }
            }
            else if(strElementName == "upgrade")
            {
                while (!reader.atEnd())
                {
                    reader.readNext();
                    QString strElementName = reader.name().toString();
                  if (reader.isStartElement()) {  // 开始元素
                      if(strElementName == "autoCheck")
                      {
                          settingData.autoCheckForUpgrade = reader.readElementText().toInt() == 1;
                      }
                  }
                  else if(reader.isEndElement()) {

                      QString strElementName = reader.name().toString();
                      if (QString::compare(strElementName, "upgrade") == 0) {
                          break;  // 跳出
                      }
                  }
               }
            }
            else if(strElementName == "other")
            {
                while (!reader.atEnd())
                {
                    reader.readNext();
                    QString strElementName = reader.name().toString();
                  if (reader.isStartElement()) {  // 开始元素
                      if(strElementName == "skinName")
                      {
                          settingData.skinName = reader.readElementText();
                      }
                      else if(strElementName == "volume")
                      {
                          settingData.volume = reader.readElementText().toInt();
                      }
                      else if(strElementName == "isMute")
                      {
                          settingData.isMute = reader.readElementText().toInt()== 1;
                      }
                      else if(strElementName == "playMode")
                      {
                          settingData.playMode = reader.readElementText().toInt();
                      }
                  }
                  else if(reader.isEndElement()) {

                      QString strElementName = reader.name().toString();
                      if (QString::compare(strElementName, "other") == 0) {
                          break;  // 跳出
                      }
                  }
               }
            }
        }
        else if(reader.isEndElement()) {

            QString strElementName = reader.name().toString();
            if (QString::compare(strElementName, "settings") == 0) {
                break;  // 跳出
            }
        }
    }

    return true;
}


QString SettingManager::MakeSureConfigPathAvailable()
{
    QString StrDataDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    //如果settings 目录不存在则创建目录
    QDir DataDir(StrDataDir);
    if(!DataDir.exists())
    {
        if(!DataDir.mkpath(StrDataDir))
        {
            BesMessageBox::information(tr("提示"),tr("无法为配置创建目录")+":" + StrDataDir);
            return "";
        }
    }

    //得到目标路径
    return StrDataDir + "/setting.xml";
}

void SettingManager::preProcessSetting()
{
    //如果不同意下载声明，下载路径置空
    if(!settingData.agreeDownloadDeclaration)
        settingData.musicDowloadPath = "";
}

