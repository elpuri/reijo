/*
-- Copyright (c) 2013, Juha Turunen (turunen@iki.fi)
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice, this
--    list of conditions and the following disclaimer.
-- 2. Redistributions in binary form must reproduce the above copyright notice,
--    this list of conditions and the following disclaimer in the documentation
--    and/or other materials provided with the distribution.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
-- ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
-- WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
-- DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
-- ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
-- (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
-- LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
-- ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
-- (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
-- SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <QQmlScriptString>
#include <QQmlExpression>
#include <QQmlParserStatus>
#include <QtQml>
#include "proputils.h"
#include "scenenode.h"
#include "shape.h"

class ShapeFactoryAttachedProperties : public QObject {
    Q_OBJECT
    SIMPLE_NOTIFYING_PROPERTY(int, index)
    SIMPLE_NOTIFYING_PROPERTY(int, count)

};

class ShapeFactory : public Shape
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    SIMPLE_PROPERTY_DECL(QVariant, model)
    SIMPLE_PROPERTY(QQmlScriptString, componentSelectorScript)

public:
    explicit ShapeFactory(QObject *parent = 0);
    virtual bool intersect(const Ray& ray, double& t) { return false; }
    virtual QVector4D surfaceNormal(const QVector4D& p, const Ray& ray) { return QVector4D(); }
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
