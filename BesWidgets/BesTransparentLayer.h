#ifndef BES_TRANSPARENT_LAYER_H
#define BES_TRANSPARENT_LAYER_H

#include <QWidget>

class BesTransparentLayer :public QWidget
{
    Q_OBJECT
public:
    BesTransparentLayer(QWidget* parent = nullptr);
    virtual void mousePressEvent(QMouseEvent* event);

signals:
    void sig_layerPressed();
};

#endif // BES_TRANSPARENT_LAYER_H
