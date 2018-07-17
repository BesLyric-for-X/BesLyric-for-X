#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

#include <QPixmap>
#include "filter.h"

//filter.h 实现原理详见：https://blog.csdn.net/markl22222/article/details/10313565

class ImageFilter
{
public:
    ///////////////////////
    /// \brief BlurImage        将图片模糊处理
    /// \param pixmap           要处理的图片
    /// \param blurRadius       模糊半径，值越大越模糊
    /// \param qualityPecentage 质量比例，1~100 的整数，超出范围自动归为边缘值; 实质是压缩后输入算法然后再拉伸
    /// \param blurAlgorithom   使用的算法，1、均值 2、线性、3、高斯
    /// \return
    ///
    static QPixmap BlurImage(QPixmap pixmap, int blurRadius, int qualityPecent, int blurAlgorithom = 3)
    {
        if(pixmap.isNull())
            return pixmap;

        if(qualityPecent < 1) qualityPecent =  1;
        if(qualityPecent > 100) qualityPecent = 100;

        //质量压缩
        QPixmap _image = pixmap;
        QSize originalSize = _image.size();

        if(qualityPecent != 100)
        {
            int width = pixmap.width();
            int widthScaled = int(width * ( 1.0 * qualityPecent / 100));
            if(widthScaled < 1)
                widthScaled = 1;

            _image = pixmap.scaledToWidth(widthScaled);
        }

        //设定采用的算法和 一维二维选择
        filter::pair_t pair;

        pair.blur = filter::Blur1D;  // 使用一维的就好，这里不开放 filter::Blur2D，太慢了

        switch(blurAlgorithom)
        {
        case 1:
            pair.mark = filter::Average;
            break;
        case 2:
            pair.mark = filter::Linear;
            break;
        default:
            pair.mark = filter::Gauss;
            break;
        }

        //执行计算
        filter::bitmap_t bmp;

        QImage image(_image.toImage());
        QImage imageRGB888 = image.convertToFormat(QImage::Format_RGB888); // 注意： imageRGB888 每行的btyes数不一定是3的倍数

        //数据拷贝到堆
        unsigned int width, height;
        width = imageRGB888.width();
        height = imageRGB888.height();
        unsigned char* buffer = new unsigned char[width * height * 3];

        for(unsigned int i=0; i< height; i++)
        {
             unsigned char* line = imageRGB888.scanLine(i);
             memcpy(buffer + i* width *3, line, width * 3);
        }

        //开始计算
        bmp.set((filter::bitmap_t::pixel_t*)buffer,
                    imageRGB888.width(), imageRGB888.height());

        filter::Filter(pair, bmp, blurRadius);

        //将计算后的结果写回去图片中
        for(unsigned int i=0; i< height; i++)
        {
             unsigned char* line = imageRGB888.scanLine(i);
             memcpy(line,buffer + i* width *3, width * 3);
        }

        delete[] buffer;

        //恢复原来的尺寸
        return QPixmap::fromImage(imageRGB888.scaled(originalSize));
    }


};

#endif //IMAGE_FILTER_H
