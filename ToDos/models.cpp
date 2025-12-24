#include "models.h"



bool GoalsFilterModel::filterAcceptsRow(int row, const QModelIndex& parent) const
{
    QModelIndex idx = sourceModel()->index(row, 0, parent);

    QDateTime deadline =
        sourceModel()->data(idx, DeadlineRole).toDateTime();

    QString folderId =
        sourceModel()->data(idx, FolderRole).toString();

    QStringList tags =
        sourceModel()->data(idx, TagRole).toStringList();

    if (m_todayOnly) {
        if (!deadline.isValid() ||
            deadline.date() != QDate::currentDate())
            return false;
    }

    if (!m_folderId.isEmpty() && folderId != m_folderId)
        return false;

    if (!m_tagId.isEmpty() && !tags.contains(m_tagId))
        return false;

    if (m_date.isValid()) {
        if (!deadline.isValid() ||
            deadline.date() != m_date)
            return false;
    }

    return true;
}

//bool GoalsFilterModel::filterAcceptsRow(int row,
//                                       const QModelIndex& parent) const
//{
//    QModelIndex idx = sourceModel()->index(row, 0, parent);

//    QDateTime deadline =
//        sourceModel()->data(idx, DeadlineRole).toDateTime();

//    QString folderId =
//        sourceModel()->data(idx, FolderRole).toString();

//    QStringList tags =
//        sourceModel()->data(idx, TagRole).toStringList();

//    QString name = sourceModel()->data(idx, Qt::DisplayRole).toString();

//    if (m_todayOnly) {
//        if (!deadline.isValid()) {
//            qDebug() << "Goal" << name << ": deadline not valid, filtered out";
//            return false;
//        }

//        QDate deadlineDate = deadline.date();
//        QDate currentDate = QDate::currentDate();

//        qDebug() << "Checking goal:" << name;
//        qDebug() << "Deadline date:" << deadlineDate.toString("yyyy-MM-dd");
//        qDebug() << "Current date:" << currentDate.toString("yyyy-MM-dd");
//        qDebug() << "Match:" << (deadlineDate == currentDate);

//        if (deadlineDate != currentDate) {
//            qDebug() << "Goal" << name << ": date mismatch, filtered out";
//            return false;
//        }

//        qDebug() << "Goal" << name << ": accepted for today";
//    }

//    if (!m_folderId.isEmpty() && folderId != m_folderId) {
//        qDebug() << "Goal" << name << ": folder mismatch, filtered out";
//        return false;
//    }

//    if (!m_tagId.isEmpty() && !tags.contains(m_tagId)) {
//        qDebug() << "Goal" << name << ": tag mismatch, filtered out";
//        return false;
//    }

//    if (m_date.isValid()) {
//        if (!deadline.isValid()) {
//            qDebug() << "Goal" << name << ": deadline not valid for date filter, filtered out";
//            return false;
//        }

//        QDate deadlineDate = deadline.date();
//        if (deadlineDate != m_date) {
//            qDebug() << "Goal" << name << ": specific date mismatch, filtered out";
//            return false;
//        }
//    }

//    return true;
//}

void GoalsFilterModel::setTodayOnly(bool on)
{
    m_todayOnly = on;
    invalidateFilter();
}

void GoalsFilterModel::setTagFilter(const QString& tagId)
{
    m_tagId = tagId;
    invalidateFilter();
}

void GoalsFilterModel::setFolderFilter(const QString& folderId)
{
    m_folderId = folderId;
    invalidateFilter();
}

void GoalsFilterModel::setDateFilter(const QDate& date)
{
    m_date = date;
    invalidateFilter();
}


GoalsFilterModel::GoalsFilterModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

//------------------------------------------------------------------------------------------------


GoalsTableModel::GoalsTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int GoalsTableModel::rowCount(const QModelIndex&) const {
    return m_goals.size();
}

int GoalsTableModel::columnCount(const QModelIndex&) const {
    return ColumnCount;
}

QVariant GoalsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    const Goal* g = m_goals.at(index.row());

    if (role == DeadlineRole)
            return g->deadline;

    if (role == TagRole)
        return g->tagIds;

    if (role == FolderRole)
        return g->folderId;

    if (role == Qt::DisplayRole) {

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

void GoalsTableModel::setGoals(const QVector<Goal*>& goals)
{
    beginResetModel();
    m_goals = goals;
    endResetModel();
}
