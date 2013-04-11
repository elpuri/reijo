#ifndef OBJECTEDITORWIDGET_H
#define OBJECTEDITORWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QDoubleValidator>

class ObjectEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectEditorWidget(QObject* editable, QWidget *parent = 0);
    
signals:
    
public slots:
    

private:

    QObject* m_target;
    QFormLayout m_layout;
//    QDoubleValidator m_doubleValidator;

};

#endif // OBJECTEDITORWIDGET_H
