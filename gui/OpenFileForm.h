#ifndef OPENFILEFORM_H
#define OPENFILEFORM_H

#include <QWidget>

#include "MainWindow.h"

namespace Ui {
class OpenFileForm;
}

class OpenFileForm : public QWidget
{
    Q_OBJECT

public:
    explicit OpenFileForm(MainWindow *parent = 0);
    ~OpenFileForm();

private:
    Ui::OpenFileForm *ui;
    MainWindow* mainWindow;
    QString fileName;

    bool readFileInfo(const QString &fileName);

private slots:
    void getFileInfo();
    void readFile();

};

#endif // OPENFILEFORM_H
