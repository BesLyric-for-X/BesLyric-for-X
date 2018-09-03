// LrcProcessor.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "LrcProcessor.h"
#include <QFile>

bool LrcProcessor::LoadFromFile(QString lyricFilePath)
{
    lrcLyrics.clear();

    //先读取所有文本
    QVector<QString> lines;

    bIsLyricValid = false;
    bIsLrcLyric = false;
    bIsNeteaseLrcFormat = true;

    QFile file(lyricFilePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString strline = QString(line).trimmed();

        if(strline.size() != 0)
            lines.push_back(strline);   //收集非空行
    }

    file.close();

    LoadFromRawLines(lines);

    return true;
}

bool LrcProcessor::LoadFromRawText(QString content)
{
    QStringList lineList = content.split('\n');
    QVector<QString> lines;
    for(auto line:lineList)
    {
        line = line.trimmed();
        if(!line.isEmpty())
            lines.push_back(line);
    }

    LoadFromRawLines(lines);

    return true;
}

void LrcProcessor::LoadFromRawLines(QVector<QString> lines)
{
    //进一步判断是否为LRC歌词
    if(lines.size() != 0)
    {
        bIsLyricValid = true;

        QVector<QPair<int, QString>> lrcLyricsTemp; //临时存放初步收集的可能无序的结果

        //这里判断的标准以 比较广泛的方式支持读取进来

        //网易云标准上传的歌词格式只有 带时间标签的行，而且一行一个时间 [xx:xx.xx]  xxx

        //广泛的lrc格式，可能包含 [ar:歌手名]、[ti:歌曲名]、[al:专辑名]、[by:编辑者(指lrc歌词的制作人)]、[offset:时间补偿值]
        //                    时间标签，形式为“[mm:ss]”或“[mm:ss.ff]”
        //                  一行可能包含多个时间标签

        QStringList infoLabels;     //储存非歌词行的信息 [ar:歌手名] 、[offset:时间补偿值] 等

        QRegExp rx_time("(\\[\\s*\\d+\\s*:\\s*\\d+(\\.\\d+)?\\s*\\])");  //匹配时间标签
        QRegExp rx_other("(\\[\\s*\\w+\\s*:\\s*\\w+\\s*\\])");  //匹配其他标签
        for(auto& line:lines)
        {
            QStringList timeList;

            int textPos = 0; //非标签文本开始的位置

            //尝试匹配时间标签
            int pos = 0;
            while ((pos = rx_time.indexIn(line, pos)) != -1) {
                timeList << rx_time.cap(1);
                pos += rx_time.matchedLength();

                textPos = pos;
            }

            //尝试匹配其他标签
            int pos2 = 0;
            while ((pos2 = rx_other.indexIn(line, pos2)) != -1) {
                infoLabels << rx_other.cap(1);
                pos2 += rx_other.matchedLength();

                textPos = pos2;
            }

            QString strLeft; //剩下的字符串
            strLeft = line.right(line.size()-textPos);   //得到去除可能的标签后的字符串

            for(auto& time: timeList) //只有当收集到时间标签时，才收集对应的歌词进入 lrcLyricsTemp
            {
                //将时间标签转化为 毫秒时间
                int p1 = time.indexOf('[');

                time = time.right(time.size()-p1-1);
                int p2 = time.indexOf(':');

                QString strMin = time.left(p2);

                time = time.right(time.size()-p2-1);
                int p3 = time.indexOf(']');

                QString strSec = time.left(p3);

                int min = strMin.toInt();
                double sec = strSec.toDouble();

                int msec = min * 60 * 1000 + int(sec * 1000);

                lrcLyricsTemp.push_back(QPair<int,QString>(msec,strLeft));
            }

            if(timeList.size() > 1)  //一行包含多个时间，认为不是标准的网易云音乐歌词
                bIsNeteaseLrcFormat = false;
        }

        if(lrcLyricsTemp.size() != 0)
        {
            bIsLrcLyric = true; //认为是lrc歌词

            //对收集到的 lrcLyricsTemp 根据时间，从小到大排序，置于  lrcLyrics 中

            while(lrcLyricsTemp.size()!=0)
            {
                int minTime = lrcLyricsTemp.front().first;
                auto iterDel = lrcLyricsTemp.begin();           //存放最小的要被删除的那个
                for(auto iter = lrcLyricsTemp.begin()+1; iter != lrcLyricsTemp.end(); iter++)
                {
                    if(iter->first < minTime)
                    {
                        iterDel = iter;
                        minTime = iter->first;
                    }
                }

                lrcLyrics.push_back(*iterDel);

                lrcLyricsTemp.erase(iterDel);
            }

            if(infoLabels.size() != 0)//包含时间标签外的其他类型标签，认为不是标准的网易云音乐歌词
                bIsNeteaseLrcFormat = false;

            GatherInfoMap(infoLabels);

            //查看 infoLabels中是否存在  [offset:时间补偿值]，存在则修正时间
            if(offsetVaule != 0)
            {
                for(auto& lrc: lrcLyrics)
                {
                    lrc.first += offsetVaule;
                    if(lrc.first < 0)
                        lrc.first = 0;
                }
            }

        }
        else
        {
            //找不到任何含有时间标签的行，认为是原生歌词
            bIsLrcLyric = false;

            //收集歌词到 lrcLyric 结构中，时间都置为 0
            for(auto& line:lines)
            {
                lrcLyrics.push_back(QPair<int,QString>(0,line));
            }
        }
    }

}

