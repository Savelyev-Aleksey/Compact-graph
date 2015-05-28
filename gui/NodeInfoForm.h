#ifndef NODEINFOFORM_H
#define NODEINFOFORM_H

#include <QWidget>

namespace Ui {
class NodeInfoForm;
}

class NodeInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit NodeInfoForm(QWidget *parent = 0);
    ~NodeInfoForm();

private:
    Ui::NodeInfoForm *ui;
};

#endif // NODEINFOFORM_H
