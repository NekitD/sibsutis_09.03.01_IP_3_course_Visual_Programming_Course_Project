#include "lists.h"
#include "models.h"

ChoosableObjectsList::ChoosableObjectsList(QObject* parent)
    : QAbstractListModel(parent)
{
}

int ChoosableObjectsList::rowCount(const QModelIndex&) const{
    return m_items.size();
}

Qt::ItemFlags ChoosableObjectsList::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ChoosableObjectsList::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return {};

    const auto* item = m_items[index.row()];

    switch(role){
        case Qt::DisplayRole:
            return item->name;
        case NameRole:
            return item->name;
        case SelectedRole:
            return item->selected;
        case IdRole:
            return item->id;
        }
    return {};
}

void ChoosableObjectsList::select(int row)
{
    for(int i = 0; i < m_items.size(); ++i){
        m_items[i]->selected = (i == row);
    }
    emit dataChanged(index(0), index(m_items.size() - 1));
}


void ChoosableObjectsList::add(ChoosableObject* item)
{
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.push_back(item);
    endInsertRows();
}


void ChoosableObjectsList::removeAt(int row)
{
    if (row < 0 || row >= m_items.size())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    delete m_items[row];
    m_items.removeAt(row);
    endRemoveRows();
}


QVariant TagsList::data(const QModelIndex& index, int role) const{
    if(!index.isValid())
        return {};

    auto* tag = dynamic_cast<Tag*>(m_items[index.row()]);
    if(!tag){
        return {};
    }

    switch(role){
        case ColorRole:
            return tag->color;
        case InKanbanRole:
            return tag->in_kanban;
        }
    return ChoosableObjectsList::data(index, role);
}

QVariant GoalsList::data(const QModelIndex& index, int role) const{
    if(!index.isValid())
        return {};

    auto* goal = dynamic_cast<Goal*>(m_items[index.row()]);
    if(!goal){
        return {};
    }

    switch(role){
        case DescriptionRole:
            return goal->description;
        case TypeRole:
            return goal->type;
        case CurrentRole:
            return goal->current;
        case TargetRole:
            return goal->target;
        case SubgoalsRole:
            return goal->subgoalIds;
        case DeadlineRole:
            return goal->deadline;
        case TagRole:
            return goal->tagIds;
        case FolderRole:
            return goal->folderId;
        case ParentRole:
            return goal->parentId;
        }
    return ChoosableObjectsList::data(index, role);
}




Goal* GoalsList::nearestGoal(const QDateTime& now) const
{
    Goal* nearest = nullptr;
    qint64 minDiff = LLONG_MAX;

    for (auto* item : m_items) {
        auto* goal = dynamic_cast<Goal*>(item);
        if (!goal)
            continue;

        if (!goal->deadline.isValid())
            continue;

        qint64 diff = now.secsTo(goal->deadline);
        if (diff >= 0 && diff < minDiff) {
            minDiff = diff;
            nearest = goal;
        }
    }
    return nearest;
}
