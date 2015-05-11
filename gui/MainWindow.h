#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "graph/Graph.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showStatusMessage(const QString& status, int timeout = 0);
    Graph& getGraph();

    void setOpenFileName(const QString &fileName);
    const QString& getOpenFile() const;
    const QString& getOpenFileName() const;

private:
    Ui::MainWindow *ui;
    Graph graph;
    QString openFile;
    QString openFileName;

private slots:
    void showOpen();
    void showSaveToFile();
    void showGenerate();
    void showStatistic();
    void showAbout();
};

#endif // MAINWINDOW_H
