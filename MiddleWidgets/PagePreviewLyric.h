#ifndef PAGEPREVIEWLYRIC_H
#define PAGEPREVIEWLYRIC_H

#include <QWidget>

class PagePreviewLyric : public QWidget
{
    Q_OBJECT

public:
    PagePreviewLyric(QWidget *parent = 0);
    ~PagePreviewLyric();

    void initLayout();
    void connectAll();

private:

};

#endif // PAGEPREVIEWLYRIC_H