QVector<QPair<int, QString>> LrcProcessor::GetLrcLyric()
{
    return lrcLyrics;
}

QString LrcProcessor::ToLrcLine(QPair<int, QString> timeLine)
{
    int pos = timeLine.first;
    int ms = pos % 1000;
    pos = pos / 1000;
    int s = pos % 60;
    int m = pos / 60;

    QString timeLabel;
    timeLabel.sprintf("[%.2d:%.2d.%.3d]", m, s, ms);

    return timeLabel + timeLine.second;
}


void LrcProcessor::GatherInfoMap(QStringList infoLabels)
{
     QMap<QString,QString> infoMap;

    //查看 infoLabels中是否存在  [offset:时间补偿值]，存在则修正时间
    for(auto label:infoLabels)
    {
       int pos1 = label.indexOf('[');
       int pos2 = label.indexOf(':');
       int pos3 = label.indexOf(']');
       if(pos1 == -1|| pos2 == -1 || pos3 == -1 )
           continue;  //无效标签，继续

       QString key = label.left(pos2).right(pos2-pos1-1);
       QString value = label.right(pos3 - pos2);
       pos3 = value.indexOf(']');
       value = value.left(pos3);

       infoMap.insert(key,value);
    }

    m_strTitle = infoMap["ti"];
    m_strArtist = infoMap["ar"];
    m_strAlbum = infoMap["al"];
    m_strEditor = infoMap["by"];
    offsetVaule = infoMap["offset"].toInt();
}

////使用整个LRC文本数据初始化Lrc处理器
//LrcProcessor::LrcProcessor(SStringW AllContent, bool bDealOffset)
//{
//	//将AllContent 分割成行
//	vector<SStringW> vecLines;
//	CStringHelper::SplitStringByToLines( AllContent, L'\n', vecLines);
	
//	ProcessData(vecLines, bDealOffset);
//}

////当前歌词文件是否有效
//bool LrcProcessor::IsLrcFileValid()
//{
//	return m_bIsLrcFileValid;
//}

//获得网易云音乐支持的格式
//vector<TimeLineInfo> LrcProcessor::GetNeteaseLrc()
//{
//    return m_vecNeteaseLrc;
//}

////生成Lrc文件
//bool LrcProcessor::GenerateNeteaseLrcFile(SStringW strFilePath)
//{
//	vector<SStringW> vecLines;

//	WCHAR szTimeBuf[MAX_BUFFER_SIZE/2];
//	for(auto iter = m_vecNeteaseLrc.begin(); iter != m_vecNeteaseLrc.end(); iter++)
//	{
//		int ms = iter->m_nmSesonds;

//		szTimeBuf[0] = L'\0';
//		int minutes = ms/60000;
//		ms = ms%60000;
//		double seconds = ms*1.0/1000;
//		_stprintf(szTimeBuf,_T("[%02d:%06.3lf]"),minutes,seconds);

