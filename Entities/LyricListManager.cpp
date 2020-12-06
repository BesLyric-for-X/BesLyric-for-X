#include <QStandardPaths>

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
        loadFromDataDir();

    return listData;
}

bool LyricListManager::saveLyricListData(LyricListData data)
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

        writer.writeStartElement("lyricList"); // 开始元素<lyricList>

        writer.writeStartElement("history"); // 开始元素<history>

        for(int j = 0; j < data.listsHistory.size(); j++)
        {
            writer.writeStartElement("list"); // 开始元素<list>

            writer.writeAttribute("name", data.listsHistory[j].name);

            for(int i = 0; i<data.listsHistory[j].items.size(); i++)
            {
				writer.writeStartElement("item"); // 开始元素<item>
                writer.writeTextElement("song",data.listsHistory[j].items[i].song);
                writer.writeTextElement("lyric",  data.listsHistory[j].items[i].lyric);
				writer.writeEndElement();  // 结束子元素 </item>
            }

            writer.writeEndElement();  // 结束子元素 </list>
        }

        writer.writeEndElement();  // 结束子元素 </history>


        writer.writeStartElement("created"); // 开始元素<created>

        for(int j = 0; j < data.listsCreated.size(); j++)
        {
            writer.writeStartElement("list"); // 开始元素<list>

            writer.writeAttribute("name", data.listsCreated[j].name);

            for(int i = 0; i<data.listsCreated[j].items.size(); i++)
            {
				writer.writeStartElement("item"); // 开始元素<item>
                writer.writeTextElement("song", data.listsCreated[j].items[i].song);
                writer.writeTextElement("lyric",  data.listsCreated[j].items[i].lyric);
				writer.writeEndElement();  // 结束子元素 </item>
            }

            writer.writeEndElement();  // 结束子元素 </list>
        }

        writer.writeEndElement();  // 结束子元素 </created>

        writer.writeEndElement();  // 结束子元素 </lyricList>

        writer.writeEndDocument();  // 结束文档

        file.close();  // 关闭文件
    }

    //成功保存后，赋值给listData
    listData = data;
    return true;
}


void LyricListManager::loadFromDataDir()
{
    QString path = MakeSureConfigPathAvailable();
    if(path.size() == 0)
        return;

    if(!QFile::exists(path))  //不存在，自动创建歌词单文件，并保存
    {
        if(listData.listsHistory.size()==0)
        {
            LyricList lyricList;
            lyricList.name = tr("制作记录");
            listData.listsHistory.push_back(lyricList);
        }

        if(!saveLyricListData(listData))
            BesMessageBox::information(tr("提示"), tr("尝试创建默认歌词单文件失败，可能是程序没有写权限"));
    }
    else //存在，读取配置
    {
		if (!LoadListData(path))
		{
            BesMessageBox::information(tr("提示"), tr("载入歌词单文件失败")
                     + " : \n\n" + path + "\n\n"+ "将自动为您重建默认歌词单 :)");

            if(listData.listsHistory.size()==0)
            {
                LyricList lyricList;
                lyricList.name = tr("制作记录");
                listData.listsHistory.push_back(lyricList);
            }

			//自动重写
			if (!saveLyricListData(listData))
                BesMessageBox::information(tr("提示"), tr("尝试创建默认歌词单文件失败，可能是程序没有写权限 :("));
		}
    }

    bDataLoaded = true;
}

bool LyricListManager::LoadListData(QString filePath)
{
    //打开文件
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) { // 只读模式打开文件
            qDebug() << QString("Cannot read file %1(%2).").arg(filePath).arg(file.errorString());
            return false;
    }

    LyricListData lyricListData;

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
            if (QString::compare(strElementName, "lyricList") == 0) // 根元素
            {
                parseAll(reader, lyricListData);
            }
        }
    }

    if (reader.hasError()) {  // 解析出错
       qDebug() << QObject::tr("错误信息：%1  行号：%2  列号：%3  字符位移：%4").arg(reader.errorString()).arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.characterOffset());
       return false;
    }

    file.close();  // 关闭文件

    listData = lyricListData;


    return true;
}

bool LyricListManager::parseAll(QXmlStreamReader &reader, LyricListData &data)
{
    while (!reader.atEnd()) {

        reader.readNext();
        if (reader.isStartElement()) {  // 开始元素
            QString strElementName = reader.name().toString();
            if(strElementName == "history")
            {
                QVector<LyricList> lists;
                if(!parseLyricList(reader, lists,"history"))
                    return false;

                data.listsHistory = lists;
            }
            else if(strElementName == "created")
            {
                QVector<LyricList> lists;
                if(!parseLyricList(reader, lists, "created"))
                    return false;

                data.listsCreated = lists;
            }
        }
        else if(reader.isEndElement()) {

            QString strElementName = reader.name().toString();
            if (QString::compare(strElementName, "lyricList") == 0) {
                break;  // 跳出
            }
        }
    }

    return true;
}

bool LyricListManager::parseLyricList(QXmlStreamReader &reader, QVector<LyricList> &lists, QString parentName)
{
    lists.clear();
    while (!reader.atEnd()) {

        reader.readNext();
        if (reader.isStartElement()) {  // 开始元素
            QString strElementName = reader.name().toString();
            if(strElementName == "list")
            {
                LyricList lyricList;

                //获取列名
                QXmlStreamAttributes attributes = reader.attributes();
                if (attributes.hasAttribute("name")) {
                    QString listName = attributes.value("name").toString();
                    lyricList.name = listName;
                    qDebug() << QString::fromLocal8Bit("attribute: name(%1)").arg(listName);
                }
                else
                    return false;

                //获取具体项
                while (!reader.atEnd()) {
                    reader.readNext();
                    if (reader.isStartElement()) {  // 开始元素
                        QString strElementName = reader.name().toString();
                        if(strElementName == "item")
                        {
                            LyricListItem item;

                            //收集单个项
                            while (!reader.atEnd()){
                                  reader.readNext();
                                  if (reader.isStartElement()) {  // 开始元素
                                      QString strElementName = reader.name().toString();
                                      if(strElementName == "song")
                                      {
                                          item.song = reader.readElementText();
                                      }
                                      else if(strElementName == "lyric")
                                      {
                                          item.lyric = reader.readElementText();
                                      }
                                  }
                                  else if(reader.isEndElement()) {

                                      QString strElementName = reader.name().toString();
                                      if (QString::compare(strElementName, "item") == 0) {
                                          break;  // 跳出
                                      }
                                  }
                            }

                            lyricList.items.push_back(item);
                        }
                    }
                    else if(reader.isEndElement()) {

                        QString strElementName = reader.name().toString();
                        if (QString::compare(strElementName, "list") == 0) {
                            break;  // 跳出
                        }
                    }
                }

                lists.push_back(lyricList);
            }
        }
        else if(reader.isEndElement()) {

            QString strElementName = reader.name().toString();
            if (QString::compare(strElementName, parentName) == 0) {
                break;  // 跳出
            }
        }
    }

    return true;
}


QString LyricListManager::MakeSureConfigPathAvailable()
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
    return StrDataDir + "/lyricList.xml";
}

