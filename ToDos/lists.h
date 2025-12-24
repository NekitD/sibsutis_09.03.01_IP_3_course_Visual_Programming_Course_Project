#ifndef LISTS_H
#define LISTS_H

#include <QVariant>
#include <QAbstractListModel>
#include "objects.h"

class ChoosableObjectsList;
class FoldersList;
class TabsList;
class TagsList;
class GoalsList;

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


#endif // LISTS_H
