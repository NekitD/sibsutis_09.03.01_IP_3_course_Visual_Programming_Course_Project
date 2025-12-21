#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QAbstractItemModel>
#include <QDateTime>

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    QString* mainPathToSource;
    TimeDesk* n_timeDesk;
    NearEventDesk* n_nearEventDesk;
    QPushButton* n_noteButton;
    QPushButton* n_settingsButton;
    QPushButton* n_aboutButton;


private slots:
    void openAddFolder();


};


class TimeDesk : public QWidget
{
    Q_OBJECT
public:
    explicit TimeDesk(QWidget *parent = nullptr);
private:
    QString* datetimeOutput;
};

class NearEventDesk : public QWidget
{
    Q_OBJECT
public:
    explicit NearEventDesk(QWidget *parent = nullptr);

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

    QString parentId;
    QList<QString> tagIds;
    QString folderId;
};


class ChoosableObjectsList : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ChoosableObjectsList(QObject* patent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void select(int row);

    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        SelectedRole,
        TypeRole
    };

protected:
    QVector<ChoosableObject*> m_items;
};


class FoldersList : public ChoosableObjectsList
{

};

class TagsList : public ChoosableObjectsList
{
public:
    QVariant data(const QModelIndex& index, int role) const override;
    enum TagRoles{
        ColorRole = Qt::UserRole + 10,
        InKanbanRole
    };
};

class GoalsList : public ChoosableObjectsList{

};


#endif // MAINWINDOW_H
