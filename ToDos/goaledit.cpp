#include "goaledit.h"
#include "ui_goaledit.h"
#include <QVBoxLayout>
#include <QDateTimeEdit>
#include <QUuid>
#include <QMenu>
#include <QProgressBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>



GoalEdit::GoalEdit(QWidget *parent, bool newness, QString* mainPathToSource, Goal* existingGoal)
    : QDialog(parent),
      ui(new Ui::GoalEdit),
      currentType(Simple),
      m_existingGoal(existingGoal),
      m_newness(newness)
{
    ui->setupUi(this);

    QVBoxLayout* mainLayout = new QVBoxLayout;


    setStyleSheet("background-color: rgba(254, 245, 218, 1)");

    ui->lineEdit->setStyleSheet("QLineEdit{background-color: rgba(236, 243, 246, 1); border: 2px solid black; font-size: 35px;}");
    ui->lineEdit->setPlaceholderText("Введите название...");
    ui->plainTextEdit->setStyleSheet("QPlainTextEdit{background-color: rgba(236, 243, 246, 1); border: 2px solid black; font-size: 28px;}");
    ui->plainTextEdit->setPlaceholderText("Введите описание...");

    mainLayout->addWidget(ui->label);

    mainLayout->addWidget(ui->label_2);
    mainLayout->addWidget(ui->lineEdit);
    mainLayout->addWidget(ui->label_3);
    mainLayout->addWidget(ui->plainTextEdit);

    QWidget* radioW = new QWidget;
    QHBoxLayout* radioL = new QHBoxLayout;

    // Устанавливаем имена для радиокнопок для удобства поиска
    radioL->addWidget(ui->radioButton);
    radioL->addWidget(ui->radioButton_2);
    radioL->addWidget(ui->radioButton_3);

    radioW->setLayout(radioL);

    mainLayout->addWidget(radioW);

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

    // Кнопки дедлайна и тэгов

    ui->deadlineButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(236, 243, 246, 1);
            border: 2px solid black;
            border-radius: 5px;
            font-size: 16px;
            padding: 10px;
            min-height: 50px;
            text-align: left;
        }
        QPushButton:hover {
            background-color: rgba(216, 223, 236, 1);
        }
    )");
    ui->deadlineButton->setText("Выбрать дату и время");


    ui->tagsButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(236, 243, 246, 1);
            border: 2px solid black;
            border-radius: 5px;
            font-size: 16px;
            padding: 10px;
            min-height: 50px;
            text-align: left;
        }
        QPushButton:hover {
            background-color: rgba(216, 223, 236, 1);
        }
    )");
    ui->tagsButton->setText("Выбрать тэги");

    // Устанавливаем иконки
    ui->deadlineButton->setIcon(QIcon(QString(*mainPathToSource + "\\calendar.png")));
    ui->tagsButton->setIcon(QIcon(QString(*mainPathToSource + "\\hashtag.svg")));
    ui->deadlineButton->setIconSize(QSize(32, 32));
    ui->tagsButton->setIconSize(QSize(32, 32));

    // Создаем StackedWidget для типов целей
    typeStack = new QStackedWidget(this);
    typeStack->setObjectName("typeStack");

    // Создаем виджеты для каждого типа
    createSimpleWidget();
    createAccumWidget();
    createCompositeWidget();

    // Добавляем виджеты в стек
    typeStack->addWidget(simpleWidget);
    typeStack->addWidget(accumWidget);
    typeStack->addWidget(compositeWidget);

    // Вставляем StackedWidget в нужное место layout
    //insertTypeStack();

    mainLayout->addWidget(typeStack);
    mainLayout->addWidget(ui->label_4);
    mainLayout->addWidget(ui->deadlineButton);
    mainLayout->addWidget(ui->label_5);
    mainLayout->addWidget(ui->tagsButton);

    // Подключаем обработчики событий
    connect(ui->deadlineButton, &QPushButton::clicked, this, [=]() {
        QDateTime dt = QDateTime::currentDateTime();

        QDialog dlg(this);
        dlg.setWindowTitle("Выбор даты и времени");
        QVBoxLayout* l = new QVBoxLayout(&dlg);
        QDateTimeEdit* edit = new QDateTimeEdit(dt);
        edit->setCalendarPopup(true);
        edit->setDisplayFormat("dd.MM.yyyy HH:mm");
        l->addWidget(edit);

        QDialogButtonBox* box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        l->addWidget(box);

        connect(box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
        connect(box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

        if (dlg.exec() == QDialog::Accepted) {
            ui->deadlineButton->setText(edit->dateTime().toString("dd.MM.yyyy HH:mm"));
            ui->deadlineButton->setProperty("deadline", edit->dateTime());
        }
    });

    if(newness){
        setWindowTitle("Новая цель");
    } else {
        setWindowTitle("Тут должно будет быть название существующей цели потом.");
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
            ui->tagsButton->setText("Выбрать тэги");
        } else {
            ui->tagsButton->setText(tags.join(", "));
        }
        ui->tagsButton->setProperty("tags", tags);
    });

    mainLayout->addWidget(ui->buttonBox);
    // Инициализируем UI с текущим типом
    updateTypeUI();

    setLayout(mainLayout);

    if (m_existingGoal && !m_newness) {
            loadGoalData(m_existingGoal);
            setWindowTitle("Редактирование: " + m_existingGoal->name);
        } else {
            setWindowTitle("Новая цель");
        }
}


