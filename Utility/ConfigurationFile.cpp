#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QCoreApplication>
#include "ConfigurationFile.h"

ConfigurationFile::ConfigurationFile(const QString &fileName) :
    m_fileName{fileName},
    m_dirPath{QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)},
    m_dirPathOld{QDir{QCoreApplication::applicationDirPath()}.filePath("data")},//?
    m_filePath{QDir{m_dirPath}.filePath(fileName)}{}

bool ConfigurationFile::readFromFile(QString &content, QString &errorMessage)
{
    QFile configurationFile{m_filePath};

    if (!configurationFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorMessage = tr("无法打开配置文件：%1\n%2").arg(m_filePath).arg(configurationFile.errorString());
        return false;
    }

    content = QString::fromUtf8(configurationFile.readAll());
    configurationFile.close();
    return true;
}

bool ConfigurationFile::writeToFile(const QString &content, QString &errorMessage)
{
    QFile configurationFile{m_filePath};

    QDir configurationDir{m_dirPath};

    if (!configurationDir.exists()) {
        if (!configurationDir.mkpath(".")) {
            errorMessage = tr("无法为配置创建目录：%1").arg(m_dirPath);
        }
    }

    if (!configurationFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        errorMessage = tr("无法打开配置文件：%1\n%2").arg(m_filePath).arg(configurationFile.errorString());
        return false;
    }

    if (configurationFile.write(content.toUtf8()) == -1) {
        errorMessage = tr("无法写入配置到文件：%1").arg(m_filePath);
        configurationFile.close();
        return false;
    }

    configurationFile.close();
    return true;
}

bool ConfigurationFile::Exists()
{
    return QFile::exists(m_filePath);
}

void ConfigurationFile::migrateConfiguration()
{
    //?
}
