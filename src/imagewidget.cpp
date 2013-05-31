#include "imagewidget.h"
#include <QMouseEvent>
#include <QDebug>

ImageWidget::ImageWidget(QWidget *parent) :
    QLabel(parent)
{
}

void ImageWidget::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << Q_FUNC_INFO;
    emit clicked(ev->x(), ev->y());
}
