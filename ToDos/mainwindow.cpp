#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    const QString mainPathToSource("C:\\Users\\Danik\\Desktop\\sibsutis_09.03.01_IP_3_course_Visual_Programming_Course_Project");

    ui->setupUi(this);
    setWindowTitle("ToDos");
    setWindowIcon(QIcon(QString(mainPathToSource + "\\IMG\\MainIcon.ico")));

    ui->centralWidget->setStyleSheet("QWidget{background-color: rgba(238, 232, 211, 1);}");
    ui->HeadWidget->setStyleSheet("QWidget{background-color: rgba(221, 205, 179, 1); border: 3px solid black;}");

    n_timeDesk = new TimeDesk(ui->HeadWidget);
    n_nearEventDesk = new NearEventDesk(ui->HeadWidget);
    n_noteButton = new QPushButton;
    n_settingsButton = new QPushButton;
    n_aboutButton = new QPushButton;

    QWidget* h_desks = new QWidget;
    QWidget* h_buttons = new QWidget;

    QHBoxLayout* headLayout = new QHBoxLayout;
    QHBoxLayout* h_desksLayout = new QHBoxLayout;
    QHBoxLayout* h_buttonsLayout = new QHBoxLayout;
    h_desksLayout->addWidget(n_timeDesk);
    h_desksLayout->addWidget(n_nearEventDesk);
    h_buttonsLayout->addWidget(n_noteButton);
    h_buttonsLayout->addWidget(n_settingsButton);
    h_buttonsLayout->addWidget(n_aboutButton);

    h_desksLayout->setContentsMargins(0, 0, 0, 0);
    h_buttonsLayout->setContentsMargins(0, 0, 0, 0);

    h_desks->setLayout(h_desksLayout);
    h_buttons->setLayout(h_buttonsLayout);

    headLayout->addWidget(h_desks, 2);
    headLayout->addWidget(h_buttons, 1);

    ui->HeadWidget->setLayout(headLayout);
    h_desks->setStyleSheet("QWidget{padding: 0; border: 0px;}");
    h_buttons->setStyleSheet("QWidget{padding: 0; border: 0px;} QPushButton{border: 3px solid black;}");

    n_noteButton->setFixedSize(90, 90);
    n_noteButton->setStyleSheet("border-radius: 30px; background-color: rgba(232, 145, 125, 1);");
    n_noteButton->setIcon(QIcon(QString(mainPathToSource + "\\IMG\\notif.svg.png")));
    n_noteButton->setIconSize(n_noteButton->size() * 0.9);

    n_settingsButton->setFixedSize(75, 75);
    n_settingsButton->setStyleSheet("border-radius: 25px; background-color: rgba(189, 190, 194, 1);");
    n_settingsButton->setIcon(QIcon(QString(mainPathToSource + "\\IMG\\settings.png")));
    n_settingsButton->setIconSize(n_settingsButton->size());


    n_aboutButton->setFixedSize(70, 70);
    n_aboutButton->setStyleSheet("QPushButton{border-radius: 35px; background-color: rgba(0, 186, 255, 1); color: white; font-size: 50px;}");
    n_aboutButton->setText("?");

    ui->TabsWidget->setStyleSheet("QWidget{background-color: rgba(198, 184, 167, 1); border: 3px solid black;}");

    QVBoxLayout* tabsLayout = new QVBoxLayout;
    QLabel* tabsHeader = new QLabel("Вкладки");
    tabsHeader->setStyleSheet("QLabel{border: 3px solid black; font-weight: Bold; font-size: 30px}");
    tabsHeader->setAlignment(Qt::AlignCenter);
    QListView* tabsOutput = new QListView;
    tabsLayout->addWidget(tabsHeader, 1);
    tabsLayout->addWidget(tabsOutput, 4);
    tabsLayout->setContentsMargins(0, 0, 0, 0);
    tabsLayout->setSpacing(0);
    ui->TabsWidget->setLayout(tabsLayout);

    ui->TagsWidget->setStyleSheet("QWidget{background-color: rgba(196, 196, 196, 1); border: 3px solid black;}");
    QVBoxLayout* tagsLayout = new QVBoxLayout;
    QLabel* tagsHeader = new QLabel("Тэги");
    tagsHeader->setStyleSheet("QLabel{border: 3px solid black; font-style: italic; font-weight: bold; font-size: 30px}");
    tagsHeader->setAlignment(Qt::AlignCenter);
    QListView* tagsOutput = new QListView;
    tagsLayout->addWidget(tagsHeader, 1);
    tagsLayout->addWidget(tagsOutput, 3);
    tagsLayout->setContentsMargins(0, 0, 0, 0);
    tagsLayout->setSpacing(0);
    ui->TagsWidget->setLayout(tagsLayout);

    ui->searchString->setStyleSheet("QLineEdit{background-color: rgba(236, 243, 246, 1); border: 2px solid black; font-size: 35px;}");
    ui->searchString->setPlaceholderText("Поиск...");
    ui->searchButton->setStyleSheet("QPushButton{background-color: rgba(81, 226, 246, 0.53); border: 2px solid black; font-size: 25px;}");
    ui->searchButton->setText("Искать");

    ui->filterButton->setStyleSheet("QPushButton{background-color: rgba(205, 247, 198, 1); border: 2px solid black}");
    ui->filterButton->setIcon(QIcon(QString(mainPathToSource + "\\IMG\\filter.svg")));
    ui->filterButton->setIconSize(ui->filterButton->size());
    ui->filterButton->setText("");

    ui->deleteButton->setStyleSheet("QPushButton{background-color: rgba(239, 85, 85, 0.3); border: 2px solid black}");
    ui->deleteButton->setIcon(QIcon(QString(mainPathToSource + "\\IMG\\trash.svg")));
    ui->deleteButton->setIconSize(ui->deleteButton->size() * 0.9);
    ui->deleteButton->setText("");

    ui->foldersWidget->setStyleSheet("QWidget{background-color: rgba(232, 203, 168, 1); border: 1px solid black;}");

    ui->addFolderButton->setStyleSheet("QPushButton{background-color: rgba(165, 224, 155, 1); border: 2px solid black}");
    ui->addFolderButton->setIcon(QIcon(QString(mainPathToSource + "\\IMG\\AddFolder.png")));
    ui->addFolderButton->setIconSize(ui->addFolderButton->size() * 0.9);
    ui->addFolderButton->setText("");

    QHBoxLayout* foldersLayout = new QHBoxLayout;
    QListView* foldersOutput = new QListView;
    foldersLayout->addWidget(foldersOutput, 7);
    foldersLayout->addWidget(ui->addFolderButton, 1);
    foldersLayout->setContentsMargins(0, 0, 0, 0);
    foldersLayout->setSpacing(0);
    ui->foldersWidget->setLayout(foldersLayout);
}

