#include "delegates.h"
#include <QStyledItemDelegate>
#include <QPainter>
#include "models.h"


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


CalendarDelegate::CalendarDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}



void CalendarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (!index.isValid())
        return;

    painter->save();

    QString name = index.data(Qt::DisplayRole).toString();
    QDateTime deadline = index.data(GoalsTableModel::DeadlineRole).toDateTime();

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, QColor(255, 235, 180, 255));
    } else {
        painter->fillRect(option.rect, QColor(255, 250, 230, 255));
    }

    painter->setPen(QPen(Qt::black, 1));
    painter->drawRect(option.rect.adjusted(0, 0, -1, -1));

    QRect nameRect = option.rect.adjusted(10, 5, -100, -5);
    painter->setPen(Qt::black);
    painter->setFont(QFont("Segoe UI", 12, QFont::Normal));

    QString elidedName = painter->fontMetrics().elidedText(
        name, Qt::ElideRight, nameRect.width());
    painter->drawText(nameRect, Qt::AlignLeft | Qt::AlignVCenter, elidedName);

    if (deadline.isValid()) {
        QString timeStr = deadline.time().toString("HH:mm");
        QRect timeRect = option.rect.adjusted(option.rect.width() - 90, 5, -10, -5);

        painter->fillRect(timeRect.adjusted(-5, 0, 5, 0),
                         QColor(230, 240, 255, 200));

        painter->setPen(QPen(QColor(100, 150, 200), 1));
        painter->drawRect(timeRect.adjusted(-5, 0, 5, 0));

        painter->setPen(QColor(50, 100, 150));
        painter->setFont(QFont("Segoe UI", 11, QFont::Bold));
        painter->drawText(timeRect, Qt::AlignCenter, timeStr);
    }

    painter->restore();
}

QSize CalendarDelegate::sizeHint(const QStyleOptionViewItem& option,
                                const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(100, 60);
}

KanbanDelegate::KanbanDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

void KanbanDelegate::paint(QPainter* painter,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const
{
    if (!index.isValid())
        return;

    painter->save();

    QString name = index.data(Qt::DisplayRole).toString();

    QRect bgRect = option.rect.adjusted(5, 5, -5, -5);

    if (option.state & QStyle::State_Selected) {
        QLinearGradient gradient(bgRect.topLeft(), bgRect.bottomLeft());
        gradient.setColorAt(0, QColor(255, 245, 200));
        gradient.setColorAt(1, QColor(255, 235, 180));
        painter->setBrush(gradient);
        painter->setPen(QPen(QColor(200, 150, 50), 3));
    } else {
        QLinearGradient gradient(bgRect.topLeft(), bgRect.bottomLeft());
        gradient.setColorAt(0, QColor(255, 255, 240));
        gradient.setColorAt(1, QColor(245, 245, 225));
        painter->setBrush(gradient);
        painter->setPen(QPen(Qt::black, 2));
    }

    painter->drawRoundedRect(bgRect, 10, 10);

    painter->setPen(Qt::black);
    QFont font("Segoe UI", 14, QFont::Bold);
    painter->setFont(font);

    QFontMetrics fm(font);
    int maxTextWidth = bgRect.width() - 20;

    QString elidedText = fm.elidedText(name, Qt::ElideRight, maxTextWidth);

    QStringList lines;
    if (fm.horizontalAdvance(name) > maxTextWidth) {
        QString currentLine;
        for (const QChar& ch : name) {
            if (fm.horizontalAdvance(currentLine + ch) > maxTextWidth) {
                lines.append(currentLine);
                currentLine = ch;
            } else {
                currentLine += ch;
            }
        }
        if (!currentLine.isEmpty()) {
            lines.append(currentLine);
        }
    } else {
        lines.append(elidedText);
    }

    int lineHeight = fm.height();
    int totalHeight = lines.size() * lineHeight;
    int startY = bgRect.top() + (bgRect.height() - totalHeight) / 2;

    for (int i = 0; i < lines.size(); ++i) {
        QRect textRect(bgRect.left(), startY + i * lineHeight,
                      bgRect.width(), lineHeight);
        painter->drawText(textRect, Qt::AlignCenter, lines[i]);
    }

    if (!(option.state & QStyle::State_Selected)) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(0, 0, 0, 30));
        painter->drawRoundedRect(bgRect.translated(0, 2), 10, 10);
    }

    painter->restore();
}

QSize KanbanDelegate::sizeHint(const QStyleOptionViewItem& option,
                              const QModelIndex& index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(200, 100);
}
