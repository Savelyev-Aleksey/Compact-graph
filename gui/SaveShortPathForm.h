#ifndef SAVESHORTPATHFORM_H
#define SAVESHORTPATHFORM_H

#include <QWidget>

namespace Ui {
    class SaveShortPathForm;
}

class MainWindow;

class SaveShortPathForm : public QWidget
{
    Q_OBJECT

public:
    explicit SaveShortPathForm(MainWindow *mainWindow, QWidget *parent = 0);
    ~SaveShortPathForm();

private:
    Ui::SaveShortPathForm *ui;
    MainWindow *mainWindow;

private slots:
    void validateShortPath();
    void saveShortPath();


};

#endif // SAVESHORTPATHFORM_H
