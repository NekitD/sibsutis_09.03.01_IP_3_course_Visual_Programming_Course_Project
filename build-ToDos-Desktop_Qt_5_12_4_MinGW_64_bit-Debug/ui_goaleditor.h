/********************************************************************************
** Form generated from reading UI file 'goaleditor.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOALEDITOR_H
#define UI_GOALEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GoalEditor
{
public:

    void setupUi(QWidget *GoalEditor)
    {
        if (GoalEditor->objectName().isEmpty())
            GoalEditor->setObjectName(QString::fromUtf8("GoalEditor"));
        GoalEditor->resize(648, 915);

        retranslateUi(GoalEditor);

        QMetaObject::connectSlotsByName(GoalEditor);
    } // setupUi

    void retranslateUi(QWidget *GoalEditor)
    {
        GoalEditor->setWindowTitle(QApplication::translate("GoalEditor", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GoalEditor: public Ui_GoalEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOALEDITOR_H
