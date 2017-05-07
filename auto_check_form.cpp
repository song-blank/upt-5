#include "auto_check_form.h"
#include "ui_auto_check_form.h"

Auto_Check_Form::Auto_Check_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Auto_Check_Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Auto_Check_Form::~Auto_Check_Form()
{
    delete ui;
}
