/********************************************************************************
** Form generated from reading UI file 'goaledit.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GOALEDIT_H
#define UI_GOALEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_GoalEdit
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QPlainTextEdit *plainTextEdit;
    QLabel *label_3;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *deadlineButton;
    QPushButton *tagsButton;

    void setupUi(QDialog *GoalEdit)
    {
        if (GoalEdit->objectName().isEmpty())
            GoalEdit->setObjectName(QString::fromUtf8("GoalEdit"));
        GoalEdit->resize(648, 915);
        buttonBox = new QDialogButtonBox(GoalEdit);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(60, 820, 541, 71));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(GoalEdit);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 10, 281, 51));
        label->setAlignment(Qt::AlignCenter);
        lineEdit = new QLineEdit(GoalEdit);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(30, 120, 591, 51));
        label_2 = new QLabel(GoalEdit);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 90, 81, 21));
        plainTextEdit = new QPlainTextEdit(GoalEdit);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(30, 210, 591, 251));
        label_3 = new QLabel(GoalEdit);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 190, 81, 16));
        radioButton = new QRadioButton(GoalEdit);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(120, 480, 111, 21));
        radioButton_2 = new QRadioButton(GoalEdit);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(260, 480, 97, 22));
        radioButton_3 = new QRadioButton(GoalEdit);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));
        radioButton_3->setGeometry(QRect(400, 480, 97, 22));
        label_4 = new QLabel(GoalEdit);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 570, 55, 16));
        label_5 = new QLabel(GoalEdit);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(40, 680, 55, 16));
        deadlineButton = new QPushButton(GoalEdit);
        deadlineButton->setObjectName(QString::fromUtf8("deadlineButton"));
        deadlineButton->setGeometry(QRect(30, 600, 591, 51));
        tagsButton = new QPushButton(GoalEdit);
        tagsButton->setObjectName(QString::fromUtf8("tagsButton"));
        tagsButton->setGeometry(QRect(30, 710, 591, 51));

        retranslateUi(GoalEdit);
        QObject::connect(buttonBox, SIGNAL(accepted()), GoalEdit, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GoalEdit, SLOT(reject()));

        QMetaObject::connectSlotsByName(GoalEdit);
    } // setupUi

    void retranslateUi(QDialog *GoalEdit)
    {
        GoalEdit->setWindowTitle(QApplication::translate("GoalEdit", "Dialog", nullptr));
        label->setText(QApplication::translate("GoalEdit", "\320\240\320\265\320\264\320\260\320\272\321\202\320\276\321\200 \321\206\320\265\320\273\320\270", nullptr));
        label_2->setText(QApplication::translate("GoalEdit", "\320\235\320\260\320\267\320\262\320\260\320\275\320\270\320\265", nullptr));
        label_3->setText(QApplication::translate("GoalEdit", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265", nullptr));
        radioButton->setText(QApplication::translate("GoalEdit", "\320\236\320\261\321\213\321\207\320\275\320\260\321\217", nullptr));
        radioButton_2->setText(QApplication::translate("GoalEdit", "\320\235\320\260\320\272\320\276\320\277\320\273\320\265\320\275\320\270\320\265", nullptr));
        radioButton_3->setText(QApplication::translate("GoalEdit", "\320\241\320\276\321\201\321\202\320\260\320\262\320\275\320\260\321\217", nullptr));
        label_4->setText(QApplication::translate("GoalEdit", "\320\241\321\200\320\276\320\272", nullptr));
        label_5->setText(QApplication::translate("GoalEdit", "\320\241\321\202\320\260\321\202\321\203\321\201\321\213", nullptr));
        deadlineButton->setText(QApplication::translate("GoalEdit", "deadlineButton", nullptr));
        tagsButton->setText(QApplication::translate("GoalEdit", "tagsButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GoalEdit: public Ui_GoalEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GOALEDIT_H
