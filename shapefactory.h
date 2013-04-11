#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <QQmlScriptString>
#include <QQmlExpression>
#include <QQmlParserStatus>
#include <QtQml>
#include "proputils.h"
#include "scenenode.h"

class ShapeFactoryAttachedProperties : public QObject {
    Q_OBJECT
    SIMPLE_NOTIFYING_PROPERTY(int, index)
    SIMPLE_NOTIFYING_PROPERTY(int, count)

};

class ShapeFactory : public SceneNode, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    SIMPLE_PROPERTY_DECL(QVariant, model)
    SIMPLE_PROPERTY(QQmlScriptString, componentSelectorScript)

public:
    explicit ShapeFactory(QObject *parent = 0);
    
    static ShapeFactoryAttachedProperties* qmlAttachedProperties(QObject* factory);

signals:
    
public slots:
    
protected:
    void classBegin();
    void componentComplete();


private:
    QVariant m_model;
    QQmlExpression* m_selectorExpression;

};

QML_DECLARE_TYPEINFO(ShapeFactory, QML_HAS_ATTACHED_PROPERTIES)


#endif // SHAPEFACTORY_H