GoalEdit::~GoalEdit()
{
    delete ui;
}

void GoalEdit::createSimpleWidget()
{
    simpleWidget = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(simpleWidget);
    layout->setContentsMargins(0, 10, 0, 10);

    // Простая цель не имеет дополнительных полей
    QLabel* infoLabel = new QLabel("Простая цель - без дополнительных параметров");
    infoLabel->setStyleSheet(R"(
        QLabel {
            font-size: 16px;
            color: #666;
            padding: 10px;
            font-style: italic;
        }
    )");

    layout->addWidget(infoLabel);
    layout->addStretch();
}

void GoalEdit::createAccumWidget()
{
    accumWidget = new QWidget;
    QVBoxLayout* accumLayout = new QVBoxLayout(accumWidget);
    accumLayout->setContentsMargins(0, 10, 0, 10);
    accumLayout->setSpacing(15);

    // Первая строка: цель и единицы измерения
    QWidget* targetWidget = new QWidget;
    QHBoxLayout* targetLayout = new QHBoxLayout(targetWidget);
    targetLayout->setContentsMargins(0, 0, 0, 0);
    targetLayout->setSpacing(10);

    QLabel* targetLabel = new QLabel("Цель:");
    targetLabel->setStyleSheet(R"(
        QLabel {
            font-size: 18px;
            font-weight: bold;
            min-width: 80px;
        }
    )");

    tar_valueLine = new QLineEdit;
    tar_valueLine->setPlaceholderText("Число...");
    tar_valueLine->setStyleSheet(R"(
        QLineEdit {
            background-color: rgba(236, 243, 246, 1);
            border: 2px solid black;
            font-size: 18px;
            padding: 10px;
            border-radius: 5px;
            min-height: 40px;
        }
    )");

    unitLineEdit = new QLineEdit;
    unitLineEdit->setPlaceholderText("ед. измерения");
    unitLineEdit->setStyleSheet(R"(
        QLineEdit {
            background-color: rgba(236, 243, 246, 1);
            border: 2px solid black;
            font-size: 18px;
            padding: 10px;
            border-radius: 5px;
            min-height: 40px;
        }
    )");

    targetLayout->addWidget(targetLabel);
    targetLayout->addWidget(tar_valueLine, 2);
    targetLayout->addWidget(unitLineEdit, 1);

    // Вторая строка: изменение текущего значения
    QWidget* changeWidget = new QWidget;
    QHBoxLayout* changeLayout = new QHBoxLayout(changeWidget);
    changeLayout->setContentsMargins(0, 0, 0, 0);
    changeLayout->setSpacing(10);

    valueLine = new QLineEdit;
    valueLine->setPlaceholderText("Число...");
    valueLine->setStyleSheet(R"(
        QLineEdit {
            background-color: rgba(236, 243, 246, 1);
            border: 2px solid black;
            font-size: 18px;
            padding: 10px;
            border-radius: 5px;
            min-height: 40px;
        }
    )");

    QPushButton* addButton = new QPushButton("Добавить (+)");
    addButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(199, 229, 197, 1);
            border: 2px solid black;
            border-radius: 5px;
            font-size: 16px;
            font-weight: bold;
            padding: 10px 15px;
            min-height: 40px;
        }
        QPushButton:hover {
            background-color: rgba(179, 209, 177, 1);
        }
    )");

    QPushButton* subButton = new QPushButton("Убавить (-)");
    subButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(239, 85, 85, 0.3);
            border: 2px solid black;
            border-radius: 5px;
            font-size: 16px;
            font-weight: bold;
            padding: 10px 15px;
            min-height: 40px;
        }
        QPushButton:hover {
            background-color: rgba(219, 65, 65, 0.4);
        }
    )");

    changeLayout->addWidget(valueLine, 2);
    changeLayout->addWidget(addButton);
    changeLayout->addWidget(subButton);

    // Прогресс-бар
    accum_progress = new QProgressBar;
    accum_progress->setStyleSheet(R"(
        QProgressBar {
            border: 2px solid black;
            border-radius: 5px;
            text-align: center;
            font-size: 14px;
            height: 30px;
        }
        QProgressBar::chunk {
            background-color: rgba(165, 224, 155, 1);
            border-radius: 3px;
        }
    )");
    accum_progress->setRange(0, 100);
    accum_progress->setValue(0);

    // Добавляем все в layout
    accumLayout->addWidget(targetWidget);
    accumLayout->addWidget(changeWidget);
    accumLayout->addWidget(accum_progress);

    // Подключаем сигналы кнопок
    connect(addButton, &QPushButton::clicked, this, [this]() {
        updateProgress(true);
    });

    connect(subButton, &QPushButton::clicked, this, [this]() {
        updateProgress(false);
    });
}

