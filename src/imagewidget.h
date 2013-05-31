#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QLabel>

class ImageWidget : public QLabel
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = 0);
    
signals:
    void clicked(int x, int y);

protected:
    void mousePressEvent(QMouseEvent *ev);
};

#endif // IMAGEWIDGET_H
