#include "test_result_history_form.h"
#include "ui_test_result_history_form.h"

test_Result_History_Form::test_Result_History_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test_Result_History_Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

test_Result_History_Form::~test_Result_History_Form()
{
    delete ui;
}

void test_Result_History_Form::on_return_PBtn_clicked()
{
    close();
}
