#ifndef GOALEDIT_H
#define GOALEDIT_H

#include <QDialog>
#include "mainwindow.h"
#include <QProgressBar>
#include <QListWidget>

namespace Ui {
class GoalEdit;
}

class GoalEdit : public QDialog
{
    Q_OBJECT
public:
    explicit GoalEdit(QWidget *parent = nullptr, bool newness = true, QString* mainPathToSource = nullptr,  Goal* existingGoal = nullptr);
//    explicit GoalEdit(TagsList* tagsModel, QWidget* parent = nullptr);
    ~GoalEdit();

    Goal* createGoal() const;
    void addGoal(Goal* g);
    Goal* createdGoal(); 


private:
    Ui::GoalEdit *ui;

    Goal* m_existingGoal;
    bool m_newness;

    QWidget* accumWidget = nullptr;
    QLineEdit* tar_valueLine = nullptr;
    QLineEdit* valueLine = nullptr;
    QLineEdit* unitLineEdit = nullptr;
    QProgressBar* accum_progress = nullptr;


//    void insertAccumWidget();
//    void insertCompositeWidget();

    enum GoalType { Simple, Accumulation, Composite };
    GoalType currentType;

    void updateTypeUI();
    TagsList* m_tagsModel;

    QListWidget* subgoalsList;

    void addSubgoal();
    void updateProgress(bool add);

    QStackedWidget* typeStack = nullptr;

        // Виджеты для каждого типа
    QWidget* simpleWidget = nullptr;
    QWidget* compositeWidget = nullptr;
    void createSimpleWidget();
    void createAccumWidget();
    void createCompositeWidget();
    void insertTypeStack();

    void loadGoalData(Goal* goal);
};

#endif // GOALEDIT_H
