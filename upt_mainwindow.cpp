#include "upt_mainwindow.h"
#include "ui_upt_mainwindow.h"
#include "form.h"
#include "configure_form.h"

UPT_MainWindow::UPT_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UPT_MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

UPT_MainWindow::~UPT_MainWindow()
{
    delete ui;
}

void UPT_MainWindow::on_pushButton_clicked()
{
    Configure_Form *configure_form= new Configure_Form();
    configure_form->show();
}
