#include "quality_control_form.h"
#include "ui_quality_control_form.h"

Quality_Control_Form::Quality_Control_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Quality_Control_Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Quality_Control_Form::~Quality_Control_Form()
{
    delete ui;
}

void Quality_Control_Form::on_return_PBtn_clicked()
{
    close();
}
