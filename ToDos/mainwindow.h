#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QListView>
#include <QStyledItemDelegate>
#include <QStackedWidget>


namespace Ui {
class MainWindow;
}

class TimeDesk;
class NearEventDesk;

class ChoosableObject;
class Tab;
class Tag;
class Folder;
class Goal;

class ChoosableObjectsList;
class FoldersList;
class TabsList;
class TagsList;
class GoalsList;

class GoalsFilterModel;
class GoalsTableModel;

class Notification;


enum Columns {
    NameColumn,
    DescriptionColumn,
    DeadlineColumn,
    StatusColumn,
    ColumnCount
};


enum TagRoles{
    ColorRole = Qt::UserRole + 10,
    InKanbanRole
};


enum Roles {
    IdRole = Qt::UserRole + 1,
    NameRole,
    SelectedRole,
    TypeRole
};


enum GoalsRoles {
    DescriptionRole = Qt::UserRole + 20,
    DeadlineRole,
    CurrentRole,
    TargetRole,
    SubgoalsRole,
    ParentRole,
    TagRole,
    FolderRole
};


struct AppSettings {
    QString theme;
    QString output;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    QString* mainPathToSource;
    QString foldersJsonPath;
    TimeDesk* n_timeDesk;
    NearEventDesk* n_nearEventDesk;
    QPushButton* n_noteButton;
    QPushButton* n_settingsButton;
    QPushButton* n_aboutButton;

    QDateTime* currentDatetime;
    QDate* currentDate;
    QTime* currentTime;

    FoldersList* foldersModel;
    QListView* foldersOutput;

    TabsList* importTabsFromJson();
    TagsList* importTagsFromJson();
    FoldersList* importFoldersFromJson();
    QVector<Goal*> importGoalsFromJson();
    void saveFoldersToJson();
    void saveGoalsToJson();



    QStackedWidget* goalsStack;

    bool m_todayOnly = false;

    QWidget* todayPage;
    QWidget* incomingPage;
    QWidget* calendarPage;
    QWidget* kanbanPage;

    GoalsTableModel* goalsModel;
    GoalsFilterModel* goalsFilter;

    QWidget* createIncomingPage();
    QWidget* createTodayPage();
    QWidget* createKanbanColumn(const QString& tagName);
    QWidget* createCalendarPage();
    QWidget* createKanbanPage();
    void onTabChanged(const QString& tabId);

    void showNotificationsMenu();
    QList<Notification> notifications;
    void loadNotifications();
    void markNotificationAsRead(const QString& id);
    void clearAllNotifications();
    void updateNotificationButtonIcon();

    void showFilterMenu();
    void applyFilters(const QStringList& filters);

    AppSettings loadSettings();
    void saveSettings(const AppSettings& settings);
    void applySettings(const AppSettings& settings);
    AppSettings currentSettings;
    void openSettings();

    QPushButton* addGoalButton = nullptr;



private slots:
    void openFolder(bool newness);
    void openAbout();
    void openNearGoal();
    void endDay();
    void openAddGoal();
    void openCreateGoalDialog();


};


class TimeDesk : public QWidget
{
    Q_OBJECT
public:
    explicit TimeDesk(QWidget *parent = nullptr);

private slots:
    void updateTime();

private:
    QLabel* dateLabel;
    QLabel* timeLabel;
    QTimer* timer;
};

class NearEventDesk : public QWidget
{
    Q_OBJECT
public:
    explicit NearEventDesk(const QString& jsonPath, QWidget* parent = nullptr);

private slots:
    void updateNearestEvent();

private:
    QString m_jsonPath;
    QLabel* goalLabel;
    QLabel* datetimeLabel;
};


class ChoosableObject
{
public:
    QString id;
    QString name;
    bool selected = false;
    virtual ~ChoosableObject() = default;
};

class Tab : public ChoosableObject
{
};

class Tag : public ChoosableObject
{
public:
    QString color;
    bool in_kanban;
};

class Folder : public ChoosableObject
{
public:
    QList<QString> goalIds;
};

class Goal : public ChoosableObject
{
public:
    QString description;
    QString type;
    QDateTime deadline;

    int current = 0;
    int target = 0;

    QStringList subgoalIds;

    QString parentId;
    QStringList tagIds;
    QString folderId;
};


class ChoosableObjectsList : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ChoosableObjectsList(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void select(int row);
    void add(ChoosableObject* item);
    void removeAt(int row);

    QVector<ChoosableObject*> items() const { return m_items; }

protected:
    QVector<ChoosableObject*> m_items;
};



class FoldersList : public ChoosableObjectsList
{
public:
    explicit FoldersList(QObject* parent = nullptr): ChoosableObjectsList(parent) {}
};

class TabsList : public ChoosableObjectsList
{
public:
    explicit TabsList(QObject* parent = nullptr): ChoosableObjectsList(parent) {}
};

class TagsList : public ChoosableObjectsList
{
public:
    explicit TagsList(QObject* parent = nullptr): ChoosableObjectsList(parent) {}
    QVariant data(const QModelIndex& index, int role) const override;
};

class GoalsList : public ChoosableObjectsList{
public:
    QVariant data(const QModelIndex& index, int role) const override;
    Goal* nearestGoal(const QDateTime& now) const;

};

class GoalsFilterModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit GoalsFilterModel(QObject* parent = nullptr);

    void setTodayOnly(bool on);
    void setTagFilter(const QString& tagId);
    void setFolderFilter(const QString& folderId);
    void setDateFilter(const QDate& date);

protected:
    bool filterAcceptsRow(int row,
                          const QModelIndex& parent) const override;

private:
    bool m_todayOnly = false;
    QString m_tagId;
    QString m_folderId;
    QDate m_date;
};

class GoalsTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit GoalsTableModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    const QVector<Goal*>& goals() const;

    void setGoals(const QVector<Goal*>& goals);
    void addGoal(Goal* g);

    enum {
        TagRole = Qt::UserRole + 1,
        DeadlineRole,
        FolderRole
    };

    QVector<Goal*> m_goals;
};

class GoalsDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit GoalsDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
};


class Notification {
public:
    QString id;
    QString text;
    QDateTime time;
    bool read = false;
};

#endif // MAINWINDOW_H
