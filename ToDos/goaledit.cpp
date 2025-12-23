#include "goaledit.h"
#include "ui_goaledit.h"
#include <QVBoxLayout>
#include <QDateTimeEdit>
#include <QUuid>
#include <QMenu>


GoalEdit::GoalEdit(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::GoalEdit),
      currentType(Simple)
{
    ui->setupUi(this);

    ui->radioButton->setChecked(true);

    connect(ui->radioButton, &QRadioButton::toggled, this, [=](bool on){
        if (on) { currentType = Simple; updateTypeUI(); }
    });
    connect(ui->radioButton_2, &QRadioButton::toggled, this, [=](bool on){
        if (on) { currentType = Accumulation; updateTypeUI(); }
    });
    connect(ui->radioButton_3, &QRadioButton::toggled, this, [=](bool on){
        if (on) { currentType = Composite; updateTypeUI(); }
    });

    updateTypeUI();
    connect(ui->deadlineButton, &QPushButton::clicked, this, [=]() {
        QDateTime dt = QDateTime::currentDateTime();

        QDialog dlg(this);
        QVBoxLayout l(&dlg);
        QDateTimeEdit edit(dt);
        edit.setCalendarPopup(true);
        l.addWidget(&edit);

        QDialogButtonBox box(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        l.addWidget(&box);

        connect(&box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(&box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

        if (dlg.exec() == QDialog::Accepted) {
            ui->deadlineButton->setText(edit.dateTime().toString("dd.MM.yyyy HH:mm"));
            ui->deadlineButton->setProperty("deadline", edit.dateTime());
        }
    });

    connect(ui->tagsButton, &QPushButton::clicked, this, [=]() {
        QMenu menu;

//        for (const Tag& tag : allTags) { // use undeclared identifier 'allTags'
//            QAction* act = menu.addAction(tag.name);
//            act->setCheckable(true);
//        }

        menu.exec(QCursor::pos());

        QStringList tags;
        for (QAction* a : menu.actions())
            if (a->isChecked())
                tags << a->text();

        ui->tagsButton->setText(tags.join(", "));
        ui->tagsButton->setProperty("tags", tags);
    });
}

GoalEdit::~GoalEdit()
{
    delete ui;
}

void GoalEdit::updateTypeUI()
{
    switch (currentType) {
    case Simple:
        // скрыть поля накопления и подцелей
        break;
    case Accumulation:
        // показать current / target
        break;
    case Composite:
        // показать список подцелей
        break;
    }
}

Goal* GoalEdit::createGoal() const
{
    Goal* g = new Goal;

    g->id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    g->name = ui->lineEdit->text();
    g->description = ui->plainTextEdit->toPlainText();

    switch (currentType) {
    case Simple:
        g->type = "simple";
        break;
    case Accumulation:
        g->type = "accumulation";
        break;
    case Composite:
        g->type = "composite";
        break;
    }

    g->deadline = ui->deadlineButton->property("deadline").toDateTime();
    g->tagIds = ui->tagsButton->property("tags").toStringList();

    g->current = 0;
    g->target = 0;

    return g;
}

//Goal* GoalEdit::createdGoal() const
//{
//    auto* g = new Goal;
//    g->id = QUuid::createUuid().toString(QUuid::WithoutBraces);
//    g->name = ui->lineEdit->text();
//    g->description = ui->plainTextEdit->toPlainText();
//    g->deadline = selectedDeadline;
//    g->tagIds = selectedTags;
//    g->type = selectedType;
//    g->folderId = currentFolderId;

//    return g;
//}

