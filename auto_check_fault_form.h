#ifndef AUTO_CHECK_FAULT_FORM_H
#define AUTO_CHECK_FAULT_FORM_H

#include <QWidget>

namespace Ui {
class Auto_Check_Fault_Form;
}

class Auto_Check_Fault_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Auto_Check_Fault_Form(QWidget *parent = 0);
    ~Auto_Check_Fault_Form();

private:
    Ui::Auto_Check_Fault_Form *ui;
};

#endif // AUTO_CHECK_FAULT_FORM_H
