#include "form.h"
#include "ui_form.h"
//#include "qDebug"
#include "configure_form.h"
#include "qmessagebox.h"
#include "auto_test_form.h"
#include "test_info_input_form.h"
#include "test_result_history_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

}

Form::~Form()
{
    delete ui;
}

void Form::on_configure_PBn_clicked()
{
//    qDebug()<<"configure";
    Configure_Form *configure_form= new Configure_Form();
    configure_form->show();
}

void Form::on_powerButton_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "Show Qt", "确定关机退出？", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
//            QMessageBox::aboutQt(NULL, "About Qt");
        this->close();
    }
}

void Form::on_auto_test_PBn_clicked()
{
    auto_Test_Form *auto_test_form = new auto_Test_Form();
    auto_test_form->show();
}

void Form::on_mannal_test_PBn_clicked()
{
    test_Info_Input_Form *d = new test_Info_Input_Form();
    d->show();
}

void Form::on_check_history_PBn_clicked()
{
    test_Result_History_Form *d = new test_Result_History_Form();
    d->show();
}
