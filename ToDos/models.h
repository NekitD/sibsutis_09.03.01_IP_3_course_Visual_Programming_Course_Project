#ifndef MODELS_H
#define MODELS_H

#include "QSortFilterProxyModel"
#include <QDate>
#include "objects.h"
#include "delegates.h"

class GoalsTableModel;



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


class GoalsTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit GoalsTableModel(QObject* parent = nullptr);
    //~GoalsTableModel() override;

    int rowCount(const QModelIndex& = QModelIndex()) const override;
    int columnCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    const QVector<Goal*>& goals() const;

    void setGoals(const QVector<Goal*>& goals);
    void addGoal(Goal* g);


    bool removeGoal(const QString& goalId);


    Goal* findGoalById(const QString& goalId) const;
    int findRowById(const QString& goalId) const;


    enum GoalsRoles {
            // Базовые роли (Qt уже имеет DisplayRole, EditRole, etc)
            IdRole = Qt::UserRole + 10,      // Начинаем с 10 для безопасности
            NameRole = Qt::UserRole + 11,
            DescriptionRole = Qt::UserRole + 12,
            TypeRole = Qt::UserRole + 13,
            CurrentRole = Qt::UserRole + 14,
            TargetRole = Qt::UserRole + 15,
            DeadlineRole = Qt::UserRole + 16,
            TagRole = Qt::UserRole + 17,
            FolderRole = Qt::UserRole + 18,
            ParentRole = Qt::UserRole + 19,
            SubgoalsRole = Qt::UserRole + 20
        };

     Q_ENUM(GoalsRoles)

    void setFilterTodayOnly(bool todayOnly);
    void setFilterTag(const QString& tag);
    void setFilterFolder(const QString& folder);
    void setFilterDate(const QDate& date);

    void setGoalSource(const QVector<Goal*>& goals);

    void applyFilters();

    const QVector<Goal*>* m_sourceGoals = nullptr; // Указатель на чужие данные
    QVector<Goal*> m_filteredGoals;

    bool m_todayOnly = false;
    QString m_tagFilter;
    QString m_folderFilter;
    QDate m_dateFilter;

signals:
    void dataChangedExternally();
};



#endif // MODELS_H
