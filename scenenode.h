#ifndef SCENENODE_H
#define SCENENODE_H

#include <QObject>
#include <QQmlListProperty>
#include <QVector3D>

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
    void addNode(SceneNode* node);

    SceneNode* parentNode() const { return m_parentNode; }
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
