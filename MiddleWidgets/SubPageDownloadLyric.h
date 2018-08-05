#ifndef SUBPAGEDOWNLOADLYRIC_H
#define SUBPAGEDOWNLOADLYRIC_H

#include <QWidget>

class SubPageDownloadLyric : public QWidget
{
    Q_OBJECT

public:
    SubPageDownloadLyric(QWidget *parent = 0);
    ~SubPageDownloadLyric();

    void initLayout();
    void initConnection();

public:

};

#endif // SUBPAGEDOWNLOADLYRIC_H
