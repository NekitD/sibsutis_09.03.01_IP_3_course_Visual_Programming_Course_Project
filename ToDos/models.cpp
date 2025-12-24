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

//GoalsTableModel::~GoalsTableModel()
//{
//    // Удаляем только если это главная модель
//    // Но лучше всегда удалять - модель владеет целями
//    //qDeleteAll(m_goals);
//    qDebug() << "GoalsTableModel destructor called";
//    m_goals.clear();
//    m_allGoals.clear();
//    m_filteredGoals.clear();
//}

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
    m_sourceGoals = &goals; // Просто запоминаем, где лежат данные
    applyFilters(); // Фильтруем на основе нового источника
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

        // Фильтр по "сегодня"
        if (m_todayOnly) {
            if (!goal->deadline.isValid() ||
                goal->deadline.date() != QDate::currentDate()) {
                accept = false;
            }
        }

        // Фильтр по тегу
        if (accept && !m_tagFilter.isEmpty()) {
            if (!goal->tagIds.contains(m_tagFilter)) {
                accept = false;
            }
        }

        // Фильтр по дате
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



//void GoalsTableModel::applyFilters() {
//    m_filteredGoals.clear();
//    if (!m_sourceGoals) return;

//    for (Goal* goal : *m_sourceGoals) { // Работаем с чужими данными
//        bool accept = true;
//        // ... ваша логика фильтрации ...
//        if (accept) {
//            m_filteredGoals.push_back(goal); // Сохраняем только указатель
//        }
//    }
//}

//void GoalsTableModel::applyFilters() {
//    qDebug() << "GoalsTableModel::applyFilters called";
//    beginResetModel();

//    m_filteredGoals.clear();

//    for (Goal* goal : m_allGoals) {
//        bool accept = true;

//        if (m_todayOnly) {
//            if (!goal->deadline.isValid() ||
//                goal->deadline.date() != QDate::currentDate()) {
//                accept = false;
//            }
//        }

//        if (accept && !m_tagFilter.isEmpty()) {
//            if (!goal->tagIds.contains(m_tagFilter)) {
//                accept = false;
//            }
//        }

//        if (accept && !m_folderFilter.isEmpty()) {
//            if (goal->folderId != m_folderFilter) {
//                accept = false;
//            }
//        }

//        if (accept && m_dateFilter.isValid()) {
//            if (!goal->deadline.isValid() ||
//                goal->deadline.date() != m_dateFilter) {
//                accept = false;
//            }
//        }

//        if (accept) {
//            m_filteredGoals.push_back(goal);
//        }
//    }

//    endResetModel();
//    qDebug() << "GoalsTableModel::applyFilters finished. Filtered:" << m_filteredGoals.size();
//}

//void GoalsTableModel::setGoals(const QVector<Goal*>& goals)
//{
//    qDebug() << "GoalsTableModel::setGoals called with" << goals.size() << "goals";
//    beginResetModel();

//    // Удаляем старые цели, если они были созданы этой моделью
//    //qDeleteAll(m_goals);
//    //m_goals.clear();

//    // Просто копируем указатели, без создания новых объектов
//    m_sourceGoals = goals;

//    // Копируем также для фильтрации
//    m_sourceGoals = goals;
//    m_filteredGoals = goals;

//    endResetModel();
//    qDebug() << "GoalsTableModel::setGoals finished";
//}

int GoalsTableModel::rowCount(const QModelIndex&) const {
    return m_filteredGoals.size();
}

//QVariant GoalsTableModel::data(const QModelIndex& index, int role) const {
//    if (!index.isValid() || index.row() >= m_filteredGoals.size())
//        return {};

//    const Goal* g = m_filteredGoals.at(index.row());

//    if (role == DeadlineRole)
//        return g->deadline;

//    if (role == TagRole)
//        return g->tagIds;

//    if (role == FolderRole)
//        return g->folderId;

//    if (role == Qt::DisplayRole) {
//        switch (index.column()) {
//            case NameColumn:
//                return g->name;
//            case DescriptionColumn:
//                return g->description;
//            case DeadlineColumn:
//                return g->deadline.isValid()
//                    ? g->deadline.toString("dd.MM.yyyy HH:mm")
//                    : "—";
//            case StatusColumn:
//                return g->tagIds.join(", ");
//        }
//    }
//    return {};
//}

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

//bool GoalsTableModel::removeGoal(const QString& goalId)
//{
//    if (!m_sourceGoals)
//        return false;

//    // Находим цель в источнике данных
//    for (int i = 0; i < m_sourceGoals->size(); ++i) {
//        Goal* goal = m_sourceGoals->at(i);
//        if (goal->id == goalId) {
//            // Находим индекс в отфильтрованном списке
//            int filteredRow = -1;
//            for (int j = 0; j < m_filteredGoals.size(); ++j) {
//                if (m_filteredGoals[j] == goal) {
//                    filteredRow = j;
//                    break;
//                }
//            }

//            // Удаляем из источника (но НЕ удаляем объект - этим займется MainWindow)
//            const_cast<QVector<Goal*>*>(m_sourceGoals)->removeAt(i);

//            // Обновляем отфильтрованный список
//            if (filteredRow >= 0) {
//                beginRemoveRows(QModelIndex(), filteredRow, filteredRow);
//                m_filteredGoals.removeAt(filteredRow);
//                endRemoveRows();
//            } else {
//                // Если цель не в фильтре, просто перефильтруем
//                applyFilters();
//            }

//            emit dataChangedExternally();
//            return true;
//        }
//    }

//    return false;
//}

bool GoalsTableModel::removeGoal(const QString& goalId)
{
    if (!m_sourceGoals) {
        qDebug() << "No source goals in model";
        return false;
    }

    // Находим цель в основном источнике данных
    for (int i = 0; i < m_sourceGoals->size(); ++i) {
        Goal* goal = m_sourceGoals->at(i);
        if (goal->id == goalId) {
            // Находим индекс в отфильтрованном списке
            int filteredRow = -1;
            for (int j = 0; j < m_filteredGoals.size(); ++j) {
                if (m_filteredGoals[j] == goal) {
                    filteredRow = j;
                    break;
                }
            }

            // Если цель есть в отфильтрованном списке, удаляем её
            if (filteredRow >= 0) {
                beginRemoveRows(QModelIndex(), filteredRow, filteredRow);
                m_filteredGoals.removeAt(filteredRow);
                endRemoveRows();
            } else {
                // Если цель не в фильтре, просто обновляем модель
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

