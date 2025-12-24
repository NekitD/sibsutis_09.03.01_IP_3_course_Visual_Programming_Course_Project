#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QListView>
#include <QTableView>
#include <QStyledItemDelegate>
#include <QStackedWidget>
#include "objects.h"
#include "delegates.h"
#include "lists.h"
#include "models.h"


namespace Ui {
class MainWindow;
}

class TimeDesk;
class NearEventDesk;



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
    QVector<Goal*> allGoals;

    GoalsTableModel* mainGoalsModel;

    GoalsTableModel* todayModel;
    GoalsTableModel* incomingModel;
    GoalsTableModel* calendarModel;
    GoalsTableModel* kanbanModel;

    void refreshAllModels();
    void createPagesWithoutOwnership();

    QWidget* createIncomingPage(const QVector<Goal*>&);
    QWidget* createTodayPage(const QVector<Goal*>&);
    QWidget* createKanbanColumn(const QString& tagName, const QVector<Goal*>& goals);
    QWidget* createCalendarPage(const QVector<Goal*>&);
    QWidget* createKanbanPage(const QVector<Goal*>&);
    void onTabChanged(const QString& tabId);

    QMap<QString, QWidget*> tagPages;
       QMap<QString, GoalsTableModel*> tagModels;

    QWidget* createTagPage(const QString& tagId, const QString& tagName, const QString& tagColor);
    void onTagSelected(const QString& tagId, const QString& tagName, const QString& tagColor);

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

    QString selectedGoalId;
    GoalsTableModel* currentActiveModel;

    void updateDeleteButtonState();

    bool deleteGoalById(const QString& goalId);

    void deleteSubgoals(const QStringList& subgoalIds);
    void updateAllModels();

    QTableView* incomingView = nullptr;
    QTableView* todayView = nullptr;
    QListView* calendarList = nullptr;
    QListView* receivedView = nullptr;
    QListView* inProgressView = nullptr;
    QListView* doneView = nullptr;

    QList<GoalsTableModel*> kanbanModels;

    void clearAllSelections();

    QTimer* clickTimer = nullptr;
    QString lastClickedGoalId;
    int clickCount = 0;

    void onSingleClick();
    void clearCurrentSelection(const QString& goalId);

    void setupContextMenuForViews();
    void showContextMenu(const QPoint& pos);

    Goal* findGoalById(const QString& goalId);

    void markGoalAsCompleted(const QString& goalId);

    void openEditGoal(const QString& goalId);

    void dataChangedForAllModels();

private slots:
    void openFolder(bool newness);
    void openAbout();
    void openNearGoal();
    void endDay();
    void openAddGoal(bool newness);
    void openCreateGoalDialog();
    void onGoalSelected(const QModelIndex& current, const QModelIndex& previous);
    void deleteSelectedGoal();

signals:
    void modelsDataChanged();
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

#endif // MAINWINDOW_H
