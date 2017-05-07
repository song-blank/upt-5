#include "configure_form.h"
#include "ui_configure_form.h"

Configure_Form::Configure_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Configure_Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

Configure_Form::~Configure_Form()
{
    delete ui;
}

void Configure_Form::on_return_PBtn_clicked()
{
    close();
}
