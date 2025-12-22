#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QListView>


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
    void saveFoldersToJson();


private slots:
    void openFolder(bool newness);


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

class GoalsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit GoalsFilterModel(QObject* parent = nullptr);

    void setCurrentTab(const QString& tabId);
    void setCurrentTag(const QString& tagId);
    void setCurrentFolder(const QString& folderId);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    QString m_tabId;
    QString m_tagId;
    QString m_folderId;
};


#endif // MAINWINDOW_H