void GoalEdit::createCompositeWidget()
{
    compositeWidget = new QWidget;
    QVBoxLayout* compositeLayout = new QVBoxLayout(compositeWidget);
    compositeLayout->setContentsMargins(0, 10, 0, 10);
    compositeLayout->setSpacing(10);

    QLabel* subgoalsLabel = new QLabel("Подцели:");
    subgoalsLabel->setStyleSheet(R"(
        QLabel {
            font-size: 18px;
            font-weight: bold;
        }
    )");

    QPushButton* addSubgoalButton = new QPushButton("Добавить подцель");
    addSubgoalButton->setStyleSheet(R"(
        QPushButton {
            background-color: rgba(165, 224, 155, 1);
            border: 2px solid black;
            border-radius: 5px;
            font-size: 16px;
            font-weight: bold;
            padding: 10px 15px;
            min-height: 40px;
        }
        QPushButton:hover {
            background-color: rgba(145, 204, 135, 1);
        }
    )");

    subgoalsList = new QListWidget;
    subgoalsList->setStyleSheet(R"(
        QListWidget {
            background-color: rgba(236, 243, 246, 1);
            border: 2px solid black;
            border-radius: 5px;
            font-size: 16px;
            min-height: 100px;
        }
        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #ddd;
        }
    )");

    compositeLayout->addWidget(subgoalsLabel);
    compositeLayout->addWidget(addSubgoalButton);
    compositeLayout->addWidget(subgoalsList);

    connect(addSubgoalButton, &QPushButton::clicked, this, [this]() {
        addSubgoal();
    });
}

void GoalEdit::insertTypeStack()
{
    // Ищем основной layout формы
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) {
        // Пробуем найти layout через ui
       //mainLayout = qobject_cast<QVBoxLayout*>(ui->verticalLayout); //no member named verticalLayout
    }

    if (!mainLayout) return;

    // Ищем позицию для вставки - после радиокнопок
    int insertPosition = -1;
    for (int i = 0; i < mainLayout->count(); i++) {
        QWidget* w = mainLayout->itemAt(i)->widget();
        if (w && (w == ui->radioButton_3 ||
                  w == ui->radioButton_2 ||
                  w == ui->radioButton)) {
            insertPosition = i + 1;
            break;
        }
    }

    // Если не нашли радиокнопки, ищем перед кнопками срока/тэгов
    if (insertPosition == -1) {
        for (int i = 0; i < mainLayout->count(); i++) {
            QWidget* w = mainLayout->itemAt(i)->widget();
            if (w == ui->deadlineButton) {
                insertPosition = i;
                break;
            }
        }
    }

    if (insertPosition > 0) {
        mainLayout->insertWidget(insertPosition, typeStack);
    } else {
        // Если ничего не нашли, добавляем перед кнопками OK/Cancel
        for (int i = 0; i < mainLayout->count(); i++) {
            QLayoutItem* item = mainLayout->itemAt(i);
            if (item->widget() && item->widget()->objectName().contains("buttonBox")) {
                insertPosition = i;
                break;
            }
        }

        if (insertPosition > 0) {
            mainLayout->insertWidget(insertPosition, typeStack);
        } else {
            mainLayout->addWidget(typeStack);
        }
    }
}

void GoalEdit::updateTypeUI()
{
    if (!typeStack) return;

    switch (currentType) {
    case Simple:
        typeStack->setCurrentWidget(simpleWidget);
        break;
    case Accumulation:
        typeStack->setCurrentWidget(accumWidget);
        break;
    case Composite:
        typeStack->setCurrentWidget(compositeWidget);
        break;
    }
}

//Goal* GoalEdit::createGoal() const
//{
//    Goal* g = new Goal;

