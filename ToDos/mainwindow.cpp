#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "goaledit.h"
#include "ui_goaledit.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QDialog>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include <QLocale>
#include <QUuid>
#include <QTableView>
#include <QHeaderView>
#include <QCalendarWidget>
#include <QRandomGenerator>
#include <QMenu>
#include <QWidgetAction>
#include <QCheckBox>
#include <QDebug>
#include <QRadioButton>
#include <QMessageBox>
#include <QScrollArea>
#include "delegates.h"
#include "objects.h"
#include "lists.h"
#include "models.h"



//----------------------------------------------------------------------------------------------------------


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    mainPathToSource = new QString("C:\\Users\\Danik\\Desktop\\sibsutis_09.03.01_IP_3_course_Visual_Programming_Course_Project");

    ui->setupUi(this);
    setWindowTitle("ToDos");
    setWindowIcon(QIcon(QString(*mainPathToSource + "\\IMG\\MainIcon.ico")));

    ui->centralWidget->setStyleSheet("QWidget{background-color: rgba(238, 232, 211, 1);}");
    ui->HeadWidget->setStyleSheet("QWidget{background-color: rgba(221, 205, 179, 1); border: 3px solid black;}");

    n_timeDesk = new TimeDesk(ui->HeadWidget);
    QString goalsJsonPath = *mainPathToSource + "\\DATA\\GOALS.json";
    n_nearEventDesk = new NearEventDesk(goalsJsonPath, ui->HeadWidget);

    n_noteButton = new QPushButton;
    n_noteButton->setCheckable(true);
    n_noteButton->setChecked(false);
    connect(n_noteButton, &QPushButton::clicked, this, [this]() {
        showNotificationsMenu();
    });

    n_noteButton->setCursor(Qt::PointingHandCursor);



    n_settingsButton = new QPushButton;
    n_settingsButton->setCursor(Qt::PointingHandCursor);
    n_aboutButton = new QPushButton;
    n_aboutButton->setCursor(Qt::PointingHandCursor);

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

    n_timeDesk->setCursor(Qt::PointingHandCursor);
    n_nearEventDesk->setCursor(Qt::PointingHandCursor);

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
    n_noteButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\notif.svg.png")));
    n_noteButton->setIconSize(n_noteButton->size() * 0.9);
    n_noteButton->setToolTip("Уведомления");
    loadNotifications();
    updateNotificationButtonIcon();

    n_settingsButton->setFixedSize(75, 75);
    n_settingsButton->setStyleSheet("border-radius: 25px; background-color: rgba(189, 190, 194, 1);");
    n_settingsButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\settings.png")));
    n_settingsButton->setIconSize(n_settingsButton->size());
    n_settingsButton->setToolTip("Настройки");
    connect(n_settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);


    n_aboutButton->setFixedSize(70, 70);
    n_aboutButton->setStyleSheet("QPushButton{border-radius: 35px; background-color: rgba(0, 186, 255, 1); color: white; font-size: 50px;}");
    n_aboutButton->setText("?");
    n_aboutButton->setToolTip("Справка");
    connect(n_aboutButton, &QPushButton::clicked, this, &MainWindow::openAbout);

    ui->TabsWidget->setStyleSheet("QWidget{background-color: rgba(198, 184, 167, 1); border: 3px solid black;}");


    QVBoxLayout* tabsLayout = new QVBoxLayout;
    QLabel* tabsHeader = new QLabel("Вкладки");
    tabsHeader->setStyleSheet("QLabel{border: 3px solid black; font-weight: Bold; font-size: 30px}");
    tabsHeader->setAlignment(Qt::AlignCenter);

    QString tabcolor = "rgba(222, 206, 187, 1)";
    QString itPadtab = "10px";
    QString itPadtag = "5px";
    QString ItemStyle = QString(R"(
                         QListView {
                             font-size: 14px;
                             font-family: "Segoe UI";
                             outline: 0;
                         }

                         QListView::item {
                             background-color: %1;
                             border: 3px solid black;
                             padding: %2;
                             margin: 3px;
                             color: black;
                         }
                         QListView::item:hover {
                             background-color: rgba(255, 240, 163, 1);
                             border: 3px solid black;
                         }
                         QListView::item:selected {
                             background-color: rgba(255, 240, 163, 1);
                             border: 5px solid black;
                             font-weight: bold;
                         }

                     )");

    QString tabItemStyle = QString(ItemStyle).arg(tabcolor).arg(itPadtab);
    QListView* tabsOutput = new QListView;
    tabsOutput->setFlow(QListView::TopToBottom);
    tabsOutput->setSpacing(1);
    tabsOutput->setStyleSheet(tabItemStyle);
    tabsOutput->setItemDelegate(new TabsDelegate);
    TabsList* tabsModel = importTabsFromJson();
    tabsOutput->setModel(tabsModel);
    tabsOutput->setCurrentIndex(tabsModel->index(0, 0));
    tabsModel->select(0);
    tabsOutput->show();
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
    QString tagcolor = "rgba(223, 212, 212, 1)";
    QString tagItemStyle = QString(ItemStyle).arg(tagcolor).arg(itPadtag);
    QListView* tagsOutput = new QListView;
    tagsOutput->setFlow(QListView::TopToBottom);
    tagsOutput->setSpacing(1);
    tagsOutput->setStyleSheet(tagItemStyle);
    tagsOutput->setItemDelegate(new TagsDelegate);
    TagsList* tagsModel = importTagsFromJson();
    tagsOutput->setModel(tagsModel);
    tagsOutput->show();
    tagsLayout->addWidget(tagsHeader, 1);
    tagsLayout->addWidget(tagsOutput, 3);
    tagsLayout->setContentsMargins(0, 0, 0, 0);
    tagsLayout->setSpacing(0);
    ui->TagsWidget->setLayout(tagsLayout);


    ui->searchString->setStyleSheet("QLineEdit{background-color: rgba(236, 243, 246, 1); border: 2px solid black; font-size: 35px;}");
    ui->searchString->setPlaceholderText("Поиск...");
    ui->searchButton->setStyleSheet("QPushButton{background-color: rgba(81, 226, 246, 0.53); border: 2px solid black; font-size: 25px;}");
    ui->searchButton->setText("");
    ui->searchButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\loop.png")));
    ui->searchButton->setIconSize(ui->filterButton->size());
    ui->searchButton->setToolTip("Искать");
    ui->searchButton->setCursor(Qt::PointingHandCursor);

    ui->filterButton->setStyleSheet("QPushButton{background-color: rgba(205, 247, 198, 1); border: 2px solid black}");
    ui->filterButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\filter.svg")));
    ui->filterButton->setIconSize(ui->filterButton->size());
    ui->filterButton->setText("");
    ui->filterButton->setToolTip("Фильтр");
    ui->filterButton->setCheckable(true);
    ui->filterButton->setChecked(false);
    ui->filterButton->setCursor(Qt::PointingHandCursor);


    connect(ui->filterButton, &QPushButton::clicked, this, [this]() {
        showFilterMenu();
    });

    ui->deleteButton->setStyleSheet("QPushButton{background-color: rgba(239, 85, 85, 0.3); border: 2px solid black;}");
    ui->deleteButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\off_trash.png")));
    ui->deleteButton->setIconSize(ui->deleteButton->size() * 0.9);
    ui->deleteButton->setText("");
    ui->deleteButton->setToolTip("Удалить цель");

    ui->foldersWidget->setStyleSheet("QWidget{background-color: rgba(232, 203, 168, 1); border: 1px solid black;}");

    ui->addFolderButton->setStyleSheet("QPushButton{background-color: rgba(165, 224, 155, 1); border: 2px solid black}");
    ui->addFolderButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\AddFolder.png")));
    ui->addFolderButton->setIconSize(ui->addFolderButton->size() * 0.9);
    ui->addFolderButton->setText("");
    ui->addFolderButton->setToolTip("Новая папка");
    ui->addFolderButton->setCursor(Qt::PointingHandCursor);

    QString folderStyle = QString(R"(
                         QListView {
                             font-size: 14px;
                             font-family: "Segoe UI";
                             outline: 0;
                         }

                         QListView::item {
                             border: 2px solid black;
                             padding: 1px;
                             margin: 0px;
                             color: black;
                         }
                         QListView::item:selected {
                             background-color: rgba(232, 208, 121, 1);
                             border: 3px solid black;
                             font-weight: bold;
                         }
                     )");
    QHBoxLayout* foldersLayout = new QHBoxLayout;
    QListView* foldersOutput = new QListView;
    int itemWidth = 180;
    FoldersDelegate* delegate = new FoldersDelegate(itemWidth, 35);
    foldersOutput->setItemDelegate(delegate);
    foldersOutput->setFlow(QListView::LeftToRight);
    foldersOutput->setSpacing(1);
    foldersModel = importFoldersFromJson();
    foldersOutput->setModel(foldersModel);
    foldersOutput->setCurrentIndex(foldersModel->index(0, 0));
    foldersOutput->show();
    foldersLayout->addWidget(foldersOutput, 7);
    foldersLayout->addWidget(ui->addFolderButton, 1);
    foldersLayout->setContentsMargins(0, 0, 0, 0);
    foldersLayout->setSpacing(0);
    ui->foldersWidget->setLayout(foldersLayout);
    foldersJsonPath = *mainPathToSource + "\\DATA\\FOLDERS.json";

    connect(ui->addFolderButton, &QPushButton::clicked, [this](){openFolder(true);});

    tabsOutput->setSelectionMode(QAbstractItemView::SingleSelection);
    tagsOutput->setSelectionMode(QAbstractItemView::SingleSelection);
    foldersOutput->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(tabsOutput->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            [tabsModel](const QModelIndex& current, const QModelIndex&) {
                tabsModel->select(current.row());
            });
    connect(tabsOutput->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            [tagsOutput](const QModelIndex&) {
                //if(current.isValid()){
                    auto sm = tagsOutput->selectionModel();
                        sm->blockSignals(true);
                        sm->clearSelection();
                        sm->setCurrentIndex(QModelIndex(), QItemSelectionModel::NoUpdate);
                        sm->blockSignals(false);
                    //};
                });

    connect(tabsOutput->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            [this](const QModelIndex& current, const QModelIndex&) {

                if (!current.isValid())
                    return;

                QString tabId = current.data(IdRole).toString();
                onTabChanged(tabId);
            });

    connect(tagsOutput->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            [tabsOutput](const QModelIndex&) {
                //if(current.isValid()){
                    auto sm = tabsOutput->selectionModel();
                                sm->blockSignals(true);
                                sm->clearSelection();
                                sm->setCurrentIndex(QModelIndex(), QItemSelectionModel::NoUpdate);
                                sm->blockSignals(false);
                 //};
            });

    goalsModel = new GoalsTableModel(this);
    goalsModel->setGoals(importGoalsFromJson());

    goalsFilter = new GoalsFilterModel(this);
    goalsFilter->setSourceModel(goalsModel);

    goalsStack = new QStackedWidget(ui->GoalsWidget);

    todayPage = createTodayPage();
    incomingPage = createIncomingPage();
    calendarPage = createCalendarPage();
    kanbanPage = createKanbanPage();

    goalsStack->addWidget(todayPage);
    goalsStack->addWidget(incomingPage);
    goalsStack->addWidget(calendarPage);
    goalsStack->addWidget(kanbanPage);

