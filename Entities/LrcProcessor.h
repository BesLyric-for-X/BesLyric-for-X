/**
* Describe    定义处理LRC 文件的相关类
*/
#pragma once

#include <QString>
#include <QVector>
#include <QPair>
#include <QMap>

class LrcProcessor{
public:

    bool LoadFromFile(QString filePath);

    bool LoadFromRawText(QString content);

    void LoadFromRawLines(QVector<QString> lines);

    QVector<QPair<int, QString>> GetLrcLyric();

    static QString ToLrcLine(QPair<int, QString> timeLine);

//	//使用读取的的文件的所有行初始化Lrc处理器
//	LrcProcessor(vector<SStringW> vecLines, bool bDealOffset = true);

//	//使用整个LRC文本数据初始化Lrc处理器
//	LrcProcessor(SStringW AllContent, bool bDealOffset = true);

//	//当前歌词文件是否有效
//	bool IsLrcFileValid();

    //获得网易云音乐支持的格式
    //vector<TimeLineInfo> GetNeteaseLrc();

//	//生成Lrc文件
//	bool GenerateNeteaseLrcFile(SStringW strFilePath);

private:



//	//处理 vector 行数据
//	void ProcessData(const vector<SStringW>& vecLines, bool bDealOffset = true);

//	//处理1行
//	bool DealOneLine(const SStringW& strLine);

//	//按歌词时间升序比较
//	bool CompareWithIncreaceTime(const TimeLineInfo & L1, const TimeLineInfo & L2);

//	//保证时间行有效（无法保证则返回 false）
//	bool MakeSureTimeLineValid(wstring& line);

    void GatherInfoMap(QStringList infoLabels);

public:
    QString	m_strTitle;					/* 歌曲标题 */
    QString	m_strArtist;				/* 艺术家 */
    QString	m_strAlbum;					/* 专辑 */
    QString	m_strEditor;				/* 编辑的人 */
    int offsetVaule;

    QVector<QPair<int, QString>> lrcLyrics;


public:
    bool bIsLyricValid;
    bool bIsLrcLyric;
    bool bIsNeteaseLrcFormat;

//private:
//	bool m_bDealOffset;						/* 表示是否处理LRC文件中的时间偏移 */
//    int		m_nOffset;                      /* 时间偏移量，为正数表示整体提前 */

};

