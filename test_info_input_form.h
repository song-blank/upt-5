#ifndef TEST_INFO_INPUT_FORM_H
#define TEST_INFO_INPUT_FORM_H

#include <QWidget>
#include "serial_device_controller.h"
#include "usb_device_controller.h"

namespace Ui {
class test_Info_Input_Form;
}

class test_Info_Input_Form : public QWidget
{
    Q_OBJECT

public:
    explicit test_Info_Input_Form(QWidget *parent = 0);
    Serial_Device_Controller *serial_dev;

    usb_device_controller *usb_dev;
    ~test_Info_Input_Form();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_backspace_PBtn_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_return_PBtn_clicked();

    void on_commit_PBtn_clicked();

    void process_camera_data(int data[P_LINE-2][P_LIST*2-2]);

private:
    Ui::test_Info_Input_Form *ui;
    //0,auto....1 manual
    bool is_manual_test;
};

#endif // TEST_INFO_INPUT_FORM_H
