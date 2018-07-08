#ifndef SUBPAGEDOWNLOADSONG_H
#define SUBPAGEDOWNLOADSONG_H

#include <QWidget>

class SubPageDownloadSong : public QWidget
{
    Q_OBJECT

public:
    SubPageDownloadSong(QWidget *parent = 0);
    ~SubPageDownloadSong();

    void initLayout();
    void connectAll();

public:

};

#endif // SUBPAGEDOWNLOADSONG_H
