#include "delegates.h"
#include <QStyledItemDelegate>
#include <QPainter>


void TabsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    opt.font.setPointSize(14);
    opt.font.setBold(option.state & QStyle::State_Selected);
    opt.displayAlignment = Qt::AlignCenter;
    QStyledItemDelegate::paint(painter, opt, index);
}


void TagsDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    opt.font.setPointSize(12);
    opt.font.setBold(option.state & QStyle::State_Selected);
    opt.displayAlignment = Qt::AlignCenter;
    QStyledItemDelegate::paint(painter, opt, index);
}

void FoldersDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QFont font = opt.font;
    font.setPointSize(25);
    font.setBold(opt.state & QStyle::State_Selected);
    opt.font = font;
    opt.displayAlignment = Qt::AlignCenter;
    painter->setPen(QPen(QColor(0, 0, 0), 1));
    if (opt.state & QStyle::State_Selected) {
        painter->fillRect(opt.rect, QColor(232, 208, 121));
        painter->setPen(QPen(QColor(0, 0, 0), 2));
        painter->drawRect(opt.rect.adjusted(1, 1, -1, -1));
    }

    painter->setPen(Qt::black);
    painter->setFont(font);
    painter->drawText(opt.rect, Qt::AlignCenter, opt.text);

    painter->restore();
}

QSize FoldersDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(width, height);
}

void FoldersDelegate::setItemSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}

GoalsDelegate::GoalsDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QSize GoalsDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const {
    return QSize(100, 80);
}

void GoalsDelegate::paint(QPainter* p, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    p->save();

    QRect r = option.rect;

    QColor bg = (option.state & QStyle::State_Selected)
        ? QColor(255, 240, 163)
        : QColor(255, 250, 230);

    p->fillRect(r, bg);
    p->setPen(QPen(Qt::black, 1));
    p->drawRect(r.adjusted(0, 0, -1, -1));

    QString text = index.data(Qt::DisplayRole).toString();

    QFont font = option.font;

    if (index.column() == 0) {
        p->drawRect(option.rect.adjusted(0, 0, -1, -1));
    }

    switch (index.column()) {
    case NameColumn:
        font.setPointSize(14);
        font.setBold(true);
        p->setFont(font);
        p->drawText(r.adjusted(10, 6, -10, -6),
                    Qt::AlignLeft | Qt::AlignVCenter,
                    text);
        break;

    case DescriptionColumn:
        font.setPointSize(10);
        font.setBold(false);
        p->setFont(font);
        p->drawText(r.adjusted(10, 6, -10, -6),
                    Qt::TextWordWrap | Qt::AlignVCenter,
                    text);
        break;

    case DeadlineColumn:
        font.setPointSize(12);
        p->setFont(font);
        p->drawText(r.adjusted(10, 6, -10, -6),
                    Qt::AlignCenter,
                    text);
        break;

    case StatusColumn:
        font.setPointSize(12);
        font.setBold(true);
        p->setFont(font);
        p->drawText(r.adjusted(10, 6, -10, -6),
                    Qt::AlignCenter,
                    text);
        break;
    }

    p->restore();
}
