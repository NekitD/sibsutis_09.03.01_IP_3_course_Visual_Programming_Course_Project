#ifndef OBJECTS_H
#define OBJECTS_H

#include <QString>
#include <QDateTime>

class ChoosableObject;
class Tab;
class Tag;
class Folder;
class Goal;
class Notification;


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

    QStringList subgoalIds;

    QString parentId;
    QStringList tagIds;
    QString folderId;
};

class Notification {
public:
    QString id;
    QString text;
    QDateTime time;
    bool read = false;
};

#endif // OBJECTS_H
