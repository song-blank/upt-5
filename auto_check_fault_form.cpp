#include "auto_check_fault_form.h"
#include "ui_auto_check_fault_form.h"

Auto_Check_Fault_Form::Auto_Check_Fault_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Auto_Check_Fault_Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Auto_Check_Fault_Form::~Auto_Check_Fault_Form()
{
    delete ui;
}