//		SStringW newLine = szTimeBuf;
//		newLine.Append( iter->m_strLine );
//		newLine.Append(L"\n");

//		vecLines.push_back(newLine);
//	}

//	return FileOperator::WriteToUtf8File( strFilePath.GetBuffer(1), vecLines);
//}


////处理 vector 行数据
//void LrcProcessor::ProcessData(const vector<SStringW>& vecLines, bool bDealOffset)
//{
//	m_bDealOffset = bDealOffset;
//	m_strTitle = L"";
//	m_strArtist = L"";
//	m_strAlbum = L"";
//	m_strEditor = L"";
//	m_nOffset = 0;

//	m_bIsLrcFileValid = true;

//	//遍历处理所有行
//	for(auto iter = vecLines.begin(); iter != vecLines.end(); iter++)
//	{
//		if( false == DealOneLine( *iter ) )
//		{
//			m_bIsLrcFileValid = false;
//			return;
//		}
//	}

//	//对所有歌词进行排序
//	sort(m_vecNeteaseLrc.begin(), m_vecNeteaseLrc.end());

//	//去除重复的空行，连续出现多个空行时，保留第一个空行的时间
//	bool isLastLineEmpty = false;
//	for(auto iter = m_vecNeteaseLrc.begin(); iter != m_vecNeteaseLrc.end(); )
//	{
//		if(iter->m_strLine.GetLength() == 0)
//		{
//			if(isLastLineEmpty)
//			{
//				iter = m_vecNeteaseLrc.erase(iter);
//			}
//			else
//			{
//				iter++;
//				isLastLineEmpty = true;
//			}
//		}
//		else
//		{
//			iter++;
//			isLastLineEmpty = false;
//		}
//	}

//	//对整体的时间进行偏移
//	if(m_bDealOffset)
//	{
//		for(auto iter = m_vecNeteaseLrc.begin(); iter != m_vecNeteaseLrc.end(); iter++)
//		{
//			iter->m_nmSesonds = iter->m_nmSesonds - m_nOffset < 0 ? 0 : iter->m_nmSesonds - m_nOffset;
//		}
//	}
//}

////处理1行
//bool LrcProcessor::DealOneLine(const SStringW& _strLine)
//{
//	SStringW strLine = _strLine;
//	int nPos1 = -1;
//	int nPos2 = -1;
//	if( -1 != strLine.Find(L"[ti:"))//标题
//	{
//		nPos1 = strLine.Find(L":");
//		nPos2 = strLine.Find(L"]");
//		m_strTitle = strLine.Left(nPos2);
//		m_strTitle = m_strTitle.Right(m_strTitle.GetLength()-1 - nPos1);
//	}
//	else if( -1 != strLine.Find(L"[ar:"))//艺术家
//	{
//		nPos1 = strLine.Find(L":");
//		nPos2 = strLine.Find(L"]");
//		m_strArtist = strLine.Left(nPos2);
//		m_strArtist = m_strArtist.Right(m_strArtist.GetLength()-1 - nPos1);
//	}
//	else if( -1 != strLine.Find(L"[al:"))//专辑
//	{
//		nPos1 = strLine.Find(L":");
//		nPos2 = strLine.Find(L"]");
//		m_strAlbum = strLine.Left(nPos2);
//		m_strAlbum = m_strAlbum.Right(m_strAlbum.GetLength()-1 - nPos1 );
//	}
//	else if( -1 != strLine.Find(L"[by:"))//歌词制作者
//	{
//		nPos1 = strLine.Find(L":");
//		nPos2 = strLine.Find(L"]");
//		m_strEditor = strLine.Left(nPos2);
//		m_strEditor = m_strEditor.Right(m_strEditor.GetLength()-1 - nPos1);
//	}
//	else if( -1 != strLine.Find(L"[offset:"))
//	{
//		nPos1 = strLine.Find(L":");
//		nPos2 = strLine.Find(L"]");
//		SStringW m_strOffset = strLine.Left(nPos2);
//		m_strOffset = m_strOffset.Right(m_strOffset.GetLength()-1 - nPos1);
//		m_nOffset = _wtoi( m_strOffset.GetBuffer(1));
//	}
//	else //其他情况认为是标记时间的行
//	{
//		wstring sLine = strLine.GetBuffer(1);
//		//分析是否为有效的时间标签行（排除其他自定义标签，对没有时间标签的歌词，添加时间[0:0.000]兼容处理）
//		if(!MakeSureTimeLineValid(sLine))
//			return true;

