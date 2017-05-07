#ifndef CONFIGURE_FORM_H
#define CONFIGURE_FORM_H

#include <QWidget>

namespace Ui {
class Configure_Form;
}

class Configure_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Configure_Form(QWidget *parent = 0);
    ~Configure_Form();

private slots:
    void on_return_PBtn_clicked();

private:
    Ui::Configure_Form *ui;
};

#endif // CONFIGURE_FORM_H
