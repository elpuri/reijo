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