//    auto* layout = new QVBoxLayout(ui->GoalsWidget);
//    layout->setContentsMargins(0, 0, 0, 0);
//    layout->addWidget(goalsStack);
//    ui->GoalsWidget->setLayout(layout);
    auto* grid = new QGridLayout(ui->GoalsWidget);
    grid->setContentsMargins(1, 1, 1, 1);
    grid->setSpacing(0);

    // основной контент
    grid->addWidget(goalsStack, 0, 0);

    addGoalButton = new QPushButton("+", ui->GoalsWidget);
    addGoalButton->setFixedSize(70, 70);
    addGoalButton->setCursor(Qt::PointingHandCursor);

    addGoalButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(165, 224, 155, 1);
            border: 3px solid white;
            border-radius: 35px;
            color: white;
            font-size: 60px;
            font-weight: bold;
            padding-bottom: 8px;
        }
        QPushButton:hover {
            background-color: rgba(145, 210, 135, 1);
        }
        QPushButton:pressed {
            background-color: rgba(120, 190, 110, 1);
        }
    )");

    grid->addWidget(addGoalButton, 0, 0,
                    Qt::AlignRight | Qt::AlignBottom);

    ui->GoalsWidget->setLayout(grid);

    connect(addGoalButton, &QPushButton::clicked, this, &MainWindow::openAddGoal);
//    connect(addGoalButton, &QPushButton::clicked, this, [this]() {
//        GoalEdit dlg(this);
//        if (dlg.exec() == QDialog::Accepted) {
//            Goal* g = dlg.createdGoal();
//            goalsModel->addGoal(g);
//            saveGoalsToJson();
//        }
//    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openAddGoal()
{
    GoalEdit dlg(this);
    dlg.setModal(true);
    dlg.exec();

    // позже тут:
    // if (dlg.result() == QDialog::Accepted)
    //    перечитать GOALS.json и обновить модель
}



void MainWindow::saveFoldersToJson()
{
    QJsonArray array;

    for (auto* obj : foldersModel->items()) {
        Folder* folder = static_cast<Folder*>(obj);

        QJsonObject o;
        o["id"] = folder->id;
        o["name"] = folder->name;

        array.append(o);
    }

    QJsonObject root;
    root["folders"] = array;

    QFile file(foldersJsonPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
        file.close();
    }
}


QWidget* MainWindow::createIncomingPage()
{
    QWidget* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);

    auto* view = new QTableView;
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setFocusPolicy(Qt::NoFocus);

    auto* filter = new GoalsFilterModel(view);
    filter->setSourceModel(goalsModel);
    filter->setTodayOnly(false);
    filter->setDateFilter(QDate());
    filter->setTagFilter("");

    view->setModel(filter);
    view->setItemDelegate(new GoalsDelegate(view));

    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setVisible(false);
    view->verticalHeader()->setDefaultSectionSize(80);


    view->setStyleSheet(R"(
            QTableView {
                background-color: rgba(228, 220, 197, 1);
                border: 3px solid black;
                font-size: 16px;
                selection-background-color: rgba(255, 230, 160, 1);
                padding-top: 3px;
                padding-bottom: 6px;
                padding-left: 5px;
                padding-right: 5px;
            }

            QTableView::item {
                background-color: rgba(255, 250, 230, 1);
                border: 1px solid black;
            }

            QTableView::item:selected {
                background-color: rgba(255, 235, 180, 1);
            }
        )");

        view->horizontalHeader()->setStyleSheet(R"(
            QHeaderView::section {
                background-color: rgba(247, 232, 179, 1);
                border: 2px solid black;
                font-size: 24px;
                font-weight: bold;
                padding: 6px;
            }
        )");



    layout->addWidget(view);
    return page;
}


QWidget* MainWindow::createTodayPage()
{
    QWidget* page = new QWidget;
    auto* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);

    auto* view = new QTableView;
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setFocusPolicy(Qt::NoFocus);

    goalsFilter->setTodayOnly(true);
    goalsFilter->setDateFilter(QDate());
    goalsFilter->setTagFilter("");

    auto* filter = new GoalsFilterModel(view);
    filter->setSourceModel(goalsModel);
    filter->setTodayOnly(true);
    filter->setDateFilter(QDate());
    filter->setTagFilter("");

    view->setModel(filter);
    view->setItemDelegate(new GoalsDelegate(view));
    view->setStyleSheet(R"(
            QTableView {
                background-color: rgba(228, 220, 197, 1);
                border: 3px solid black;
                font-size: 16px;
                selection-background-color: rgba(255, 230, 160, 1);
                                 padding-top: 3px;
                                 padding-bottom: 6px;
                                 padding-left: 5px;
                                 padding-right: 5px;
            }

            QTableView::item {
                background-color: rgba(255, 250, 230, 1);
                border: 1px solid black;
            }

            QTableView::item:selected {
                background-color: rgba(255, 235, 180, 1);
            }
        )");

        view->horizontalHeader()->setStyleSheet(R"(
            QHeaderView::section {
                background-color: rgba(247, 232, 179, 1);
                border: 2px solid black;
                font-size: 24px;
                font-weight: bold;
                padding: 6px;
            }
        )");

    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setVisible(false);
    view->verticalHeader()->setDefaultSectionSize(80);

    layout->addWidget(view);
    return page;
}

