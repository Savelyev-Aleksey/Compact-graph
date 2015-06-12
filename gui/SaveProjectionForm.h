#ifndef SAVEPROJECTIONFORM_H
#define SAVEPROJECTIONFORM_H

#include <QWidget>

namespace Ui {
class SaveProjectionForm;
}

class MainWindow;



class SaveProjectionForm : public QWidget
{
    Q_OBJECT

public:
    explicit SaveProjectionForm(MainWindow *mainWindow, QWidget *parent);
    ~SaveProjectionForm();

private:
    Ui::SaveProjectionForm *ui;
    MainWindow *mainWindow;

    void prepareWidgets();

private slots:
    void typeChanged();

    void validateInputs();

    bool checkStartNodeEdit(QString &info);
    bool checkSecondNodeEdit(QString &info);

    bool validateProjections(QString &info, bool isGraphEmpty);
    bool validateShortPath(QString &info, bool isGraphEmpty);

    void saveButtonClicked();

    void saveProjection();
    void saveShortPath();
};

#endif // SAVEPROJECTIONFORM_H
