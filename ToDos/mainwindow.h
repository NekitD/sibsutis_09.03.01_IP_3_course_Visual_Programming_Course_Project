#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QAbstractItemModel>

namespace Ui {
class MainWindow;
}

class TimeDesk;
class NearEventDesk;

class ChoosableObjectsList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TimeDesk* n_timeDesk;
    NearEventDesk* n_nearEventDesk;
    QPushButton* n_noteButton;
    QPushButton* n_settingsButton;
    QPushButton* n_aboutButton;
};


class TimeDesk : public QWidget{
    Q_OBJECT
public:
    explicit TimeDesk(QWidget *parent = nullptr);
//    TimeDesk();
private:
//    QString* deskName;
//    QString* datetimeOutput;
};

class NearEventDesk : public QWidget{
    Q_OBJECT
public:
    explicit NearEventDesk(QWidget *parent = nullptr);

};


class ChoosableObjectsList : public QAbstractItemModel{

};

#endif // MAINWINDOW_H
