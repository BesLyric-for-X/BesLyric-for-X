#ifndef UNICODE_READER_H
#define UNICODE_READER_H

#include "global.h"
#include <QObject>
#include <QString>
#include <QTextCodec>
#include <QFile>
#include <QDebug>

class UnicodeReader
{
    enum encoding_type			//文件编码类型
    {
        ENCODING_ASCII = 0,
        ENCODING_UNICODE_LITTLE_ENDIAN,
        ENCODING_UNICODE_BIG_ENDIAN,
        ENCODING_UTF_8,
        ENCODING_UTF_8_BOM,
        ENCODING_OTHER			//实际处理中，ENCODING_OTHER 也被作为 ENCODING_ASCII 处理，ENCODING_OTHER 基本未被使用
    };


public:
    bool ReadFromFile(QString filePath, QString& content)
    {
        QFile file(filePath);

        if(!file.open(QIODevice::ReadOnly))
            return false;

        content = ReadUnicodeString(file.read(file.bytesAvailable()));

        file.close();

        return true;
    }

    static QString ReadUnicodeString(const QByteArray &ba)
    {
        encoding_type etype = GetFileBufferEncodingType(ba.data(), ba.size());

        QTextCodec *codec;

        switch (etype) {
        case ENCODING_UNICODE_LITTLE_ENDIAN :codec =QTextCodec::codecForName("UTF-16LE"); break;
        case ENCODING_UNICODE_BIG_ENDIAN :codec =QTextCodec::codecForName("UTF-16BE"); break;
        case ENCODING_UTF_8 :
        case ENCODING_UTF_8_BOM :codec =QTextCodec::codecForName("UTF-8"); break;
        case ENCODING_ASCII:
        default:
            codec = QTextCodec::codecForLocale();
            break;
        }

        QTextCodec::ConverterState state;
        QString text = codec->toUnicode( ba.constData(), ba.size(), &state);

        //去掉最后可能存在的 \u0000
        int countOfEndZero = 0;
        for(int i=text.size()-1;i >=0; i--)
        {
            if(text[i] != '\0')
                break;
            else
                countOfEndZero++;
        }
        if(countOfEndZero != 0)
            text.truncate(text.size()-countOfEndZero);

        return text;
    }

private:
    static encoding_type GetFileBufferEncodingType(const char* buf, int nSize)
    {
        if (nSize < 2)
            return ENCODING_ASCII; //文件内容太少，直接当做是 ascii 码处理

        unsigned char firstByte = buf[0];
        unsigned char secondByte = buf[1];

        encoding_type type;

        if (firstByte == 0xef && secondByte == 0xbb)
            type = ENCODING_UTF_8_BOM;
        else if (firstByte == 0xff && secondByte == 0xfe)
            type = ENCODING_UNICODE_LITTLE_ENDIAN;
        else if (firstByte == 0xfe && secondByte == 0xff)
            type = ENCODING_UNICODE_BIG_ENDIAN;
        else
        {
            //测试字节数：最多测验1000个字节
            int nTestLen = nSize < 1000 ? nSize : 1000;

            //测试是否是 utf-8 无bom格式
            if (IsUTF8WithNoBom(buf, nTestLen)) //如果是纯粹0-127字符，ascii也会被判定为 utf-8编码，不过不影响
                type = ENCODING_UTF_8;
            else
                type = ENCODING_ASCII;	//默认ascii 码和其他编码，以ascii 方式处理
        }

        return type;
    }

    static bool IsUTF8WithNoBom(const void* pBuffer, long size)
    {
         //参考 http://blog.csdn.net/bladeandmaster88/article/details/54767487

         bool IsUTF8 = true;
         unsigned char* start = (unsigned char*)pBuffer;
         unsigned char* end = (unsigned char*)pBuffer + size;
         while (start < end)
         {
             if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符
             {
                 start++;
             }
             else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
             {
                 IsUTF8 = false;
                 break;
             }
             else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符
             {
                 if (start >= end - 1)
                     break;
                 if ((start[1] & (0xC0)) != 0x80)
                 {
                     IsUTF8 = false;
                     break;
                 }
                 start += 2;
             }
             else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符
             {
                 if (start >= end - 2)
                     break;
                 if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
                 {
                     IsUTF8 = false;
                     break;
                 }
                 start += 3;
             }
             else
             {
                 IsUTF8 = false;
                 break;
             }
         }
         return IsUTF8;
    }
};

#endif //UNICODE_READER_H