MainWindow::~MainWindow()
{
    delete ui;
}


TimeDesk::TimeDesk(QWidget *parent):
    QWidget(parent)
{
    setStyleSheet("QWidget{background-color: rgba(230, 198, 145, 1); border: 3px solid black; font-size: 24px;}");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    //layout->setAlignment(Qt::AlignCenter);
    QLabel* name = new QLabel("Закончить день");
    name->setAlignment(Qt::AlignCenter);
    name->setStyleSheet("QLabel{border: 3px solid black; font-weight: Bold;}");

    QWidget* subwidget = new QWidget;
    QVBoxLayout* sublayout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel* dateoutput = new QLabel("12 декабря 2025");
    dateoutput->setAlignment(Qt::AlignCenter);
    dateoutput->setStyleSheet("QLabel{border: 0px;}");
    QLabel* timeoutput = new QLabel("16:15");
    timeoutput->setAlignment(Qt::AlignCenter);
    timeoutput->setStyleSheet("QLabel{border: 0px; font-weight: Bold;}");
    layout->addWidget(name);
    sublayout->addWidget(dateoutput);
    sublayout->addWidget(timeoutput);
    subwidget->setLayout(sublayout);
    layout->addWidget(subwidget);
    setLayout(layout);
}

NearEventDesk::NearEventDesk(QWidget *parent):
    QWidget(parent)
{
    setStyleSheet("QWidget{background-color: rgba(230, 198, 145, 1); border: 3px solid black; font-size: 24px;}");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    QLabel* name = new QLabel("Ближайшая цель");
    name->setAlignment(Qt::AlignCenter);
    name->setStyleSheet("QLabel{border: 3px solid black; font-weight: Bold;}");

    QWidget* subwidget = new QWidget;
    QVBoxLayout* sublayout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel* datetimeoutput = new QLabel("13 декабря 2025, 9:50");
    datetimeoutput->setAlignment(Qt::AlignCenter);
    datetimeoutput->setStyleSheet("QLabel{border: 0px;}");
    QLabel* goaloutput = new QLabel("Зачет по физре");
    goaloutput->setAlignment(Qt::AlignCenter);
    goaloutput->setStyleSheet("QLabel{border: 0px;}");
    layout->addWidget(name);
    sublayout->addWidget(goaloutput);
    sublayout->addWidget(datetimeoutput);
    subwidget->setLayout(sublayout);
    layout->addWidget(subwidget);
    setLayout(layout);
}


