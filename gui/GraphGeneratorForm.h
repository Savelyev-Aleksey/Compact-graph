#ifndef GRAPHGENERATORFORM_H
#define GRAPHGENERATORFORM_H

#include <QWidget>

class MainWindow;

namespace Ui {
class GraphGeneratorForm;
}

class GraphGeneratorForm : public QWidget
{
    Q_OBJECT

public:
    explicit GraphGeneratorForm(MainWindow *parent = 0);
    ~GraphGeneratorForm();

private:
    Ui::GraphGeneratorForm *ui;
    MainWindow* mainWindow;

private slots:
    void generateHypercube();
    void generateTorus();
    void hypercubeDataValid();
    void torusDataValid();
};

#endif // GRAPHGENERATORFORM_H