QWidget* MainWindow::createCalendarPage()
{
    QWidget* page = new QWidget;
    auto* layout = new QHBoxLayout(page);
    layout->setContentsMargins(0,0,0,0);

    auto* list = new QListView;
    auto* calendar = new QCalendarWidget;

    calendar->setStyleSheet("background-color: rgba(246, 245, 201, 1)");
    list->setStyleSheet("background-color: rgba(228, 220, 197, 1)");

    auto* model = new GoalsFilterModel(this);
    model->setSourceModel(goalsModel);


    model->setDateFilter(QDate::currentDate());

    list->setModel(model);

    connect(calendar, &QCalendarWidget::clicked,
            this, [model](const QDate& date){
        model->setDateFilter(date);
    });

    layout->addWidget(list, 2);
    layout->addWidget(calendar, 1);

    return page;
}

QWidget* MainWindow::createKanbanColumn(const QString& tagName)
{
    QWidget* col = new QWidget;
    auto* layout = new QVBoxLayout(col);

    QLabel* header = new QLabel(tagName);
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet("font-size: 20px; font-weight: bold;");

    auto* view = new QListView;

    auto* model = new GoalsFilterModel(this);
    model->setSourceModel(goalsModel);
    model->setTagFilter(tagName);

    view->setModel(model);
    view->setModelColumn(0);

    layout->addWidget(header);
    layout->addWidget(view);
    return col;
}

QWidget* MainWindow::createKanbanPage()
{
    QWidget* page = new QWidget;
    auto* layout = new QHBoxLayout(page);

    layout->addWidget(createKanbanColumn("Получено"));
    layout->addWidget(createKanbanColumn("В работе"));
    layout->addWidget(createKanbanColumn("Выполнено"));

    return page;
}

void MainWindow::onTabChanged(const QString& tabId)
{
    goalsFilter->setTodayOnly(false);
    goalsFilter->setDateFilter(QDate());
    goalsFilter->setTagFilter("");

    if (tabId == "today") {
        goalsFilter->setTodayOnly(true);
        goalsStack->setCurrentWidget(todayPage);
    }
    else if (tabId == "all") {
        goalsStack->setCurrentWidget(incomingPage);
    }
    else if (tabId == "calendar") {
        goalsStack->setCurrentWidget(calendarPage);
    }
    else if (tabId == "kanban") {
        goalsStack->setCurrentWidget(kanbanPage);
    }
}




//-----------------------------------------------------------------------------------------------------------------
TimeDesk::TimeDesk(QWidget *parent):
    QWidget(parent)
{
    setToolTip("Закончить день");
    setStyleSheet("QWidget{background-color: rgba(230, 198, 145, 1); border: 3px solid black; font-size: 24px;}");
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    QLabel* name = new QLabel("Закончить день");
    name->setAlignment(Qt::AlignCenter);
    name->setStyleSheet("QLabel{border: 3px solid black; font-weight: Bold;}");

    QWidget* datetimeWidget = new QWidget;
    QVBoxLayout* datetimeLayout = new QVBoxLayout;



    dateLabel = new QLabel;
    timeLabel = new QLabel;
    dateLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setAlignment(Qt::AlignCenter);
    dateLabel->setStyleSheet("QLabel{border: 0px;}");
    timeLabel->setStyleSheet("QLabel{border: 0px; font-weight: Bold;}");
    layout->addWidget(name);
    datetimeLayout->addWidget(dateLabel);
    datetimeLayout->addWidget(timeLabel);
    datetimeWidget->setLayout(datetimeLayout);
    layout->addWidget(datetimeWidget);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TimeDesk::updateTime);
    timer->start(1000); // раз в секунду
    updateTime();
}

void TimeDesk::updateTime()
{
    QDateTime now = QDateTime::currentDateTime();

    QLocale ru(QLocale::Russian);

    dateLabel->setText(ru.toString(now.date(), "d MMMM yyyy"));
    timeLabel->setText(now.time().toString("HH:mm"));
}

NearEventDesk::NearEventDesk(const QString& jsonPath, QWidget* parent)
    : QWidget(parent), m_jsonPath(jsonPath)
{
    setToolTip("Открыть ближайшую цель");
    setStyleSheet("QWidget{background-color: rgba(230, 198, 145, 1); border: 3px solid black; font-size: 24px;}");

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel* title = new QLabel("Ближайшая цель");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("QLabel{border: 3px solid black; font-weight: Bold;}");

    goalLabel = new QLabel("—");
    datetimeLabel = new QLabel("—");

    goalLabel->setAlignment(Qt::AlignCenter);
    datetimeLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(title);


    QWidget* contentWidget = new QWidget;
    contentWidget->setStyleSheet("QLabel{border: 0}");
    QVBoxLayout* contentLayout = new QVBoxLayout;
    contentLayout->addWidget(goalLabel);
    contentLayout->addWidget(datetimeLabel);
    contentWidget->setLayout(contentLayout);

    layout->addWidget(contentWidget);

    auto* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NearEventDesk::updateNearestEvent);
    timer->start(5000);

    updateNearestEvent();
}

void NearEventDesk::updateNearestEvent()
{
    QFile file(m_jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray goals = doc.object()["goals"].toArray();

    QDateTime now = QDateTime::currentDateTime();
    QDateTime nearest;
    QString nearestName;

    for (const QJsonValue& val : goals) {
        QJsonObject obj = val.toObject();

        QString deadlineStr = obj["deadline"].toString();
        if (deadlineStr.isEmpty())
            continue;

        QDateTime deadline =
            QDateTime::fromString(deadlineStr, Qt::ISODate);

        if (!deadline.isValid())
            continue;

        if (deadline > now &&
            (!nearest.isValid() || deadline < nearest)) {
            nearest = deadline;
            nearestName = obj["name"].toString();
        }
    }

    if (nearest.isValid()) {
        goalLabel->setText(nearestName);
        datetimeLabel->setText(nearest.toString("dd MMMM yyyy, HH:mm"));
    } else {
        goalLabel->setText("Нет ближайших целей");
        datetimeLabel->setText("");
    }
}

void MainWindow::openFolder(bool newness)
{
    QDialog* editFolderWindow = new QDialog;
    editFolderWindow->setModal(true);
    editFolderWindow->resize(518, 207);
    editFolderWindow->setStyleSheet("background-color: rgba(254, 245, 218, 1);");
    QVBoxLayout* layout = new QVBoxLayout;
    QLabel* name = new QLabel("Название");
    name->setStyleSheet("QLabel{font-weight: bold}");
    name->setStyleSheet("QLabel{font-size: 20px;}");
    layout->addWidget(name);
    QLineEdit* text = new QLineEdit;
    text->setStyleSheet("QLineEdit{background-color: rgba(236, 243, 246, 1); border: 2px solid black; font-size: 35px;}");
    text->setPlaceholderText(text->text());
    layout->addWidget(text);
    QWidget* buttons = new QWidget;
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    QWidget* horspacer = new QWidget;
    QPushButton* save = new QPushButton("Сохранить");
    save->setStyleSheet("QPushButton{background-color: rgba(199, 229, 197, 1); font-size: 25px;}");
    QPushButton* cancel = new QPushButton("Отмена");
    cancel->setStyleSheet("QPushButton{background-color: rgba(220, 224, 220, 0.94); font-size: 25px;}");
    buttonsLayout->addWidget(horspacer);
    buttonsLayout->addWidget(save);


    if(newness){
        editFolderWindow->setWindowTitle("Новая папка");
    } else {
        QPushButton* del = new QPushButton("Удалить");
        del->setStyleSheet("QPushButton{background-color: rgba(223, 192, 192, 1); font-size: 25px;}");
        connect(ui->deleteButton, &QPushButton::clicked, this, [=]() {
            QModelIndex idx = foldersOutput->currentIndex();
            if (!idx.isValid())
                return;

            foldersModel->removeAt(idx.row());
            saveFoldersToJson();
        });
    }


    buttonsLayout->addWidget(cancel);
    buttons->setLayout(buttonsLayout);
    layout->addWidget(buttons);
    editFolderWindow->setLayout(layout);
    editFolderWindow->setWindowTitle("Новая папка");
    editFolderWindow->setWindowIcon(QIcon(QString(*mainPathToSource + "\\IMG\\folder.png")));
    editFolderWindow->setVisible(true);

    connect(cancel, &QPushButton::clicked, [editFolderWindow](){editFolderWindow->close();});
    connect(save, &QPushButton::clicked, this, [=]() {
        QString nameText = text->text().trimmed();
        if (nameText.isEmpty())
            return;

        Folder* folder = new Folder;
        folder->id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        folder->name = nameText;

        foldersModel->add(folder);
        saveFoldersToJson();

        editFolderWindow->close();
    });
}

//------------------DATA---------------------------------------------------------------------------------

ChoosableObjectsList::ChoosableObjectsList(QObject* parent)
    : QAbstractListModel(parent)
{
}

int ChoosableObjectsList::rowCount(const QModelIndex&) const{
    return m_items.size();
}

Qt::ItemFlags ChoosableObjectsList::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ChoosableObjectsList::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return {};

    const auto* item = m_items[index.row()];

    switch(role){
        case Qt::DisplayRole:
            return item->name;
        case NameRole:
            return item->name;
        case SelectedRole:
            return item->selected;
        case IdRole:
            return item->id;
        }
    return {};
}

