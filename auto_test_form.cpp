#include "auto_test_form.h"
#include "ui_auto_test_form.h"

auto_Test_Form::auto_Test_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::auto_Test_Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

auto_Test_Form::~auto_Test_Form()
{
    delete ui;
}

void auto_Test_Form::on_return_PBtn_clicked()
{
    close();
}
