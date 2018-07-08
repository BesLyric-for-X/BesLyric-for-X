#ifndef BOXPAGEPREVIEWLYRIC_H
#define BOXPAGEPREVIEWLYRIC_H

#include <QWidget>

class BoxPagePreviewLyric : public QWidget
{
    Q_OBJECT

public:
    BoxPagePreviewLyric(QWidget *parent = 0);
    ~BoxPagePreviewLyric();

    void initLayout();
    void connectAll();

public:

};

#endif // BOXPAGEPREVIEWLYRIC_H
