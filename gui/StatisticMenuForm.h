#ifndef STATISTICMENUFORM_H
#define STATISTICMENUFORM_H

#include <QWidget>

namespace Ui {
class StatisticMenuForm;
}


class MainWindow;

class StatisticMenuForm : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticMenuForm(MainWindow *parent);
    ~StatisticMenuForm();

private:
    Ui::StatisticMenuForm *ui;

    void menuShowGraph();
    void menuShowNode();
};

#endif // STATISTICMENUFORM_H
