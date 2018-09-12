#include "BesTransparentLayer.h"

BesTransparentLayer::BesTransparentLayer(QWidget *parent):QWidget(parent)
{
    this->setMouseTracking(true);

}



void BesTransparentLayer::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    emit sig_layerPressed();
}

