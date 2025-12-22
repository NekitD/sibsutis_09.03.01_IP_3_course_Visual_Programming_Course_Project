#include "mainwindow.h"
#include "ui_mainwindow.h"
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


class TabsDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override
    {
        QStyleOptionViewItem opt = option;
        opt.font.setPointSize(14);
        opt.font.setBold(option.state & QStyle::State_Selected);
        opt.displayAlignment = Qt::AlignCenter;
        QStyledItemDelegate::paint(painter, opt, index);
    }
};

class TagsDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override
    {
        QStyleOptionViewItem opt = option;
        opt.font.setPointSize(12);
        opt.font.setBold(option.state & QStyle::State_Selected);
        opt.displayAlignment = Qt::AlignCenter;
        QStyledItemDelegate::paint(painter, opt, index);
    }
};

class FoldersDelegate : public QStyledItemDelegate {
public:
    FoldersDelegate(int itemWidth = 200, int itemHeight = 50, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), width(itemWidth), height(itemHeight) {}

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override
    {
        painter->save();

        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // Настройки шрифта
        QFont font = opt.font;
        font.setPointSize(25);
        font.setBold(opt.state & QStyle::State_Selected);
        opt.font = font;
        opt.displayAlignment = Qt::AlignCenter;

        if (opt.state & QStyle::State_Selected) {
            painter->fillRect(opt.rect, QColor(232, 208, 121));
            painter->setPen(QPen(QColor(0, 0, 0), 2));
            painter->drawRect(opt.rect.adjusted(1, 1, -1, -1));
        }

        // Рисуем текст
        painter->setPen(Qt::black);
        painter->setFont(font);
        painter->drawText(opt.rect, Qt::AlignCenter, opt.text);

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override
    {
        Q_UNUSED(option);
        Q_UNUSED(index);
        return QSize(width, height);
    }

    // Для возможности изменения ширины динамически
    void setItemSize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;
    }

private:
    int width;
    int height;
};

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

    QString tabcolor = "rgba(222, 206, 187, 1)";
    QString ItemStyle = QString(R"(
                         QListView {
                             font-size: 14px;
                             font-family: "Segoe UI";
                             outline: 0;
                         }

                         QListView::item {
                             background-color: %1;
                             border: 3px solid black;
                             padding: 10px;
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

    QString tabItemStyle = QString(ItemStyle).arg(tabcolor);
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
    QString tagItemStyle = QString(ItemStyle).arg(tagcolor);
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

    ui->filterButton->setStyleSheet("QPushButton{background-color: rgba(205, 247, 198, 1); border: 2px solid black}");
    ui->filterButton->setIcon(QIcon(QString(*mainPathToSource + "\\IMG\\filter.svg")));
    ui->filterButton->setIconSize(ui->filterButton->size());
    ui->filterButton->setText("");
    ui->filterButton->setToolTip("Фильтр");

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




    QString folderStyle = QString(R"(
                         QListView {
                             font-size: 14px;
                             font-family: "Segoe UI";
                             outline: 0;
                         }

                         QListView::item {
                             border: 3px solid black;
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
    FoldersList* foldersModel = importFoldersFromJson();
    foldersOutput->setModel(foldersModel);
    foldersOutput->setCurrentIndex(foldersModel->index(0, 0));
    foldersOutput->show();
    foldersLayout->addWidget(foldersOutput, 7);
    foldersLayout->addWidget(ui->addFolderButton, 1);
    foldersLayout->setContentsMargins(0, 0, 0, 0);
    foldersLayout->setSpacing(0);
    ui->foldersWidget->setLayout(foldersLayout);

    connect(ui->addFolderButton, &QPushButton::clicked, this, &MainWindow::openAddFolder);

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
            [tagsOutput](const QModelIndex& current, const QModelIndex&) {
                //if(current.isValid()){
                    auto sm = tagsOutput->selectionModel();
                        sm->blockSignals(true);
                        sm->clearSelection();
                        sm->setCurrentIndex(QModelIndex(), QItemSelectionModel::NoUpdate);
                        sm->blockSignals(false);
                    //};
                });
    connect(tagsOutput->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            [tabsOutput](const QModelIndex& current, const QModelIndex&) {
                //if(current.isValid()){
                    auto sm = tabsOutput->selectionModel();
                                sm->blockSignals(true);
                                sm->clearSelection();
                                sm->setCurrentIndex(QModelIndex(), QItemSelectionModel::NoUpdate);
                                sm->blockSignals(false);
                 //};
            });
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

bool GoalsFilterModel::filterAcceptsRow(int row, const QModelIndex& parent) const
{
    QModelIndex idx = sourceModel()->index(row, 0, parent);

    const QString folderId =
        sourceModel()->data(idx, FolderRole).toString();

    const QStringList tags =
        sourceModel()->data(idx, TagRole).toStringList();

    if (!m_folderId.isEmpty() && folderId != m_folderId)
        return false;

    if (!m_tagId.isEmpty()) {
        if (!tags.contains(m_tagId))
            return false;
    }

    return true;
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
