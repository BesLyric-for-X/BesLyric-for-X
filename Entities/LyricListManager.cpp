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

bool LyricListManager::saveLyricListData(LyricListData data, QString &errorMessage)
{
    QString strData;

    {
        QXmlStreamWriter writer(&strData);

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
    }

    //将配置写入xml文件
    if (!configurationFile.writeToFile(strData, errorMessage)){
        return false;
    }

    //成功保存后，赋值给listData
    listData = data;
    return true;
}

bool LyricListManager::loadFromDataDir(QString &errorMessage)
{
    if(!configurationFile.Exists())  //不存在，自动创建歌词单文件，并保存
    {
        if(listData.listsHistory.size()==0)
        {
            LyricList lyricList;
            lyricList.name = tr("制作记录");
            listData.listsHistory.push_back(lyricList);
        }

        return saveLyricListData(listData, errorMessage);
    }
    else //存在，读取配置
    {
        if (!LoadListData(errorMessage))
		{
            BesMessageBox::information(tr("提示"), tr("载入歌词单文件失败：%1\n%2\n将自动为您重建默认歌词单").arg(configurationFile.filePath()).arg(errorMessage));

            if(listData.listsHistory.size()==0)
            {
                LyricList lyricList;
                lyricList.name = tr("制作记录");
                listData.listsHistory.push_back(lyricList);
            }

			//自动重写
            if (!saveLyricListData(listData, errorMessage)){
                BesMessageBox::information(tr("提示"), tr("创建默认歌词单文件失败：%1").arg(errorMessage));
            }
		}
    }

    bDataLoaded = true;
}

bool LyricListManager::LoadListData(QString &errorMessage)
{
    QString strData;

    //打开文件
    if (!configurationFile.readFromFile(strData, errorMessage)){
        return false;
    }

    LyricListData lyricListData;

    //读取示例参考: https://blog.csdn.net/liang19890820/article/details/52808829

    //将配置从字符串读出
    QXmlStreamReader reader(strData);

    QString strElementName;

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
        errorMessage = tr("错误信息：%1  行号：%2  列号：%3  字符位移：%4").arg(reader.errorString()).arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.characterOffset());
        return false;
    }

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

