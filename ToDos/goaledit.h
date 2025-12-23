#ifndef GOALEDIT_H
#define GOALEDIT_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class GoalEdit;
}

class GoalEdit : public QDialog
{
    Q_OBJECT
public:
    explicit GoalEdit(QWidget *parent = nullptr);
//    explicit GoalEdit(TagsList* tagsModel, QWidget* parent = nullptr);
    ~GoalEdit();

    Goal* createGoal() const;
    void addGoal(Goal* g);
    Goal* createdGoal();

private:
    Ui::GoalEdit *ui;

    enum GoalType { Simple, Accumulation, Composite };
    GoalType currentType;

    void updateTypeUI();
    TagsList* m_tagsModel;
};

#endif // GOALEDIT_H
