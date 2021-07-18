#ifndef APPHELPER_H
#define APPHELPER_H
#include <QString>
#include <QFile>
#include <QApplication>
#include <QColor>
#include <QPalette>
#include "SkinFactory/SkinFactory.h"
#include <QSharedPointer>

class AppHelper
{
public:
    //设置皮肤样式
    static void SetStyle(QApplication* app, const QString &styleName)
    {
        QSharedPointer<ISkin> skin = SkinFacgtory::getSkin(styleName);
        QString qss = skin->GetCssString();

        QFile file(QString(":/resource/css/black.css"));
        file.open(QFile::ReadOnly);
        QString qssTest = QLatin1String(file.readAll());    //暂时保留测试用

        app->setPalette(QPalette(styleName == "black" ? QColor("#393b3f"):  QColor("#e5e5e7")));
        app->setStyleSheet(qss);
        //app->setPalette(QPalette(styleName == "black" ? QColor("#ff0000"):  QColor("#e5e5e7")));
    }

    static QColor GetBaseColorFromStyleName( const QString &styleName)
    {
        QColor color;

        if(styleName.contains("#"))
            color.setNamedColor(styleName);
        else if(styleName == "black")
            color.setNamedColor(themColorRed);//黑色主题的基础色是红色
        else if(styleName =="red")
            color.setNamedColor(themColorRed);
        else if(styleName =="pink")
            color.setNamedColor(themColorPink);
        else if(styleName =="blue")
            color.setNamedColor(themColorBlue);
        else if(styleName =="green")
            color.setNamedColor(themColorGreen);
        else if(styleName =="gold")
            color.setNamedColor(themColorGold);
        else
            color.setNamedColor(themColorRed);

        return color;
    }
};

#endif // APPHELPER_H
