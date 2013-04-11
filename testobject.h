#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QObject>

#include "proputils.h"

class TestObject : public QObject
{
    Q_OBJECT

    SIMPLE_PROPERTY(QString, stringProp)
    SIMPLE_PROPERTY(QString, stringProp2)
    SIMPLE_PROPERTY(qreal, realProp)

public:
    explicit TestObject(QObject *parent = 0) {
        m_stringProp = "foo";
        m_stringProp2 = "bar";
        m_realProp = 3.1415;
    }
    
public slots:
    
};

#endif // TESTOBJECT_H
