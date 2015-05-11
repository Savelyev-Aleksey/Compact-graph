#ifndef GRAPHINFOFORM_H
#define GRAPHINFOFORM_H

#include <QWidget>

#include "graph/types.h"

namespace Ui {
class GraphInfoForm;
}

class MainWindow;
class QTableWidget;

class GraphInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit GraphInfoForm(MainWindow *parent);
    ~GraphInfoForm();


private:
    Ui::GraphInfoForm *ui;
    MainWindow* mainWindow;

    void printTableStat(const UlongMap* map, QTableWidget* table, bool shrink);

    void printGraphInfo();
    void printShortPathInfo();

    void printGraphStatistic();
    void printShortPathStatistic();

    void findParameters();
};

#endif // GRAPHINFOFORM_H
