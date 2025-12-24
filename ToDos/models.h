#ifndef MODELS_H
#define MODELS_H

#include "QSortFilterProxyModel"
#include <QDate>
#include "objects.h"
#include "delegates.h"

class GoalsFilterModel;
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

    enum {
        TagRole = Qt::UserRole + 1,
        DeadlineRole,
        FolderRole
    };

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
};



#endif // MODELS_H
