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

    unsigned short eccentrType;

    void printTableStat(const UintMap* map, QTableWidget* table, bool shrink);

    void printGraphParameters();
    void printGraphStatistic();
    void printEccentriciyStatistic();

    void createShortPaths();
    void createProjections();

    void updateButtonsStatus();

    void readProjectionsInfo();
    void projectionsInfoStatus();
};

#endif // GRAPHINFOFORM_H
