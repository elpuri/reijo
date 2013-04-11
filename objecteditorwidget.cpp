#include "objecteditorwidget.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <QLineEdit>
#include <QDebug>
#include <QDoubleValidator>

template<class T> bool belongsTo(QMetaProperty& p) {
    return p.enclosingMetaObject() == &T::staticMetaObject;
}

ObjectEditorWidget::ObjectEditorWidget(QObject* editable, QWidget *parent) :
    QWidget(parent), m_target(editable), m_layout(this)
{
    const QMetaObject* mo = m_target->metaObject();
    int propCount = mo->propertyCount();
    for (int i=0; i < propCount; i++) {
        QMetaProperty prop = mo->property(i);
        if (prop.isDesignable() && !belongsTo<QObject>(prop)) {
            QWidget* editorWidget = NULL;
            switch(prop.type()) {
            case QVariant::String: {
                QLineEdit* lineEdit = new QLineEdit(this);
                lineEdit->setText(m_target->property(prop.name()).toString());
                editorWidget = lineEdit;
                break;
            }

            case QVariant::Double: {
                QLineEdit* lineEdit = new QLineEdit(this);
                lineEdit->setText(m_target->property(prop.name()).toString());
                lineEdit->setValidator(new QDoubleValidator(this));
                editorWidget = lineEdit;
                break;
            }
            default:
                qWarning() << "ObjectEditorWidget: Skipping property" << prop.name() << "with unrecognized datatype" << prop.typeName();
            }
            if (editorWidget)
                m_layout.addRow(QString(prop.name()), editorWidget);
        }
    }
}
