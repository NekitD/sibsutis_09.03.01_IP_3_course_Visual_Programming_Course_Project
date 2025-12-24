#include "models.h"
#include <QDebug>



//------------------------------------------------------------------------------------------------

int GoalsTableModel::columnCount(const QModelIndex&) const{
    return 4;
}

GoalsTableModel::GoalsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    qDebug() << "GoalsTableModel constructor called";
}


void GoalsTableModel::setFilterTodayOnly(bool todayOnly) {
    m_todayOnly = todayOnly;
}

void GoalsTableModel::setFilterTag(const QString& tag) {
    m_tagFilter = tag;
}

void GoalsTableModel::setFilterFolder(const QString& folder) {
    m_folderFilter = folder;
}

void GoalsTableModel::setFilterDate(const QDate& date) {
    m_dateFilter = date;
}

void GoalsTableModel::setGoalSource(const QVector<Goal*>& goals) {
    beginResetModel();
    m_sourceGoals = &goals;
    applyFilters();
    endResetModel();
}

void GoalsTableModel::applyFilters() {
    qDebug() << "Applying filters: todayOnly=" << m_todayOnly
             << ", tagFilter=" << m_tagFilter
             << ", dateFilter=" << m_dateFilter.toString("yyyy-MM-dd");

    beginResetModel();

    m_filteredGoals.clear();
    if (!m_sourceGoals) {
        qDebug() << "No source goals!";
        endResetModel();
        return;
    }

    qDebug() << "Source has" << m_sourceGoals->size() << "goals";

    for (Goal* goal : *m_sourceGoals) {
        bool accept = true;

        if (m_todayOnly) {
            if (!goal->deadline.isValid() ||
                goal->deadline.date() != QDate::currentDate()) {
                accept = false;
            }
        }

        if (accept && !m_tagFilter.isEmpty()) {
            if (!goal->tagIds.contains(m_tagFilter)) {
                accept = false;
            }
        }

        if (accept && m_dateFilter.isValid()) {
            if (!goal->deadline.isValid() ||
                goal->deadline.date() != m_dateFilter) {
                accept = false;
            }
        }

        if (accept) {
            m_filteredGoals.push_back(goal);
        }
    }

    qDebug() << "Filtered to" << m_filteredGoals.size() << "goals";
    endResetModel();
}


int GoalsTableModel::rowCount(const QModelIndex&) const {
    return m_filteredGoals.size();
}


QVariant GoalsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_filteredGoals.size())
        return {};

    const Goal* g = m_filteredGoals.at(index.row());

    switch(role) {
        case Qt::DisplayRole:
            switch (index.column()) {
                case NameColumn:
                    return g->name;
                case DescriptionColumn:
                    return g->description;
                case DeadlineColumn:
                    return g->deadline.isValid()
                        ? g->deadline.toString("dd.MM.yyyy HH:mm")
                        : "—";
                case StatusColumn:
                    return g->tagIds.join(", ");
            }
            break;

        case IdRole:
            return g->id;

        case NameRole:
            return g->name;

        case DescriptionRole:
            return g->description;

        case TypeRole:
            return g->type;

        case CurrentRole:
            return g->current;

        case TargetRole:
            return g->target;

        case DeadlineRole:
            return g->deadline;

        case TagRole:
            return g->tagIds;

        case FolderRole:
            return g->folderId;

        case ParentRole:
            return g->parentId;

        case SubgoalsRole:
            return g->subgoalIds;
    }

    return {};
}

QVariant GoalsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};

    switch (section) {
        case NameColumn: return "Цель";
        case DescriptionColumn: return "Описание";
        case DeadlineColumn: return "Срок";
        case StatusColumn: return "Статусы";
    }
    return {};
}


bool GoalsTableModel::removeGoal(const QString& goalId)
{
    if (!m_sourceGoals) {
        qDebug() << "No source goals in model";
        return false;
    }

    for (int i = 0; i < m_sourceGoals->size(); ++i) {
        Goal* goal = m_sourceGoals->at(i);
        if (goal->id == goalId) {
            int filteredRow = -1;
            for (int j = 0; j < m_filteredGoals.size(); ++j) {
                if (m_filteredGoals[j] == goal) {
                    filteredRow = j;
                    break;
                }
            }

            if (filteredRow >= 0) {
                beginRemoveRows(QModelIndex(), filteredRow, filteredRow);
                m_filteredGoals.removeAt(filteredRow);
                endRemoveRows();
            } else {
                applyFilters();
            }

            return true;
        }
    }

    qDebug() << "Goal not found in model:" << goalId;
    return false;
}

Goal* GoalsTableModel::findGoalById(const QString& goalId) const
{
    if (!m_sourceGoals)
        return nullptr;

    for (Goal* goal : *m_sourceGoals) {
        if (goal->id == goalId)
            return goal;
    }

    return nullptr;
}

int GoalsTableModel::findRowById(const QString& goalId) const
{
    for (int i = 0; i < m_filteredGoals.size(); ++i) {
        if (m_filteredGoals[i]->id == goalId)
            return i;
    }

    return -1;
}

