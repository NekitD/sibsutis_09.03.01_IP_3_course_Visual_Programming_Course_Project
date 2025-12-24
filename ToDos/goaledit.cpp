#include "goaledit.h"
#include "ui_goaledit.h"
#include <QVBoxLayout>
#include <QDateTimeEdit>
#include <QUuid>
#include <QMenu>


GoalEdit::GoalEdit(QWidget *parent, bool newness, QString* mainPathToSource)
    : QDialog(parent),
      ui(new Ui::GoalEdit),
      currentType(Simple)
{
    ui->setupUi(this);


    setStyleSheet("background-color: rgba(254, 245, 218, 1)");



    ui->lineEdit->setStyleSheet("QLineEdit{background-color: rgba(236, 243, 246, 1); border: 2px solid black; font-size: 35px;}");
    ui->lineEdit->setPlaceholderText("Введите название...");
    ui->plainTextEdit->setStyleSheet("QPlainTextEdit{background-color: rgba(236, 243, 246, 1); border: 2px solid black; font-size: 28px;}");
    ui->plainTextEdit->setPlaceholderText("Введите описание...");
    ui->deadlineButton->setStyleSheet("QPushButton{background-color: rgba(236, 243, 246, 1);}");
    ui->deadlineButton->setText("...");
    ui->deadlineButton->setIcon(QIcon(QString(*mainPathToSource + "\\calendar.png")));
    ui->tagsButton->setStyleSheet("QPushButton{background-color: rgba(236, 243, 246, 1);}");
    ui->tagsButton->setText("...");
    ui->tagsButton->setIcon(QIcon(QString(*mainPathToSource + "\\hashtag.svg")));

    ui->deadlineButton->setIconSize(QSize(32, 32));
    ui->tagsButton->setIconSize(QSize(32, 32));


    ui->radioButton->setChecked(true);
    ui->radioButton->setStyleSheet(R"(
            QRadioButton {
                font-size: 18px;
                padding: 8px;
                spacing: 5px;
                min-height: 30px;
            }
            QRadioButton::indicator {
                width: 24px;
                height: 24px;
            }
            QRadioButton::indicator:unchecked {
                border: 2px solid #999999;
                border-radius: 12px;
                background-color: white;
            }
            QRadioButton::indicator:checked {
                border: 2px solid #2196F3;
                border-radius: 12px;
                background-color: #2196F3;
            }
        )");

        ui->radioButton_2->setStyleSheet(R"(
            QRadioButton {
                font-size: 18px;
                padding: 8px;
                spacing: 5px;
                min-height: 30px;
            }
            QRadioButton::indicator {
                width: 24px;
                height: 24px;
            }
            QRadioButton::indicator:unchecked {
                border: 2px solid #999999;
                border-radius: 12px;
                background-color: white;
            }
            QRadioButton::indicator:checked {
                border: 2px solid #2196F3;
                border-radius: 12px;
                background-color: #2196F3;
            }
        )");

        ui->radioButton_3->setStyleSheet(R"(
            QRadioButton {
                font-size: 18px;
                padding: 8px;
                spacing: 5px;
                min-height: 30px;
            }
            QRadioButton::indicator {
                width: 24px;
                height: 24px;
            }
            QRadioButton::indicator:unchecked {
                border: 2px solid #999999;
                border-radius: 12px;
                background-color: white;
            }
            QRadioButton::indicator:checked {
                border: 2px solid #2196F3;
                border-radius: 12px;
                background-color: #2196F3;
            }
        )");

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

//    connect(ui->tagsButton, &QPushButton::clicked, this, [=]() {
//        QMenu menu;

////        for (const Tag& tag : allTags) { // use undeclared identifier 'allTags'
////            QAction* act = menu.addAction(tag.name);
////            act->setCheckable(true);
////        }

//        menu.exec(QCursor::pos());

//        QStringList tags;
//        for (QAction* a : menu.actions())
//            if (a->isChecked())
//                tags << a->text();

//        ui->tagsButton->setText(tags.join(", "));
//        ui->tagsButton->setProperty("tags", tags);
//    });

    if(newness){
        setWindowTitle("Новая цель");
    }

    connect(ui->tagsButton, &QPushButton::clicked, this, [=]() {
            QMenu menu;
            menu.setStyleSheet(R"(
                QMenu {
                    background-color: white;
                    border: 2px solid #2196F3;
                    border-radius: 5px;
                    padding: 5px;
                }
                QMenu::item {
                    padding: 8px 20px;
                    font-size: 16px;
                }
                QMenu::item:selected {
                    background-color: #E3F2FD;
                    color: #1976D2;
                }
                QMenu::indicator {
                    width: 20px;
                    height: 20px;
                }
            )");

            QStringList exampleTags = {"Важно", "Срочно", "В работе"};

            QString currentTags = ui->tagsButton->property("tags").toStringList().join(",");

            for (const QString& tagName : exampleTags) {
                QAction* act = menu.addAction(tagName);
                act->setCheckable(true);
                act->setChecked(currentTags.contains(tagName));
            }

            menu.exec(ui->tagsButton->mapToGlobal(QPoint(0, ui->tagsButton->height())));

            QStringList tags;
            for (QAction* a : menu.actions()) {
                if (a->isChecked()) {
                    tags << a->text();
                }
            }

            if (tags.isEmpty()) {
                ui->tagsButton->setText("   ...");
            } else {
                ui->tagsButton->setText("   " + tags.join(", "));
            }
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