void ChoosableObjectsList::select(int row)
{
    for(int i = 0; i < m_items.size(); ++i){
        m_items[i]->selected = (i == row);
    }
    emit dataChanged(index(0), index(m_items.size() - 1));
}


void ChoosableObjectsList::add(ChoosableObject* item)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.push_back(item);
    endInsertRows();
}


void ChoosableObjectsList::removeAt(int row)
{
    if (row < 0 || row >= m_items.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    delete m_items[row];
    m_items.removeAt(row);
    endRemoveRows();
}


QVariant TagsList::data(const QModelIndex& index, int role) const{
    if(!index.isValid())
        return {};

    auto* tag = dynamic_cast<Tag*>(m_items[index.row()]);
    if(!tag){
        return {};
    }

    switch(role){
        case ColorRole:
            return tag->color;
        case InKanbanRole:
            return tag->in_kanban;
        }
    return ChoosableObjectsList::data(index, role);
}

QVariant GoalsList::data(const QModelIndex& index, int role) const{
    if(!index.isValid())
        return {};

    auto* goal = dynamic_cast<Goal*>(m_items[index.row()]);
    if(!goal){
        return {};
    }

    switch(role){
        case DescriptionRole:
            return goal->description;
        case TypeRole:
            return goal->type;
        case CurrentRole:
            return goal->current;
        case TargetRole:
            return goal->target;
        case SubgoalsRole:
            return goal->subgoalIds;
        case DeadlineRole:
            return goal->deadline;
        case TagRole:
            return goal->tagIds;
        case FolderRole:
            return goal->folderId;
        case ParentRole:
            return goal->parentId;
        }
    return ChoosableObjectsList::data(index, role);
}


TabsList* MainWindow::importTabsFromJson(){
    QFile json(QString(*mainPathToSource + "\\DATA\\TABS.json"));
    if (!json.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return nullptr;
    }
    QJsonDocument doc = QJsonDocument::fromJson(json.readAll());
    QJsonObject root = doc.object();
    QJsonArray tabsArray = root["tabs"].toArray();
    json.close();
    TabsList* tabsModel = new TabsList(this);

    for (const QJsonValue& value : tabsArray) {
        QJsonObject obj = value.toObject();

        Tab* tab = new Tab;
        tab->id = obj["id"].toString();
        tab->name = obj["name"].toString();

        tabsModel->add(tab);
    }

    return tabsModel;
}

TagsList* MainWindow::importTagsFromJson(){
    QFile json(QString(*mainPathToSource + "\\DATA\\TAGS.json"));
    if (!json.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return nullptr;
    }
    QJsonDocument doc = QJsonDocument::fromJson(json.readAll());
    QJsonObject root = doc.object();
    QJsonArray tagsArray = root["tags"].toArray();
    json.close();
    TagsList* tagsModel = new TagsList(this);

    for (const QJsonValue& value : tagsArray) {
        QJsonObject obj = value.toObject();

        Tag* tag = new Tag;
        tag->id = obj["id"].toString();
        tag->name = obj["name"].toString();
        tag->color = obj["color"].toString();

        tagsModel->add(tag);
    }

    return tagsModel;
}

FoldersList* MainWindow::importFoldersFromJson(){
    QFile json(QString(*mainPathToSource + "\\DATA\\FOLDERS.json"));
    if (!json.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return nullptr;
    }
    QJsonDocument doc = QJsonDocument::fromJson(json.readAll());
    QJsonObject root = doc.object();
    QJsonArray tabsArray = root["folders"].toArray();
    json.close();
    FoldersList* foldersModel = new FoldersList(this);

    for (const QJsonValue& value : tabsArray) {
        QJsonObject obj = value.toObject();

        Folder* folder = new Folder;
        folder->id = obj["id"].toString();
        folder->name = obj["name"].toString();

        foldersModel->add(folder);
    }

    return foldersModel;
}


Goal* GoalsList::nearestGoal(const QDateTime& now) const
{
    Goal* nearest = nullptr;
    qint64 minDiff = LLONG_MAX;

    for (auto* item : m_items) {
        auto* goal = dynamic_cast<Goal*>(item);
        if (!goal)
            continue;

        if (!goal->deadline.isValid())
            continue;

        qint64 diff = now.secsTo(goal->deadline);
        if (diff >= 0 && diff < minDiff) {
            minDiff = diff;
            nearest = goal;
        }
    }
    return nearest;
}

//-------------------------------------------------------------------------------




QVector<Goal*> MainWindow::importGoalsFromJson()
{
    QVector<Goal*> result;

    QFile file(*mainPathToSource + "\\DATA\\GOALS.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return result;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray arr = doc.object()["goals"].toArray();

    for (const auto& v : arr) {
        QJsonObject o = v.toObject();

        Goal* g = new Goal;
        g->id = o["id"].toString();
        g->name = o["name"].toString();
        g->description = o["description"].toString();
        g->type = o["type"].toString();
        g->current = o["current"].toInt();
        g->target = o["target"].toInt();
        g->folderId = o["folderId"].toString();
        g->parentId = o["parentId"].toString();
        g->deadline = QDateTime::fromString(o["deadline"].toString(), Qt::ISODate);

        for (auto t : o["tagIds"].toArray())
            g->tagIds << t.toString();

        result.push_back(g);
    }

    return result;
}

void MainWindow::loadNotifications() {
    notifications.clear();

    QString jsonPath = *mainPathToSource + "\\DATA\\NOTIFICATIONS.json";
    QFile jsonFile(jsonPath);

    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(jsonFile.readAll());
    jsonFile.close();

    if (doc.isNull() || !doc.isObject()) {
        return;
    }

    QJsonObject root = doc.object();
    QJsonArray notificationsArray = root["notifications"].toArray();

    for (const QJsonValue& value : notificationsArray) {
        QJsonObject obj = value.toObject();

        Notification notification;
        notification.id = QString::number(QDateTime::currentMSecsSinceEpoch()) +
                         QString::number(QRandomGenerator::global()->generate() % 1000);
        notification.text = obj["text"].toString();
        notification.time = QDateTime::fromString(obj["time"].toString(), Qt::ISODate);
        notification.read = false;

        notifications.append(notification);
    }

    std::sort(notifications.begin(), notifications.end(),
              [](const Notification& a, const Notification& b) {
                  return a.time > b.time;
              });
}


void MainWindow::showNotificationsMenu()
{
    loadNotifications();

    QMenu* notificationsMenu = new QMenu(this);
    notificationsMenu->setMinimumWidth(350);

    notificationsMenu->setStyleSheet(
        "QMenu {"
        "    background-color: rgba(254, 245, 218, 1);"
        "    border: 2px solid #dee2e6;"
        "    border-radius: 8px;"
        "    padding: 0px;"
        "}"
        "QMenu::item {"
        "    padding: 0px;"
        "    margin: 0px;"
        "    border: none;"
        "}"
        "QMenu::separator {"
        "    height: 1px;"
        "    background-color: #dee2e6;"
        "    margin: 5px 10px;"
        "}"
    );

    QWidget* headerWidget = new QWidget;
    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(15, 15, 15, 15);
    headerLayout->setSpacing(5);

    QLabel* titleLabel = new QLabel("Уведомления");
    titleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #212529;"
        "}"
    );
    headerLayout->addWidget(titleLabel);

    int unreadCount = 0;
    for (const Notification& notification : notifications) {
        if (!notification.read) unreadCount++;
    }

    if (unreadCount > 0) {
        QLabel* countLabel = new QLabel(QString("%1 непрочитанных").arg(unreadCount));
        countLabel->setStyleSheet(
            "QLabel {"
            "    font-size: 12px;"
            "    color: #6c757d;"
            "}"
        );
        headerLayout->addWidget(countLabel);
    }

    QWidgetAction* headerAction = new QWidgetAction(notificationsMenu);
    headerAction->setDefaultWidget(headerWidget);
    notificationsMenu->addAction(headerAction);

    notificationsMenu->addSeparator();

    if (notifications.isEmpty()) {
        QWidget* emptyWidget = new QWidget;
        QVBoxLayout* emptyLayout = new QVBoxLayout(emptyWidget);
        emptyLayout->setContentsMargins(30, 40, 30, 40);

        QLabel* emptyIcon = new QLabel("");
        emptyIcon->setAlignment(Qt::AlignCenter);
        emptyIcon->setStyleSheet("font-size: 36px;");

        QLabel* emptyText = new QLabel("Нет уведомлений");
        emptyText->setAlignment(Qt::AlignCenter);
        emptyText->setStyleSheet(
            "QLabel {"
            "    font-size: 14px;"
            "    color: #6c757d;"
            "    margin-top: 10px;"
            "}"
        );

        emptyLayout->addWidget(emptyIcon);
        emptyLayout->addWidget(emptyText);

        QWidgetAction* emptyAction = new QWidgetAction(notificationsMenu);
        emptyAction->setDefaultWidget(emptyWidget);
        notificationsMenu->addAction(emptyAction);
    } else {
        for (const Notification& notification : notifications) {
            QWidget* notificationWidget = new QWidget;
            QVBoxLayout* notificationLayout = new QVBoxLayout(notificationWidget);
            notificationLayout->setContentsMargins(15, 10, 15, 10);
            notificationLayout->setSpacing(5);

            QLabel* textLabel = new QLabel(notification.text);
            textLabel->setWordWrap(true);
            textLabel->setStyleSheet(
                QString(
                    "QLabel {"
                    "    font-size: 14px;"
                    "    color: %1;"
                    "    padding-right: 20px;"
                    "}"
                ).arg(notification.read ? "#6c757d" : "#212529")
            );

            QString timeStr = notification.time.toString("dd MMMM yyyy, hh:mm");
            QLabel* timeLabel = new QLabel(timeStr);
            timeLabel->setStyleSheet(
                "QLabel {"
                "    font-size: 12px;"
                "    color: #6c757d;"
                "    font-style: italic;"
                "}"
            );

            if (!notification.read) {
                QWidget* indicatorWidget = new QWidget;
                indicatorWidget->setFixedSize(8, 8);
                indicatorWidget->setStyleSheet(
                    "QWidget {"
                    "    background-color: #007bff;"
                    "    border-radius: 4px;"
                    "}"
                );
                notificationLayout->addWidget(indicatorWidget, 0, Qt::AlignRight);
            }

            notificationLayout->addWidget(textLabel);
            notificationLayout->addWidget(timeLabel);
            notificationWidget->setCursor(Qt::PointingHandCursor);

            QWidgetAction* notificationAction = new QWidgetAction(notificationsMenu);
            notificationAction->setDefaultWidget(notificationWidget);

            connect(notificationAction, &QAction::triggered, this, [this, notification]() {
                markNotificationAsRead(notification.id);
            });

            notificationsMenu->addAction(notificationAction);

            if (&notification != &notifications.last()) {
                notificationsMenu->addSeparator();
            }
        }

        notificationsMenu->addSeparator();

        QWidget* actionsWidget = new QWidget;
        QHBoxLayout* actionsLayout = new QHBoxLayout(actionsWidget);
        actionsLayout->setContentsMargins(10, 10, 10, 10);
        actionsLayout->setSpacing(10);

        QPushButton* clearButton = new QPushButton("Очистить все");
        clearButton->setStyleSheet(
            "QPushButton {"
            "    background-color: #dc3545;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 4px;"
            "    padding: 6px 12px;"
            "    font-size: 12px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #c82333;"
            "}"
        );

        QPushButton* markAllButton = new QPushButton("Прочитать все");
        markAllButton->setStyleSheet(
            "QPushButton {"
            "    background-color: #28a745;"
            "    color: white;"
            "    border: none;"
            "    border-radius: 4px;"
            "    padding: 6px 12px;"
            "    font-size: 12px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #218838;"
            "}"
        );

        actionsLayout->addWidget(clearButton);
        actionsLayout->addWidget(markAllButton);

        QWidgetAction* actionsAction = new QWidgetAction(notificationsMenu);
        actionsAction->setDefaultWidget(actionsWidget);
        notificationsMenu->addAction(actionsAction);
        connect(clearButton, &QPushButton::clicked, this, [this, notificationsMenu]() {
            clearAllNotifications();
            notificationsMenu->close();
        });

        connect(markAllButton, &QPushButton::clicked, this, [this, notificationsMenu]() {
            for (Notification& notification : notifications) {
                notification.read = true;
            }
            notificationsMenu->close();
            showNotificationsMenu();
        });
    }
    QPoint menuPos = n_noteButton->mapToGlobal(QPoint(0, n_noteButton->height()));
    notificationsMenu->exec(menuPos);
    notificationsMenu->deleteLater();
    n_noteButton->setChecked(false);
    updateNotificationButtonIcon();
}

void MainWindow::markNotificationAsRead(const QString& id) {
    for (Notification& notification : notifications) {
        if (notification.id == id) {
            notification.read = true;
            break;
        }
    }
}

void MainWindow::clearAllNotifications() {
    notifications.clear();
}

void MainWindow::updateNotificationButtonIcon() {
    int unreadCount = 0;
    for (const Notification& notification : notifications) {
        if (!notification.read) unreadCount++;
    }

    if (unreadCount > 0) {
        n_noteButton->setStyleSheet(
            "QPushButton {"
            "    border-radius: 30px;"
            "    background-color: rgba(232, 145, 125, 1);"
            "    border: 3px solid black;"
            "}"
            "QPushButton:checked {"
            "    background-color: rgba(200, 120, 100, 1);"
            "}"
        );
    } else {
        n_noteButton->setStyleSheet(
            "QPushButton {"
            "    border-radius: 30px;"
            "    background-color: rgba(232, 145, 125, 0.7);"
            "    border: 3px solid black;"
            "}"
            "QPushButton:checked {"
            "    background-color: rgba(200, 120, 100, 1);"
            "}"
        );
    }
}



void MainWindow::showFilterMenu()
{
    QMenu* filterMenu = new QMenu(this);
    filterMenu->setMinimumWidth(220);

    filterMenu->setStyleSheet(
        "QMenu {"
        "    background-color: rgba(254, 245, 218, 1);"
        "    border: 2px solid #2196F3;"
        "    border-radius: 10px;"
        "    padding: 0px;"
        "}"
        "QMenu::item {"
        "    padding: 12px 20px;"
        "    margin: 0px;"
        "    border-radius: 0px;"
        "    font-size: 14px;"
        "    color: #333333;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #E3F2FD;"
        "    color: #1976D2;"
        "}"
        "QMenu::separator {"
        "    height: 1px;"
        "    background-color: #E0E0E0;"
        "    margin: 5px 0px;"
        "}"
        "QMenu::indicator {"
        "    width: 16px;"
        "    height: 16px;"
        "}"
    );

    // Заголовок меню
    QWidget* headerWidget = new QWidget;
    QVBoxLayout* headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(15, 15, 15, 10);
    headerLayout->setSpacing(5);

    // Иконка и заголовок
    QWidget* titleContainer = new QWidget;
    QHBoxLayout* titleLayout = new QHBoxLayout(titleContainer);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* iconLabel = new QLabel("");
    iconLabel->setStyleSheet("font-size: 18px;");

    QLabel* titleLabel = new QLabel("Срок выполнения");
    titleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    color: #1976D2;"
        "}"
    );

    titleLayout->addWidget(iconLabel);
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    QLabel* subtitleLabel = new QLabel("Фильтр по дате дедлайна");
    subtitleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 12px;"
        "    color: #757575;"
        "    padding-top: 5px;"
        "}"
    );

    headerLayout->addWidget(titleContainer);
    headerLayout->addWidget(subtitleLabel);

    QWidgetAction* headerAction = new QWidgetAction(filterMenu);
    headerAction->setDefaultWidget(headerWidget);
    filterMenu->addAction(headerAction);

    filterMenu->addSeparator();

    QActionGroup* dateGroup = new QActionGroup(filterMenu);
    dateGroup->setExclusive(true);

    static QString currentDateFilter = "all";

    auto createDateAction = [&](const QString& text, const QString& value, const QString& emoji = "") {
        QString actionText = emoji.isEmpty() ? text : QString("%1 %2").arg(emoji).arg(text);
        QAction* action = new QAction(actionText, filterMenu);
        action->setCheckable(true);
        action->setChecked(value == currentDateFilter);
        action->setData(value);
        dateGroup->addAction(action);
        return action;
    };

    QAction* allDates = createDateAction("Все задачи", "all", "");
    filterMenu->addAction(allDates);

    QAction* today = createDateAction("Сегодня", "today", "");
    filterMenu->addAction(today);

    QAction* tomorrow = createDateAction("Завтра", "tomorrow", "");
    filterMenu->addAction(tomorrow);

    QAction* thisWeek = createDateAction("На этой неделе", "week", "");
    filterMenu->addAction(thisWeek);

    QAction* thisMonth = createDateAction("В этом месяце", "month", "");
    filterMenu->addAction(thisMonth);

    filterMenu->addSeparator();

    QAction* overdue = createDateAction("Просроченные", "overdue", "");
    filterMenu->addAction(overdue);

    QAction* noDate = createDateAction("Без срока", "no_date", "∞");
    filterMenu->addAction(noDate);

    filterMenu->addSeparator();

    QWidget* customWidget = new QWidget;
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(10, 10, 10, 10);
    customLayout->setSpacing(5);

    QLabel* customLabel = new QLabel("Произвольный период:");
    customLabel->setStyleSheet("font-weight: bold; font-size: 13px; color: #555;");
    customLayout->addWidget(customLabel);

    QWidget* periodButtonsWidget = new QWidget;
    QHBoxLayout* periodButtonsLayout = new QHBoxLayout(periodButtonsWidget);
    periodButtonsLayout->setContentsMargins(0, 0, 0, 0);
    periodButtonsLayout->setSpacing(5);

    QPushButton* lastWeekBtn = new QPushButton("Неделя");
    QPushButton* lastMonthBtn = new QPushButton("Месяц");
    QPushButton* lastYearBtn = new QPushButton("Год");

    QString buttonStyle =
        "QPushButton {"
        "    padding: 6px 10px;"
        "    border: 1px solid #BBDEFB;"
        "    border-radius: 4px;"
        "    background-color: #E3F2FD;"
        "    font-size: 12px;"
        "    color: #1976D2;"
        "}"
        "QPushButton:hover {"
        "    background-color: #BBDEFB;"
        "}";

    lastWeekBtn->setStyleSheet(buttonStyle);
    lastMonthBtn->setStyleSheet(buttonStyle);
    lastYearBtn->setStyleSheet(buttonStyle);

    periodButtonsLayout->addWidget(lastWeekBtn);
    periodButtonsLayout->addWidget(lastMonthBtn);
    periodButtonsLayout->addWidget(lastYearBtn);

    customLayout->addWidget(periodButtonsWidget);

    QWidgetAction* customAction = new QWidgetAction(filterMenu);
    customAction->setDefaultWidget(customWidget);
    filterMenu->addAction(customAction);

    filterMenu->addSeparator();

    QWidget* buttonsWidget = new QWidget;
    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);
    buttonsLayout->setContentsMargins(10, 10, 10, 10);
    buttonsLayout->setSpacing(10);

    QPushButton* resetButton = new QPushButton("Сбросить");
    QPushButton* applyButton = new QPushButton("Применить");

    resetButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #F5F5F5;"
        "    color: #757575;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 6px;"
        "    padding: 8px 15px;"
        "    font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #E0E0E0;"
        "}"
    );

    applyButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    padding: 8px 20px;"
        "    font-size: 13px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
    );

    buttonsLayout->addWidget(resetButton);
    buttonsLayout->addWidget(applyButton);

    QWidgetAction* buttonsAction = new QWidgetAction(filterMenu);
    buttonsAction->setDefaultWidget(buttonsWidget);
    filterMenu->addAction(buttonsAction);

    // Подключаем сигналы
    connect(dateGroup, &QActionGroup::triggered, this, [&](QAction* action) {
        currentDateFilter = action->data().toString();
    });



    QPoint menuPos = ui->filterButton->mapToGlobal(QPoint(0, ui->filterButton->height()));
    filterMenu->exec(menuPos);

    ui->filterButton->setChecked(false);

    filterMenu->deleteLater();
}

