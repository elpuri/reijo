#include "scenenode.h"
//#include "shape.h"

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

