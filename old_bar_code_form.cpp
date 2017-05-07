#include "old_bar_code_form.h"
#include "ui_old_bar_code_form.h"

old_Bar_Code_Form::old_Bar_Code_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::old_Bar_Code_Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

old_Bar_Code_Form::~old_Bar_Code_Form()
{
    delete ui;
}

void old_Bar_Code_Form::on_return_PBtn_clicked()
{
    close();
}
