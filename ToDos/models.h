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



#endif // MODELS_H