void MainWindow::applyFilters(const QStringList& filters)
{

    qDebug() << "Применяем фильтры:" << filters;

    // if (goalsFilterModel) {
    //     goalsFilterModel->setFilterData(filters);
    //     goalsFilterModel->invalidateFilter();
    // }
}


AppSettings MainWindow::loadSettings()
{
    AppSettings settings;
    settings.theme = "light";
    settings.output = "list";

    QString settingsPath = *mainPathToSource + "\\DATA\\SETTINGS.json";
    QFile file(settingsPath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if (!doc.isNull() && doc.isObject()) {
            QJsonObject obj = doc.object();
            settings.theme = obj["theme"].toString();
            settings.output = obj["output"].toString();
        }
    }

    return settings;
}

void MainWindow::saveSettings(const AppSettings& settings)
{
    QJsonObject obj;
    obj["theme"] = settings.theme;
    obj["output"] = settings.output;

    QJsonDocument doc(obj);

    QString settingsPath = *mainPathToSource + "\\DATA\\SETTINGS.json";
    QFile file(settingsPath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void MainWindow::applySettings(const AppSettings& settings)
{
    if (settings.theme == "dark") {
        ui->centralWidget->setStyleSheet("QWidget{background-color: rgba(50, 50, 50, 1);}");
        ui->HeadWidget->setStyleSheet("QWidget{background-color: rgba(70, 70, 70, 1); border: 3px solid white;}");
    } else {
        ui->centralWidget->setStyleSheet("QWidget{background-color: rgba(238, 232, 211, 1);}");
        ui->HeadWidget->setStyleSheet("QWidget{background-color: rgba(221, 205, 179, 1); border: 3px solid black;}");
    }

    currentSettings = settings;
}

void MainWindow::openSettings()
{
    QDialog* settingsDialog = new QDialog(this);
    settingsDialog->setWindowTitle("Настройки");
    settingsDialog->setModal(true);
    settingsDialog->resize(500, 600);
    settingsDialog->setStyleSheet("background-color: rgba(254, 245, 218, 1);");

    QVBoxLayout* mainLayout = new QVBoxLayout(settingsDialog);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Загружаем текущие настройки
    AppSettings settings = loadSettings();

    // === ТЕМА ===
    QLabel* themeLabel = new QLabel("Тема:");
    themeLabel->setStyleSheet("QLabel{font-weight: bold; font-size: 18px;}");
    mainLayout->addWidget(themeLabel);

    QWidget* themeWidget = new QWidget;
    QHBoxLayout* themeLayout = new QHBoxLayout(themeWidget);
    themeLayout->setContentsMargins(0, 0, 0, 0);

    QRadioButton* lightTheme = new QRadioButton("Светлая");
    QRadioButton* darkTheme = new QRadioButton("Тёмная");

    lightTheme->setChecked(settings.theme == "light");
    darkTheme->setChecked(settings.theme == "dark");

    lightTheme->setStyleSheet("QRadioButton{font-size: 16px; spacing: 10px;}");
    darkTheme->setStyleSheet("QRadioButton{font-size: 16px; spacing: 10px;}");

    themeLayout->addWidget(lightTheme);
    themeLayout->addWidget(darkTheme);
    themeLayout->addStretch();

    mainLayout->addWidget(themeWidget);

    // === ОТОБРАЖЕНИЕ ЦЕЛЕЙ ===
    QLabel* outputLabel = new QLabel("Отображение целей:");
    outputLabel->setStyleSheet("QLabel{font-weight: bold; font-size: 18px;}");
    mainLayout->addWidget(outputLabel);

    QWidget* outputWidget = new QWidget;
    QHBoxLayout* outputLayout = new QHBoxLayout(outputWidget);
    outputLayout->setContentsMargins(0, 0, 0, 0);

    QRadioButton* listOutput = new QRadioButton("Список");
    QRadioButton* tilesOutput = new QRadioButton("Плитки");

    listOutput->setChecked(settings.output == "list");
    tilesOutput->setChecked(settings.output == "tiles");

    listOutput->setStyleSheet("QRadioButton{font-size: 16px; spacing: 10px;}");
    tilesOutput->setStyleSheet("QRadioButton{font-size: 16px; spacing: 10px;}");

    outputLayout->addWidget(listOutput);
    outputLayout->addWidget(tilesOutput);
    outputLayout->addStretch();

    mainLayout->addWidget(outputWidget);
    QLabel* foldersLabel = new QLabel("Папки");
    foldersLabel->setStyleSheet("QLabel{font-weight: bold; font-size: 18px;}");
    mainLayout->addWidget(foldersLabel);

    // Загружаем папки из JSON
    QJsonArray foldersArray;
    QString foldersPath = *mainPathToSource + "\\DATA\\FOLDERS.json";
    QFile foldersFile(foldersPath);

    if (foldersFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument doc = QJsonDocument::fromJson(foldersFile.readAll());
        foldersFile.close();

        if (!doc.isNull() && doc.isObject()) {
            foldersArray = doc.object()["folders"].toArray();
        }
    }

    QWidget* foldersContainer = new QWidget;
    QVBoxLayout* foldersLayout = new QVBoxLayout(foldersContainer);
    foldersLayout->setSpacing(5);

    for (const QJsonValue& value : foldersArray) {
        QJsonObject obj = value.toObject();
        QString name = obj["name"].toString();

        QCheckBox* folderCheck = new QCheckBox(name);
        folderCheck->setChecked(true); // По умолчанию все выбраны
        folderCheck->setStyleSheet("QCheckBox{font-size: 16px; spacing: 10px;}");
        foldersLayout->addWidget(folderCheck);
    }

    mainLayout->addWidget(foldersContainer);

    QLabel* tagsHeader = new QLabel("Тэги");
    tagsHeader->setStyleSheet("QLabel{font-weight: bold; font-size: 18px;}");
    mainLayout->addWidget(tagsHeader);

    QWidget* tagsHeaders = new QWidget;
    QHBoxLayout* tagsHeadersLayout = new QHBoxLayout(tagsHeaders);

    QLabel* nameHeader = new QLabel("Название");
    nameHeader->setStyleSheet("QLabel{font-weight: bold; font-size: 16px;}");
    QLabel* kanbanHeader = new QLabel("В канбане");
    kanbanHeader->setStyleSheet("QLabel{font-weight: bold; font-size: 16px;}");

    tagsHeadersLayout->addWidget(nameHeader, 2);
    tagsHeadersLayout->addWidget(kanbanHeader, 1);

    mainLayout->addWidget(tagsHeaders);

    QJsonArray tagsArray;
    QString tagsPath = *mainPathToSource + "\\DATA\\TAGS.json";
    QFile tagsFile(tagsPath);

    if (tagsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument doc = QJsonDocument::fromJson(tagsFile.readAll());
        tagsFile.close();

        if (!doc.isNull() && doc.isObject()) {
            tagsArray = doc.object()["tags"].toArray();
        }
    }

    QWidget* tagsContainer = new QWidget;
    QVBoxLayout* tagsLayout = new QVBoxLayout(tagsContainer);
    tagsLayout->setSpacing(10);

    for (const QJsonValue& value : tagsArray) {
        QJsonObject obj = value.toObject();
        QString name = obj["name"].toString();
        bool inKanban = obj["in_kanban"].toBool();

        QWidget* tagRow = new QWidget;
        QHBoxLayout* tagRowLayout = new QHBoxLayout(tagRow);
        tagRowLayout->setContentsMargins(0, 0, 0, 0);

        QLabel* tagName = new QLabel(name);
        tagName->setStyleSheet("QLabel{font-size: 16px;}");

        QCheckBox* kanbanCheck = new QCheckBox;
        kanbanCheck->setChecked(inKanban);
        kanbanCheck->setStyleSheet("QCheckBox{spacing: 10px;}");

        tagRowLayout->addWidget(tagName, 2);
        tagRowLayout->addWidget(kanbanCheck, 1, Qt::AlignCenter);

        tagsLayout->addWidget(tagRow);
    }

    mainLayout->addWidget(tagsContainer);
    mainLayout->addStretch();

    QWidget* buttonsWidget = new QWidget;
    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);
    buttonsLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton* saveButton = new QPushButton("Сохранить");
    QPushButton* cancelButton = new QPushButton("Отмена");

    saveButton->setStyleSheet(
        "QPushButton {"
        "    background-color: rgba(165, 224, 155, 1);"
        "    border: 2px solid black;"
        "    font-size: 18px;"
        "    padding: 10px 30px;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(145, 204, 135, 1);"
        "}"
    );

    cancelButton->setStyleSheet(
        "QPushButton {"
        "    background-color: rgba(239, 85, 85, 0.3);"
        "    border: 2px solid black;"
        "    font-size: 18px;"
        "    padding: 10px 30px;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(219, 65, 65, 0.4);"
        "}"
    );

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(cancelButton);

    mainLayout->addWidget(buttonsWidget);

    connect(cancelButton, &QPushButton::clicked, settingsDialog, &QDialog::reject);
    connect(saveButton, &QPushButton::clicked, [=]() {
        AppSettings newSettings;
        newSettings.theme = lightTheme->isChecked() ? "light" : "dark";
        newSettings.output = listOutput->isChecked() ? "list" : "tiles";

        saveSettings(newSettings);
        applySettings(newSettings);

        QJsonArray updatedTagsArray;
        for (int i = 0; i < tagsArray.size(); ++i) {
            QJsonObject tagObj = tagsArray[i].toObject();
            QWidget* tagRow = qobject_cast<QWidget*>(tagsLayout->itemAt(i)->widget());
            if (tagRow) {
                QCheckBox* kanbanCheck = tagRow->findChild<QCheckBox*>();
                if (kanbanCheck) {
                    tagObj["in_kanban"] = kanbanCheck->isChecked();
                }
            }
            updatedTagsArray.append(tagObj);
        }

        QJsonObject tagsRoot;
        tagsRoot["tags"] = updatedTagsArray;

        QFile tagsFile(tagsPath);
        if (tagsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QJsonDocument doc(tagsRoot);
            tagsFile.write(doc.toJson(QJsonDocument::Indented));
            tagsFile.close();
        }

        settingsDialog->accept();
        QMessageBox::information(this, "Сохранено", "Настройки успешно сохранены!");
    });

    settingsDialog->exec();
    settingsDialog->deleteLater();
}


void MainWindow::openAbout()
{
    QDialog* aboutDialog = new QDialog(this);
    aboutDialog->setWindowTitle("Справка");
    aboutDialog->setModal(false);
    aboutDialog->setMinimumSize(500, 700);
    aboutDialog->setStyleSheet(R"(
        QDialog {
            background-color: rgba(254, 245, 218, 1);
        }
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        QScrollArea > QWidget > QWidget {
            background-color: transparent;
        }
    )");

    QVBoxLayout* mainLayout = new QVBoxLayout(aboutDialog);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QWidget* contentWidget = new QWidget;
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(40, 30, 40, 30);
    contentLayout->setSpacing(30);

    QLabel* titleLabel = new QLabel("# Справка");
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 28px;
            font-weight: bold;
            color: #333333;
            margin-bottom: 20px;
        }
    )");
    contentLayout->addWidget(titleLabel);

    QLabel* topPanelLabel = new QLabel("## Верхняя панель");
    topPanelLabel->setStyleSheet(R"(
        QLabel {
            font-size: 22px;
            font-weight: bold;
            color: #2c3e50;
            margin-top: 10px;
            margin-bottom: 10px;
        }
    )");
    contentLayout->addWidget(topPanelLabel);

    QLabel* topPanelText = new QLabel(
        "<p><b>Доска \"Ближайшее событие\"</b></p>"
        "<p>Кнопка уведомлений  Настройки  </p>"
    );
    topPanelText->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            color: #34495e;
            line-height: 1.6;
            margin-left: 15px;
        }
    )");
    topPanelText->setWordWrap(true);
    contentLayout->addWidget(topPanelText);

    QLabel* middlePanelLabel = new QLabel("## Средняя панель");
    middlePanelLabel->setStyleSheet(R"(
        QLabel {
            font-size: 22px;
            font-weight: bold;
            color: #2c3e50;
            margin-top: 10px;
            margin-bottom: 10px;
        }
    )");
    contentLayout->addWidget(middlePanelLabel);

    QLabel* middlePanelText = new QLabel(
        "<p>Фильтр  Удаление  Папки  </p>"
    );
    middlePanelText->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            color: #34495e;
            line-height: 1.6;
            margin-left: 15px;
        }
    )");
    middlePanelText->setWordWrap(true);
    contentLayout->addWidget(middlePanelText);

    QLabel* goalsLabel = new QLabel("## Работа с целями");
    goalsLabel->setStyleSheet(R"(
        QLabel {
            font-size: 22px;
            font-weight: bold;
            color: #2c3e50;
            margin-top: 10px;
            margin-bottom: 10px;
        }
    )");
    contentLayout->addWidget(goalsLabel);

    QLabel* goalsText = new QLabel(
        "<p>На этой доске отображается текущая дата и время. "
        "По сути она играет роль часов. Также по нажатии на неё "
        "открывается окно с итогами дня.</p>"
    );
    goalsText->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            color: #34495e;
            line-height: 1.6;
            margin-left: 15px;
            margin-bottom: 20px;
        }
    )");
    goalsText->setWordWrap(true);
    contentLayout->addWidget(goalsText);

    QFrame* line1 = new QFrame;
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    line1->setStyleSheet("QFrame { border: 1px solid #bdc3c7; margin: 10px 0; }");
    contentLayout->addWidget(line1);

    QLabel* nearestEventLabel = new QLabel("## Доска \"Ближайшее событие\"");
    nearestEventLabel->setStyleSheet(R"(
        QLabel {
            font-size: 22px;
            font-weight: bold;
            color: #2c3e50;
            margin-top: 10px;
            margin-bottom: 10px;
        }
    )");
    contentLayout->addWidget(nearestEventLabel);

    QWidget* exampleWidget = new QWidget;
    exampleWidget->setStyleSheet(R"(
        QWidget {
            background-color: rgba(230, 198, 145, 1);
            border: 3px solid black;
            border-radius: 5px;
            padding: 15px;
        }
    )");

    QVBoxLayout* exampleLayout = new QVBoxLayout(exampleWidget);
    exampleLayout->setSpacing(5);

    QLabel* exampleDate = new QLabel("15 декабря, 2025");
    exampleDate->setStyleSheet(R"(
        QLabel {
            font-size: 18px;
            font-weight: normal;
            color: #000000;
            text-align: center;
        }
    )");

    QLabel* exampleTime = new QLabel("11:40");
    exampleTime->setStyleSheet(R"(
        QLabel {
            font-size: 24px;
            font-weight: bold;
            color: #000000;
            text-align: center;
        }
    )");

    QLabel* exampleTask = new QLabel("Зачёт по экономике");
    exampleTask->setStyleSheet(R"(
        QLabel {
            font-size: 20px;
            font-weight: normal;
            color: #000000;
            text-align: center;
        }
    )");

    exampleLayout->addWidget(exampleDate);
    exampleLayout->addWidget(exampleTime);
    exampleLayout->addWidget(exampleTask);

    contentLayout->addWidget(exampleWidget);

    QLabel* nearestEventText = new QLabel(
        "<p>Дачная доска показывает задачу, срок которой ближе всего к данному моменту времени. "
        "При нажатии на эту доску открывается окно этой задачи.</p>"
    );
    nearestEventText->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            color: #34495e;
            line-height: 1.6;
            margin-left: 15px;
            margin-top: 15px;
        }
    )");
    nearestEventText->setWordWrap(true);
    contentLayout->addWidget(nearestEventText);

    // Разделительная линия
    QFrame* line2 = new QFrame;
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    line2->setStyleSheet("QFrame { border: 1px solid #bdc3c7; margin: 10px 0; }");
    contentLayout->addWidget(line2);

    // Раздел "Кнопка уведомлений"
    QLabel* notificationsLabel = new QLabel("## Кнопка уведомлений");
    notificationsLabel->setStyleSheet(R"(
        QLabel {
            font-size: 22px;
            font-weight: bold;
            color: #2c3e50;
            margin-top: 10px;
            margin-bottom: 10px;
        }
    )");
    contentLayout->addWidget(notificationsLabel);

    // Виджет для примера уведомления
    QWidget* notificationExampleWidget = new QWidget;
    notificationExampleWidget->setStyleSheet(R"(
        QWidget {
            background-color: rgba(232, 145, 125, 0.8);
            border: 2px solid #000000;
            border-radius: 8px;
            padding: 10px;
        }
    )");

    QVBoxLayout* notificationLayout = new QVBoxLayout(notificationExampleWidget);
    notificationLayout->setSpacing(3);

    QLabel* notificationDate = new QLabel("12 декабря, 2025");
    notificationDate->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            font-weight: normal;
            color: #000000;
        }
    )");

    QLabel* notificationDay = new QLabel("пятница");
    notificationDay->setStyleSheet(R"(
        QLabel {
            font-size: 14px;
            font-weight: normal;
            color: #000000;
            font-style: italic;
        }
    )");

    notificationLayout->addWidget(notificationDate);
    notificationLayout->addWidget(notificationDay);

    contentLayout->addWidget(notificationExampleWidget);

    QPushButton* closeButton = new QPushButton("Закрыть");
    closeButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(0, 186, 255, 1);
            color: white;
            border: 2px solid #000000;
            border-radius: 5px;
            font-size: 18px;
            font-weight: bold;
            padding: 10px 40px;
            margin-top: 20px;
        }
        QPushButton:hover {
            background-color: rgba(0, 166, 235, 1);
        }
        QPushButton:pressed {
            background-color: rgba(0, 146, 215, 1);
        }
    )");

    connect(closeButton, &QPushButton::clicked, aboutDialog, &QDialog::close);

    // Центрируем кнопку
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();
    contentLayout->addLayout(buttonLayout);

    // Добавляем растяжку в конец
    contentLayout->addStretch();

    // Устанавливаем контент в scroll area
    scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(scrollArea);

    aboutDialog->setWindowIcon(QIcon(QString(*mainPathToSource + "\\IMG\\info.png")));

    aboutDialog->show();

    QRect mainGeometry = this->geometry();
    int x = mainGeometry.x() + (mainGeometry.width() - aboutDialog->width()) / 2;
    int y = mainGeometry.y() + (mainGeometry.height() - aboutDialog->height()) / 2;
    aboutDialog->move(x, y);
}


