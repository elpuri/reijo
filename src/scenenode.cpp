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

#include "scenenode.h"
#include <QDebug>


SceneNode::SceneNode(QObject *parent) :
    QObject(parent),
    m_parentNode(nullptr)
{
}

QQmlListProperty<QObject> SceneNode::data()
{
    return QQmlListProperty<QObject>(this, this, SceneNode::appendChild, SceneNode::childCount, SceneNode::childAt, nullptr);
}

void SceneNode::appendChild(QQmlListProperty<QObject> *list, QObject *child)
{
//    qDebug() << Q_FUNC_INFO << (void*) child << child->metaObject()->className();
    SceneNode* self = (SceneNode*) list->data;
    child->setParent(self);
    SceneNode* node = qobject_cast<SceneNode*>(child);
    if (node)
        self->addNode(node);
}

int SceneNode::childCount(QQmlListProperty<QObject> *list)
{
    SceneNode* self = (SceneNode*) list->data;
    return self->children().count();
}

QObject* SceneNode::childAt(QQmlListProperty<QObject> *list, int index)
{
    SceneNode* self = (SceneNode*) list->data;
    return self->children().at(index);
}

void SceneNode::addNode(SceneNode *node)
{
//    qDebug() << Q_FUNC_INFO;
    node->m_parentNode = this;
    m_childNodes.append(node);
}

void SceneNode::dump(int level)
{
    QString d;
    for (int i=0; i < level; i++)
        d.append(" ");
    QString tabs(d);
    d.append(metaObject()->className()).append(": ");
    d.append(dumpProperties());
    qDebug() << d;

    foreach (SceneNode* child, m_childNodes)
        child->dump(level + 1);
}

