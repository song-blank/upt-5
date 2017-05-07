#include "upt_test_result_form.h"
#include "ui_upt_test_result_form.h"

UPT_Test_Result_Form::UPT_Test_Result_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UPT_Test_Result_Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

UPT_Test_Result_Form::~UPT_Test_Result_Form()
{
    delete ui;
}

void UPT_Test_Result_Form::on_return_PBtn_clicked()
{
//    hide();
    close();
}