void MainWindow::saveGoalsToJson()
{
    QJsonArray arr;

    for (Goal* g : goalsModel->goals()) {
        QJsonObject o;
        o["id"] = g->id;
        o["name"] = g->name;
        o["description"] = g->description;
        o["type"] = g->type;
        o["deadline"] = g->deadline.toString(Qt::ISODate);
        o["tagIds"] = QJsonArray::fromStringList(g->tagIds);
        o["folderId"] = g->folderId;

        arr.append(o);
    }

    QJsonObject root;
    root["goals"] = arr;

    QFile f(*mainPathToSource + "\\DATA\\GOALS.json");
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    f.close();
}


void MainWindow::openCreateGoalDialog()
{
    GoalEdit dlg(this);

    if (dlg.exec() == QDialog::Accepted) {
        Goal* goal = dlg.createGoal();
        goalsModel->addGoal(goal);
        saveGoalsToJson();
    }
}

void GoalsTableModel::addGoal(Goal* goal)
{
    int row = m_goals.size();

    beginInsertRows(QModelIndex(), row, row);
    m_goals.append(goal);
    endInsertRows();
}

void MainWindow::openNearGoal(){

}
void MainWindow::endDay(){

}

const QVector<Goal*>& GoalsTableModel::goals() const
{
    return m_goals;
}
