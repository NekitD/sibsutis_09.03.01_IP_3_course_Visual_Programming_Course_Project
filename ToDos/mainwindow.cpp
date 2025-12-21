#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QDialog>
#include <QString>

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
    n_noteButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\notif.svg.png")));
    n_noteButton->setIconSize(n_noteButton->size() * 0.9);
    n_noteButton->setToolTip("Уведомления");

    n_settingsButton->setFixedSize(75, 75);
    n_settingsButton->setStyleSheet("border-radius: 25px; background-color: rgba(189, 190, 194, 1);");
    n_settingsButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\settings.png")));
    n_settingsButton->setIconSize(n_settingsButton->size());
    n_settingsButton->setToolTip("Настройки");


    n_aboutButton->setFixedSize(70, 70);
    n_aboutButton->setStyleSheet("QPushButton{border-radius: 35px; background-color: rgba(0, 186, 255, 1); color: white; font-size: 50px;}");
    n_aboutButton->setText("?");
    n_aboutButton->setToolTip("Справка");

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
    ui->searchButton->setText("");
    ui->searchButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\loop.png")));
    ui->searchButton->setIconSize(ui->filterButton->size());
    ui->searchButton->setToolTip("Искать");

    ui->filterButton->setStyleSheet("QPushButton{background-color: rgba(205, 247, 198, 1); border: 2px solid black}");
    ui->filterButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\filter.svg")));
    ui->filterButton->setIconSize(ui->filterButton->size());
    ui->filterButton->setText("");
    ui->filterButton->setToolTip("Фильтр");

    ui->deleteButton->setStyleSheet("QPushButton{background-color: rgba(239, 85, 85, 0.3); border: 2px solid black;}");
    ui->deleteButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\off_trash.png")));
    ui->deleteButton->setIconSize(ui->deleteButton->size() * 0.9);
    ui->deleteButton->setText("");
    ui->deleteButton->setToolTip("Удалить");

    ui->foldersWidget->setStyleSheet("QWidget{background-color: rgba(232, 203, 168, 1); border: 1px solid black;}");

    ui->addFolderButton->setStyleSheet("QPushButton{background-color: rgba(165, 224, 155, 1); border: 2px solid black}");
    ui->addFolderButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\AddFolder.png")));
    ui->addFolderButton->setIconSize(ui->addFolderButton->size() * 0.9);
    ui->addFolderButton->setText("");
    ui->addFolderButton->setToolTip("Новая папка");

    QHBoxLayout* foldersLayout = new QHBoxLayout;
    QListView* foldersOutput = new QListView;
    foldersLayout->addWidget(foldersOutput, 7);
    foldersLayout->addWidget(ui->addFolderButton, 1);
    foldersLayout->setContentsMargins(0, 0, 0, 0);
    foldersLayout->setSpacing(0);
    ui->foldersWidget->setLayout(foldersLayout);

    connect(ui->addFolderButton, &QPushButton::clicked, this, &MainWindow::openAddFolder);
}

MainWindow::~MainWindow()
{
    delete ui;
}


TimeDesk::TimeDesk(QWidget *parent):
    QWidget(parent)
{
    setToolTip("Закончить день");
    setStyleSheet("QWidget{background-color: rgba(230, 198, 145, 1); border: 3px solid black; font-size: 24px;}");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
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
    setToolTip("Открыть ближайшую цель");
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

void MainWindow::openAddFolder()
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
    text->setPlaceholderText("Новая папка");
    layout->addWidget(text);
    QWidget* buttons = new QWidget;
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    QWidget* horspacer = new QWidget;
    QPushButton* save = new QPushButton("Сохранить");
    save->setStyleSheet("QPushButton{background-color: rgba(199, 229, 197, 1); font-size: 25px;}");
    QPushButton* cancel = new QPushButton("Отмена");
    cancel->setStyleSheet("QPushButton{background-color: rgba(223, 192, 192, 1); font-size: 25px;}");
    buttonsLayout->addWidget(horspacer);
    buttonsLayout->addWidget(save);
    buttonsLayout->addWidget(cancel);
    buttons->setLayout(buttonsLayout);
    layout->addWidget(buttons);
    editFolderWindow->setLayout(layout);
    editFolderWindow->setWindowTitle("Новая папка");
    editFolderWindow->setWindowIcon(QIcon(QString(*mainPathToSource + "\\IMG\\folder.png")));
    editFolderWindow->setVisible(true);
    connect(cancel, &QPushButton::clicked, [editFolderWindow](){editFolderWindow->close();});
    connect(save, &QPushButton::clicked, [editFolderWindow](){editFolderWindow->close();});
}

//------------------DATA---------------------------------------------------------------------------------


int ChoosableObjectsList::rowCount(const QModelIndex&) const{
    return m_items.size();
}

Qt::ItemFlags ChoosableObjectsList::flags(const QModelIndex& index) const{

}

QVariant ChoosableObjectsList::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return {};

    const auto* item = m_items[index.row()];

    switch(role){
        case Qt::DisplayRole:
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
