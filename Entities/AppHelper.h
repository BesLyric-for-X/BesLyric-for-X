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

        app->setStyleSheet(qss);
        app->setPalette(QPalette(styleName == "black" ? QColor("#393b3f"):  QColor("#e5e5e7")));
    }

};

#endif // APPHELPER_H
