#ifndef SERIAL_DEVICE_CONTROLLER_H
#define SERIAL_DEVICE_CONTROLLER_H

#define CHAR_CMD_SIZE 16
#define CODE_INFO_SIZE 128
#define M 100
#define N 20

class Serial_Device_Controller
{
public:
    Serial_Device_Controller(int com_num);
    int init(int serial_num ,int speed, int databits, int stopbits, int parity,int mode);
    int device_self_check();
    int temperature_control();
    int check_pair_transistor_signal();
    int laser_on();
    int shut_down();

    int get_code_info();

    //int is_code_new(int *is_new);



    //int check_pair_transistor_signal();
//    int check_pair_transistor_signal();
//    int check_pair_transistor_signal();
private:
    int open_port(int serial_num);
    int set_speed(int fd, int speed);
    int set_other_attribute(int fd, int databits, int stopbits, int parity, int mode);

private:

    //串口参数
    int fd;
    int com_port;
    int speed;
    int nBits;
    char nEvent;
    int nStop;
   // char temp[1024];
    char reply[1];
    char reply_data_self[16];
    char reply_data_tem[16];
    char reply_data_pipe[16];
    char reply_data_laser[16];
    char reply_data_down[16];


    int tty_lm;//下位机描述符
    int tty_scaner;//扫描模块描述符
    int nread;//读取个数
    int nwrite;//写入取个数

    int i;//计数用

    char code_info[CODE_INFO_SIZE];//二维码数据大小

    char self_checking[CHAR_CMD_SIZE];
    char temperature[CHAR_CMD_SIZE];
    char check_pipe[CHAR_CMD_SIZE];

    char shine[CHAR_CMD_SIZE];
    char shutdown[CHAR_CMD_SIZE];
    //自检回复指令
    char self_checking_ok[CHAR_CMD_SIZE];
    char fail_reboot[CHAR_CMD_SIZE] ;
    char self_checking_no_strip[CHAR_CMD_SIZE] ;
    //温控回复指令
    //	char temperature_normal[128] ;
    //	char temperature_hot_over[];
    //	char temperature_cold_over[];
    //对管回复
    char check_pipe_ok[CHAR_CMD_SIZE];
    char check_pipe_fail[CHAR_CMD_SIZE];




};

#endif // SERIAL_DEVICE_CONTROLLER_H