//		wstring sTimeLabel = L"";
//		wstring sLyric = L"";
//		int nPosColon = -1;
//		sLyric = sLine.substr( sLine.find_last_of(L']') + 1);
//		do{
//			nPos1 = sLine.find_first_of(L'[');
//			nPosColon =  sLine.find_first_of(L':');
//			nPos2 =  sLine.find_first_of(L']');
//			if(nPos1 ==0 && nPos1 < nPosColon && nPosColon < nPos2) // strLine头还有时间标签
//			{
//				sTimeLabel = sLine.substr(nPos1, nPos2+1);

//				TimeLineInfo timeInfoLine(SStringW( (sTimeLabel + sLyric).c_str()));

//				m_vecNeteaseLrc.push_back(timeInfoLine); //添加一行到网易云歌词

//				sLine = sLine.substr(nPos2+1); //去除前面标签
//			}
//			else
//				break;
//		}
//		while( true);
//	}

//	return true;
//}

////按歌词时间升序比较
//bool LrcProcessor::CompareWithIncreaceTime(const TimeLineInfo & L1, const TimeLineInfo & L2)
//{
//	return L1.m_nmSesonds < L2.m_nmSesonds;
//}

////保证时间行有效（无法保证则返回 false）
//bool LrcProcessor::MakeSureTimeLineValid(wstring& line)
//{
//	wstring strLine = line;
//	//查看是否有标签 [:]
//	size_t pos1 = strLine.find_first_of(L'[');
//	size_t pos2 = strLine.find_first_of(L':');
//	size_t pos3 = strLine.find_first_of(L']');
	
//	//对于没有标签的情况，当成歌词，添加 [0:0.000] 时间
//	if(pos1 == wstring::npos && pos2 == wstring::npos && pos2 == wstring::npos )
//	{
//		if(CStringHelper::GetTrimLength(line, L" \t\r\n") == 0)//为空串
//		{
//			return false;
//		}

//		//在最后的时候 strLine 可能没有 \n,添加一个回车符号
//		strLine = CStringHelper::Trim(strLine, L"\r\n") + L"\n";

//		line = L"[0:0.000]" + strLine;
//		return true;
//	}

//	if( pos1 == wstring::npos || pos2 == wstring::npos || pos2 == wstring::npos ||
//		pos1 > pos2 || pos1 > pos3 || pos2 > pos3 ||
//		pos1 +1 == pos2 || pos2+1 == pos3
//		)	//非法行，无法处理
//		return false;

//	//存在标签，自定义和无效时间标签都 无法继续处理；只有有效的时间标签，才可以
//	wstring strMinute = strLine.substr( pos1+1,pos2 - pos1 -1);
//	wstring strSecond = strLine.substr( pos2+1,pos3 - pos2 -1);
//	bool bValid = true;
//	for(auto i = strMinute.begin(); i != strMinute.end(); i++)
//	{
//		if(!iswdigit(*i))
//		{
//			bValid = false;
//			return false;	//无效
//		}
//	}

//	// 处理 秒 可能是 dd.ddd 或 dd 的形式
//	bool HasMeetDot = false;
//	for(auto i = strSecond.begin(); i != strSecond.end(); i++)
//	{
//		if(!HasMeetDot)
//		{
//			if(iswdigit(*i))
//				continue;
//			else if( *i == L'.')
//			{
//				HasMeetDot = true;
//			}
//		}
//		else
//		{
//			if(!iswdigit(*i))
//			{
//				bValid = false;
//				return false;	//无效
//			}
//		}
//	}
		
//	//在最后的时候 strLine 可能没有 \n,添加一个回车符号
//	line = CStringHelper::Trim(strLine, L"\r\n") + L"\n";

//	return true;
//}
