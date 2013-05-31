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

#ifndef SCENENODE_H
#define SCENENODE_H

#include <QObject>
#include <QQmlListProperty>
#include <QVector3D>
#include <functional>
#include "proputils.h"
#include <QDebug>

class SceneNode : public QObject
{
    Q_OBJECT

    Q_CLASSINFO("DefaultProperty", "data")
    Q_PROPERTY(QQmlListProperty<QObject> data READ data CONSTANT)

public:
    explicit SceneNode(QObject *parent = 0);
    QQmlListProperty<QObject> data();

    Q_INVOKABLE void addNode(SceneNode* node);
    Q_INVOKABLE SceneNode* parentNode() const { return m_parentNode; }
    QList<SceneNode*> childNodes() const { return m_childNodes; }
    void dump(int level);


    template<typename T> static T* findFirstNodeOfType(SceneNode* node) {
        T* t = qobject_cast<T*>(node);
        if (t)
            return t;

        foreach(SceneNode* child, node->m_childNodes) {
            t = qobject_cast<T*>(node);
            if (t)
                return t;
            t = findFirstNodeOfType<T>(child);
            if (t)
                return t;
        }

        return nullptr;
    }

    template<typename T> static void gatherNodes(QList<T*>& nodes, SceneNode* node, std::function<bool (T*)> accepted) {
        T* t = qobject_cast<T*>(node);
        if (t && accepted(t))
            nodes.append(t);
        foreach (SceneNode* child, node->childNodes())
            gatherNodes<T>(nodes, child, accepted);
    }

    template<typename T> static void gatherNodeOfType(QList<T*>& nodes, SceneNode* node) {
        T* t = qobject_cast<T*>(node);
        if (t)
            nodes.append(t);

        foreach (SceneNode* child, node->childNodes())
            gatherNodeOfType<T>(nodes, child);
    }


protected:
    virtual QString dumpProperties() { return QString("[]"); }


signals:
    
public slots:

private:
    static void appendChild(QQmlListProperty<QObject> *list, QObject*);
    static QObject* childAt(QQmlListProperty<QObject> *list, int index);
    static int childCount(QQmlListProperty<QObject> *list);

protected:
    SceneNode* m_parentNode;
    QList<SceneNode*> m_childNodes;
};


#endif // SCENENODE_H