//    g->id = QUuid::createUuid().toString(QUuid::WithoutBraces);
//    g->name = ui->lineEdit->text();
//    g->description = ui->plainTextEdit->toPlainText();

//    switch (currentType) {
//    case Simple:
//        g->type = "simple";
//        break;
//    case Accumulation:
//        g->type = "accumulation";
//        break;
//    case Composite:
//        g->type = "composite";
//        break;
//    }

//    g->deadline = ui->deadlineButton->property("deadline").toDateTime();
//    g->tagIds = ui->tagsButton->property("tags").toStringList();

//    g->current = 0;
//    g->target = 0;

//    return g;
//}

//Goal* GoalEdit::createGoal() const
//{
//    Goal* goal;

//    // Если редактируем существующую цель, используем её
//    if (m_existingGoal && !m_newness) {
//        goal = m_existingGoal;
//    } else {
//        // Иначе создаем новую
//        goal = new Goal;
//        goal->id = QUuid::createUuid().toString(QUuid::WithoutBraces);
//    }

//    // Заполняем/обновляем данные
//    goal->name = ui->lineEdit->text();
//    goal->description = ui->plainTextEdit->toPlainText();

//    // Тип цели
//    switch (currentType) {
//    case Simple:
//        goal->type = "simple";
//        goal->current = 0;
//        goal->target = 0;
//        break;
//    case Accumulation:
//        goal->type = "accumulation";
//        if (tar_valueLine && accum_progress) {
//            bool ok;
//            goal->target = tar_valueLine->text().toInt(&ok);
//            if (!ok) goal->target = 0;
//            goal->current = accum_progress->value();
//        }
//        break;
//    case Composite:
//        goal->type = "composite";
//        goal->current = 0;
//        goal->target = 0;
//        // Здесь нужно сохранить подцели
//        if (subgoalsList) {
//            goal->subgoalIds.clear();
//            for (int i = 0; i < subgoalsList->count(); ++i) {
//                // Предположим, что каждый элемент списка имеет ID
//                // В реальности здесь нужно создать подцели
//                goal->subgoalIds.append(QString::number(i));
//            }
//        }
//        break;
//    }

//    // Дедлайн
//    goal->deadline = ui->deadlineButton->property("deadline").toDateTime();

//    // Тэги
//    goal->tagIds = ui->tagsButton->property("tags").toStringList();

//    // По умолчанию в первую папку
//    goal->folderId = "folder_0";

//    return goal;
//}

Goal* GoalEdit::createGoal() const
{
    Goal* goal;

    if (!m_newness && m_existingGoal) {
        // Редактируем существующую цель
        goal = m_existingGoal;
    } else {
        // Создаем новую цель
        goal = new Goal;
        goal->id = QUuid::createUuid().toString(QUuid::WithoutBraces);
        goal->folderId = "folder_1"; // По умолчанию в первую папку
        goal->parentId = ""; // По умолчанию нет родителя
        goal->subgoalIds.clear(); // Нет подцелей
    }

    // Заполняем/обновляем основные данные
    goal->name = ui->lineEdit->text().trimmed();
    goal->description = ui->plainTextEdit->toPlainText().trimmed();

    // Определяем тип цели
    switch (currentType) {
    case Simple: {
        goal->type = "simple";
        goal->current = 0;
        goal->target = 0;
        break;
    }

    case Accumulation: {
        goal->type = "accumulation";

        // Получаем target
        bool targetOk, currentOk;
        int targetValue = tar_valueLine->text().toInt(&targetOk);
        if (targetOk && targetValue > 0) {
            goal->target = targetValue;
        } else {
            goal->target = 0;
        }

        // Получаем current из прогресс-бара
        goal->current = accum_progress ? accum_progress->value() : 0;

        // Если есть единицы измерения, добавляем их к описанию
        QString unit = unitLineEdit ? unitLineEdit->text().trimmed() : "";
        if (!unit.isEmpty() && !goal->description.contains(unit)) {
            if (!goal->description.isEmpty()) {
                goal->description += "\n";
            }
            goal->description += "Единицы измерения: " + unit;
        }
        break;
    }

    case Composite: {
        goal->type = "composite";
        goal->current = 0;
        goal->target = 0;

        // Сохраняем подцели
        goal->subgoalIds.clear();
        if (subgoalsList) {
            // Создаем ID для каждой подцели
            for (int i = 0; i < subgoalsList->count(); ++i) {
                QListWidgetItem* item = subgoalsList->item(i);
                if (item) {
                    // В реальном приложении здесь нужно было бы создавать
                    // полноценные объекты Goal для подцелей
                    QString subgoalId = QUuid::createUuid().toString(QUuid::WithoutBraces);
                    goal->subgoalIds.append(subgoalId);
                }
            }
        }
        break;
        }
    }

    // Сохраняем дедлайн
    QVariant deadlineVar = ui->deadlineButton->property("deadline");
    if (deadlineVar.isValid()) {
        goal->deadline = deadlineVar.toDateTime();
    } else {
        goal->deadline = QDateTime(); // Невалидная дата
    }

    // Сохраняем тэги
    QVariant tagsVar = ui->tagsButton->property("tags");
    if (tagsVar.isValid()) {
        goal->tagIds = tagsVar.toStringList();
    } else {
        goal->tagIds.clear();
    }

    // Логирование для отладки
    qDebug() << "Creating/updating goal:";
    qDebug() << "  ID:" << goal->id;
    qDebug() << "  Name:" << goal->name;
    qDebug() << "  Type:" << goal->type;
    qDebug() << "  Tags:" << goal->tagIds;
    qDebug() << "  Deadline valid:" << goal->deadline.isValid();
    if (goal->deadline.isValid()) {
        qDebug() << "  Deadline:" << goal->deadline.toString("dd.MM.yyyy HH:mm");
    }
    qDebug() << "  Current:" << goal->current;
    qDebug() << "  Target:" << goal->target;
    qDebug() << "  Subgoals:" << goal->subgoalIds.size();

    return goal;
}


