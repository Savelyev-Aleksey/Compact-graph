#ifndef NODEINFOFORM_H
#define NODEINFOFORM_H

#include <QWidget>

namespace Ui {
class NodeInfoForm;
}

class MainWindow;


class NodeInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit NodeInfoForm(MainWindow *parent);
    ~NodeInfoForm();

private:
    Ui::NodeInfoForm *ui;
    MainWindow* mainWindow;
    size_t currentNodeId;

    void findNode();
    void projectionInfo();
    void clearInfo();

    void createProjection();
};

#endif // NODEINFOFORM_H
