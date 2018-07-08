#ifndef APPHELPER_H
#define APPHELPER_H
#include <QString>
#include <QFile>
#include <QApplication>
#include <QColor>
#include <QPalette>

class AppHelper
{
public:
    //设置皮肤样式
    static void SetStyle(QApplication& app, const QString &styleName)
    {
        //QFile file(QString(":/image/%1.css").arg(styleName));
        QFile file(QString(":/resource/css/black.css").arg(styleName));
        file.open(QFile::ReadOnly);
        QString qss = QLatin1String(file.readAll());
        app.setStyleSheet(qss);
        app.setPalette(QPalette(QColor("#fefefe")));
        QColor color1 = QColor("#F0F0F0");
        QColor color2 = QColor("#CC0033");
        QColor color3 = QColor("#bb0033");
        QColor color4 = QColor("#aa0033");
    }

};

#endif // APPHELPER_H
