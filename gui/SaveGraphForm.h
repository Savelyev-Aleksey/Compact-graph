#ifndef SAVEGRAPHFORM_H
#define SAVEGRAPHFORM_H

#include <QWidget>

#include "graph/FileTypes.h"

namespace Ui {
    class SaveGraphForm;
}

class MainWindow;
class SaveFrom;



class SaveGraphForm : public QWidget
{
    Q_OBJECT

public:
    explicit SaveGraphForm(MainWindow *mainWindow, QWidget *parent);
    ~SaveGraphForm();

private:
    Ui::SaveGraphForm *ui;
    MainWindow *mainWindow;

    ulong nodeId;

    void prepareGraphWidgets();

private slots:
    void graphTypeChanged();

    void validateInputs();

    bool checkStartNodeEdit(QString &info);

    bool validateNodeNodeValue(QString &info);
    bool validateBracketsFlat(QString &info);
    bool validateBracketsNested(QString &info, bool isGraphEmpty);
    bool validateProjections(QString &info, bool isGraphEmpty);


    void saveGraph();
};

#endif // SAVEGRAPHFORM_H
