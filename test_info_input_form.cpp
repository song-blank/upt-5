#include "test_info_input_form.h"
#include "ui_test_info_input_form.h"
#include <qmessagebox.h>
#include <qdebug.h>

test_Info_Input_Form::test_Info_Input_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::test_Info_Input_Form)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    is_manual_test=true;
    serial_dev = new Serial_Device_Controller(1);
//    serial_dev->device_self_check();
//    serial_dev->temperature_control();
//    serial_dev->check_pair_transistor_signal();
//    serial_dev->laser_on();
//   // serial_dev->shut_down();//  ???
//    serial_dev->get_code_info();
//    delete serial_dev;


    usb_dev = new usb_device_controller();

   usb_dev->get_camera_data();
   delete usb_dev;
   
}

test_Info_Input_Form::~test_Info_Input_Form()
{
    delete ui;
}

void test_Info_Input_Form::on_pushButton_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "1";
    ui->test_ID_Ledit->setText(str);
}

void test_Info_Input_Form::on_pushButton_2_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "2";
    ui->test_ID_Ledit->setText(str);
}

void test_Info_Input_Form::on_pushButton_3_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "3";
    ui->test_ID_Ledit->setText(str);
}

void test_Info_Input_Form::on_pushButton_4_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "4";
    ui->test_ID_Ledit->setText(str);
}

void test_Info_Input_Form::on_pushButton_5_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "5";
    ui->test_ID_Ledit->setText(str);
}

void test_Info_Input_Form::on_pushButton_6_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "6";
    ui->test_ID_Ledit->setText(str);
}

void test_Info_Input_Form::on_pushButton_7_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "7";
    ui->test_ID_Ledit->setText(str);
}

void test_Info_Input_Form::on_pushButton_8_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "8";
    ui->test_ID_Ledit->setText(str);
}

void test_Info_Input_Form::on_pushButton_9_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "9";
    ui->test_ID_Ledit->setText(str);
}

void test_Info_Input_Form::on_pushButton_10_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str += "0";
    ui->test_ID_Ledit->setText(str);
}



void test_Info_Input_Form::on_backspace_PBtn_clicked()
{
    QString str = ui->test_ID_Ledit->text();
    str=str.mid(0,str.length()-1);
    ui->test_ID_Ledit->setText(str);
}



void test_Info_Input_Form::on_return_PBtn_clicked()
{
    close();
//    hide();
}

void test_Info_Input_Form::on_commit_PBtn_clicked()
{
    //读取对管信号，若未得到对管信号，则一直停在此界面，此时用户可以点击“返回”按键返回至上一个界面；若对管插到位，则读条码。

    //check_pair_transistor_signal();

    //if error
    //显示提示信息让用户插入正确试纸条

    //if ok

        //check code is old or new?

        //if old
        //弹出旧条码对话框，提示说明旧条码的状态

        //if new
            //if auto_test
                //弹出自动测量对话框，然后进行自动测量。
            //if mannal_test
                //进行检测，完毕后弹出UPT检测结果对话框。
                //1.激光器出光,下位机自动关光。
                //2.采集图像。
                //3.对采集到的图像数据进行处理，显示检测结果对话框，选择继续或者打印。
                //4.若继续则ID号加1，并保存结果。
   int status;
   int is_new;
   status = serial_dev->check_pair_transistor_signal();
   //debug()

   if(status!=8777)
   {
       status = serial_dev->get_code_info();
       qDebug()<<"get_code_info status is"<<status;
       if(status ==0)
       {
            //serial_dev->is_code_new(is_new);
           if(is_new!=999)
           {
               if(is_manual_test)
               {
                    serial_dev->laser_on();
                    usb_dev->get_camera_data();
                    qDebug("get_camera_data");
                    process_camera_data(usb_dev->data4gray);
                    QMessageBox::StandardButton rb = QMessageBox::question(NULL, "Continue", "Continue or Print?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                    if(rb == QMessageBox::Yes)
                    {
                //            QMessageBox::aboutQt(NULL, "About Qt");
                        qDebug()<<"Continue";

                    }
                    else
                    {
                        qDebug()<<"Print";
                    }


               }
               else
               {
                    qDebug()<<"Auto test";
               }
           }
           else
           {
               qDebug()<<"get_code_info is old"<<status;
           }
       }
       else
       {
            qDebug()<<"Code is old....";
       }
   }
   else
   {
       qDebug()<<"check_pair_transistor_signal error!";
   }

}


void test_Info_Input_Form::process_camera_data(int data[P_LINE-2][P_LIST*2-2])
{
    qDebug()<<"in process_camera_data..";
    for(int i=0;i<P_LINE-2;i++)
    {
        for(int j=0;j<P_LIST*2-2;j++)
        {
            qDebug()<<data[i][j];
        }
        break;
    }
    qDebug()<<"process_camera_data over.";
}
