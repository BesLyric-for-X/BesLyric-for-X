#ifndef CONFIGURATIONDATA_H
#define CONFIGURATIONDATA_H

#include <QObject>

class ConfigurationFile : public QObject
{
    Q_OBJECT

public:
    ConfigurationFile(const QString &m_fileName);

    bool readFromFile(QString &content, QString &errorMessage);
    bool writeToFile(const QString &content, QString &errorMessage);

    bool Exists();

    void migrateConfiguration();

private:
    QString m_fileName;
    QString m_dirPath;
    QString m_dirPathOld;
    QString m_filePath;

public:
    const QString &filePath() const {return m_filePath;}
};

#endif // CONFIGURATIONDATA_H