// Метод для обновления прогресса
void GoalEdit::updateProgress(bool add)
{
    if (!valueLine || !tar_valueLine || !accum_progress) return;

    bool ok;
    int changeValue = valueLine->text().toInt(&ok);
    if (!ok || changeValue <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректное число для изменения");
        return;
    }

    int targetValue = tar_valueLine->text().toInt(&ok);
    if (!ok || targetValue <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректное число для цели");
        return;
    }

    int currentValue = accum_progress->value();
    int newValue;

    if (add) {
        newValue = qMin(currentValue + changeValue, targetValue);
    } else {
        newValue = qMax(currentValue - changeValue, 0);
    }

    accum_progress->setValue(newValue);
    accum_progress->setFormat(QString("%1/%2 %3")
        .arg(newValue)
        .arg(targetValue)
        .arg(unitLineEdit ? unitLineEdit->text() : ""));
}

// Метод для добавления подцели
void GoalEdit::addSubgoal()
{
    if (!subgoalsList) return;

    bool ok;
    QString subgoalName = QInputDialog::getText(this, "Добавить подцель",
        "Введите название подцели:", QLineEdit::Normal, "", &ok);

    if (ok && !subgoalName.isEmpty()) {
        QListWidgetItem* item = new QListWidgetItem(subgoalName);
        subgoalsList->addItem(item);
    }
}

void GoalEdit::loadGoalData(Goal* goal)
{
    if (!goal) return;

    // Заполняем основные поля
    ui->lineEdit->setText(goal->name);
    ui->plainTextEdit->setPlainText(goal->description);

    // Устанавливаем дедлайн
    if (goal->deadline.isValid()) {
        ui->deadlineButton->setText(goal->deadline.toString("dd.MM.yyyy HH:mm"));
        ui->deadlineButton->setProperty("deadline", goal->deadline);
    }

    // Устанавливаем тэги
    if (!goal->tagIds.isEmpty()) {
        ui->tagsButton->setText(goal->tagIds.join(", "));
        ui->tagsButton->setProperty("tags", goal->tagIds);
    }

    // Устанавливаем тип цели
    if (goal->type == "simple") {
        ui->radioButton->setChecked(true);
        currentType = Simple;
    } else if (goal->type == "accumulation") {
        ui->radioButton_2->setChecked(true);
        currentType = Accumulation;
    } else if (goal->type == "composite") {
        ui->radioButton_3->setChecked(true);
        currentType = Composite;
    }

    // Загружаем специфичные данные для типов
    if (currentType == Accumulation && goal->type == "accumulation") {
        if (tar_valueLine) {
            tar_valueLine->setText(QString::number(goal->target));
        }
        if (accum_progress) {
            accum_progress->setValue(goal->current);
            accum_progress->setMaximum(goal->target);
            if (unitLineEdit && !goal->description.isEmpty()) {
                // Можно использовать единицы измерения из описания или отдельного поля
                unitLineEdit->setText("ед.");
            }
        }
    }

    // Для составных целей загружаем подцели
    if (currentType == Composite && goal->type == "composite") {
        // Здесь нужно загрузить подцели
        // Это зависит от того, как вы их храните
    }

    updateTypeUI();
}

