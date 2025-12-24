#ifndef DELEGATES_H
#define DELEGATES_H

#include <QStyledItemDelegate>
#include <QPainter>

enum Columns {
    NameColumn,
    DescriptionColumn,
    DeadlineColumn,
    StatusColumn,
    ColumnCount
};

class TabsDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

};

class TagsDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

class FoldersDelegate : public QStyledItemDelegate {
public:
    FoldersDelegate(int itemWidth = 200, int itemHeight = 50, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), width(itemWidth), height(itemHeight) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void setItemSize(int newWidth, int newHeight);

private:
    int width;
    int height;
};

class GoalsDelegate : public QStyledItemDelegate {
public:
    explicit GoalsDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
#endif // DELEGATES_H
