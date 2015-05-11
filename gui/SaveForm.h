#ifndef SAVEFORM_H
#define SAVEFORM_H

#include <QWidget>

namespace Ui {
class SaveForm;
}

class MainWindow;

class SaveForm : public QWidget
{
    Q_OBJECT

public:
    explicit SaveForm(MainWindow *parent = 0);
    ~SaveForm();

private:
    Ui::SaveForm *ui;
    MainWindow *mainWindow;

private slots:
    void menuShowGraph();
    void menuShowShortPath();
};

#endif // SAVEFORM_H
