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

#include "shapefactory.h"
#include <QDebug>
#include <QQmlComponent>
#include <QQmlEngine>


ShapeFactory::ShapeFactory(QObject *parent) :
    SceneNode(parent),
    m_selectorExpression(nullptr)
{
}

QVariant ShapeFactory::model()
{
    return m_model;
}



void ShapeFactory::modelSetter(QVariant model)
{
    m_model = model;
}

void ShapeFactory::classBegin()
{
}

void ShapeFactory::componentComplete()
{
//    qDebug() << Q_FUNC_INFO << (void*) parentNode();
    m_selectorExpression = new QQmlExpression(m_componentSelectorScript);

    if (m_model.type() == QMetaType::Int) {
        int count = m_model.toInt();
        qDebug() << "Creating" << count << "nodes";
        for (int i=0; i < count; i++) {
            bool undefined;
            auto retVal = m_selectorExpression->evaluate(&undefined);
            if (undefined) {
                qCritical() << "Couldn't evaluate selector expression" << m_selectorExpression->error().toString();
            }

            QQmlComponent* component = nullptr;

            if (retVal.canConvert<QObject*>())
                component = qobject_cast<QQmlComponent*>(retVal.value<QObject*>());

            if (component) {
                QObject* o = component->create(QQmlEngine::contextForObject(this));
                SceneNode* node = qobject_cast<SceneNode*>(o);
                if (!node) {
                    qCritical() << "ShapeFactory: Component isn't a SceneNode" << component->url();
                    delete o;
                    break;
                }
                ShapeFactoryAttachedProperties* attached =
                        (ShapeFactoryAttachedProperties*) qmlAttachedPropertiesObject<ShapeFactory>(node, true);
                attached->indexSetter(i);
                attached->countSetter(count);
                QQmlListProperty<QObject> list = data();
                list.append(&list, node);
            } else {
                qCritical() << "ShapeFactory: Component selector script didn't return a Component";
                qCritical() << m_selectorExpression->expression();
                break;
            }
        }
    }
}

ShapeFactoryAttachedProperties* ShapeFactory::qmlAttachedProperties(QObject *factory)
{
//    qDebug() << Q_FUNC_INFO << factory->metaObject()->className();
    return new ShapeFactoryAttachedProperties();
}
