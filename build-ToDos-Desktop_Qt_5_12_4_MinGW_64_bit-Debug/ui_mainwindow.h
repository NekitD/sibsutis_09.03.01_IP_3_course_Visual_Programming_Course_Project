/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *HeadWidget;
    QWidget *TabsWidget;
    QWidget *TagsWidget;
    QPushButton *searchButton;
    QPushButton *filterButton;
    QPushButton *deleteButton;
    QWidget *foldersWidget;
    QLineEdit *searchString;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 650);
        MainWindow->setMinimumSize(QSize(755, 479));
        MainWindow->setBaseSize(QSize(1024, 650));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        HeadWidget = new QWidget(centralWidget);
        HeadWidget->setObjectName(QString::fromUtf8("HeadWidget"));
        HeadWidget->setGeometry(QRect(0, 0, 1024, 142));
        HeadWidget->setMinimumSize(QSize(1024, 142));
        HeadWidget->setBaseSize(QSize(1024, 142));
        TabsWidget = new QWidget(centralWidget);
        TabsWidget->setObjectName(QString::fromUtf8("TabsWidget"));
        TabsWidget->setGeometry(QRect(0, 145, 167, 289));
        TagsWidget = new QWidget(centralWidget);
        TagsWidget->setObjectName(QString::fromUtf8("TagsWidget"));
        TagsWidget->setGeometry(QRect(0, 440, 167, 180));
        searchButton = new QPushButton(centralWidget);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));
        searchButton->setGeometry(QRect(660, 145, 95, 61));
        filterButton = new QPushButton(centralWidget);
        filterButton->setObjectName(QString::fromUtf8("filterButton"));
        filterButton->setGeometry(QRect(760, 145, 126, 61));
        deleteButton = new QPushButton(centralWidget);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));
        deleteButton->setGeometry(QRect(890, 145, 126, 61));
        foldersWidget = new QWidget(centralWidget);
        foldersWidget->setObjectName(QString::fromUtf8("foldersWidget"));
        foldersWidget->setGeometry(QRect(169, 210, 851, 53));
        searchString = new QLineEdit(centralWidget);
        searchString->setObjectName(QString::fromUtf8("searchString"));
        searchString->setGeometry(QRect(200, 145, 461, 61));
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        searchButton->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
        filterButton->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
        deleteButton->setText(QApplication::translate("MainWindow", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
