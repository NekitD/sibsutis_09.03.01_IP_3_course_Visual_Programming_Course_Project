#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, "Ошибка",
            "Ваша система не поддерживает системный лоток.");
        return 1;
    }

    a.setQuitOnLastWindowClosed(false);

    MainWindow w;
    w.show();

    return a.exec();
}
