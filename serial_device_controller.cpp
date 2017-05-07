#include "serial_device_controller.h"
#include <stdio.h>  //标准输入输出定义
#include <string.h>
#include <sys/types.h>
#include <errno.h>  //错误号定义
#include <sys/stat.h>
#include <fcntl.h>  //文件控制定义
#include <unistd.h>  //Unix标准函数定义
#include <termios.h>  //POSIX中断控制定义
#include <stdlib.h>  //标准函数库定义
#include <sys/ioctl.h>
#include <pthread.h>

#include <QDebug>
#include <QThread>



/***********************************/
#define  RS485  0020000//rs485
#define  Normal 0020001//normal
#define  HWHD	0020002//hardware handing
#define	 Modem  0020004//modem
#define  ISO7816T0 0020010//iso7816 t=0
#define  ISO7816T1 0020020//iso7816 t=1
#define  IrDA	   0020040//IrDA
/***********************************/

static int speed_arr[] = {B230400, B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1800, B1200, B600, B300};
static int name_arr[]  = {230400,  115200,  57600,  38400,  19200,  9600,  4800,  2400,  1800,  1200,  600,  300};



Serial_Device_Controller::Serial_Device_Controller(int com)
{
	char self_checking1[CHAR_CMD_SIZE] = {0xee,0x06,0x00,0x00,0x06,0x99};
	char temperature1[CHAR_CMD_SIZE] = {0xee,0x06,0x00,0x01,0x07,0x99};
	char check_pipe1[CHAR_CMD_SIZE] = {0xee,0x06,0x00,0x02,0x08,0x99};

	char shine1[CHAR_CMD_SIZE] = {0xee,0x06,0x00,0x04,0x0A,0x99};
	char shutdown1[CHAR_CMD_SIZE] = {0xee,0x06,0x00,0x05,0x0B,0x99};
	//自检回复指令
	char self_checking_ok1[CHAR_CMD_SIZE] = {0xee,0x07,0x00,0x00,0x00,0x07,0x99};
	char fail_reboot1[CHAR_CMD_SIZE] = {0xee,0x07,0x00,0x00,0x01,0x08,0x99};
	char self_checking_no_strip1[CHAR_CMD_SIZE] = {0xee,0x07,0x00,0x00,0x02,0x09,0x99};
	//温控回复指令
	//	char temperature_normal[16] = {0xee,0x09,0x00,0x01,0x00,0xXX,0xXX,0xXX,0x99};
	//	char temperature_hot_over[16] = {0xee,0x09,0x00,0x01,0x01,0xXX,0xXX,0xXX,0x99};
	//	char temperature_cold_over[16] = {0xee,0x09,0x00,0x01,0x01,0xXX,0xXX,0xXX,0x99};
	//对管回复
	char check_pipe_ok1[CHAR_CMD_SIZE] = {0xee,0x07,0x00,0x02,0x00,0x09,0x99};
	char check_pipe_fail1[CHAR_CMD_SIZE] = {0xee,0x07,0x00,0x02,0x01,0x0A,0x99};

	memmove(self_checking,self_checking1,CHAR_CMD_SIZE);
	memmove(temperature,temperature1,CHAR_CMD_SIZE);
	memmove(check_pipe,check_pipe1,CHAR_CMD_SIZE);
	memmove(shine,shine1,CHAR_CMD_SIZE);
	memmove(shutdown,shutdown1,CHAR_CMD_SIZE);
	memmove(self_checking_ok,self_checking_ok1,CHAR_CMD_SIZE);
	memmove(fail_reboot,fail_reboot1,CHAR_CMD_SIZE);
	memmove(self_checking_no_strip,self_checking_no_strip1,CHAR_CMD_SIZE);
	memmove(check_pipe_ok,check_pipe_ok1,CHAR_CMD_SIZE);
	memmove(check_pipe_fail,check_pipe_fail1,CHAR_CMD_SIZE);


}

int Serial_Device_Controller::init(int serial_num,int speed, int databits,int stopbits, int parity, int mode)
{

	if((fd=open_port(serial_num))<0)//打卡串口
	{
		perror("open_port error");
		return 0;
	}
	set_speed(fd, speed);
	set_other_attribute(fd, databits, stopbits, parity, mode);
	qDebug("init");
	return fd;

}

int Serial_Device_Controller::device_self_check()
{
	qDebug("device_auto_check");
	tty_lm = init(3,115200,8,1,0,0); //要对应 串口号和波特率
	if(tty_lm < 0){
		qDebug("tty_lm=%d",tty_lm);
		return -0x1001;
	}

	nwrite=write(tty_lm,self_checking,6);
	if(nwrite < 0){
		qDebug("nwrite=%d",nwrite);
		return -0x1002;
	}


	nread = read(tty_lm,reply,1);
	if(nread < 0){
		qDebug("nread=%d",nread);
		return -0x1003;
	}
	for(i = 0;i < nread;i++){
		printf("reply[%d]=%#X\n",i,reply[i]);
	}

	if(reply[0] == 0xAA){
		sleep(4);
		nread = read(tty_lm,reply_data_self,14);
		if(nread < 0){
			qDebug("nread=%d",nread);
			return -0x1004;
		}
		for(i = 0;i < nread;i++){
			printf("reply_data_self[%d]=%#X\n",i,reply_data_self[i]);
		}



		if(reply_data_self[4]==0x00 && reply_data_self[5]==0x07){
			close(tty_lm);
			return 0;//自检成功，下位机就绪
		}
		if(reply_data_self[4]==0x01 && reply_data_self[5]==0x08){
			close(tty_lm);
			return 0x0001;//温控模块功能异常，下位机自检失败，请重启
		}

		if(reply_data_self[11]==0x00 && reply_data_self[12]==0x07){
			close(tty_lm);
			return 0x0002;//光电对管被遮挡，下位机自检失败，请勿插入试纸
		}
	}
	else
		close(tty_lm);
	return -1;

}

int Serial_Device_Controller::temperature_control()
{
	qDebug("temperature_control");

	tty_lm = init(3,115200,8,1,0,0); //要对应 串口号和波特率
	if(tty_lm < 0){
		qDebug("tty_lm=%d",tty_lm);
		return -0x1001;
	}

	nwrite=write(tty_lm,temperature,6);
	if(nwrite < 0){
		qDebug("nwrite=%d",nwrite);
		return -0x1002;
	}


	nread = read(tty_lm,reply,1);
	if(nread < 0){
		qDebug("nread=%d",nread);
		return -0x1003;
	}
	for(i = 0;i < nread;i++){
		printf("reply[%d]=%#X\n",i,reply[i]);
	}

	if(reply[0] == 0xAA){
		sleep(10);
		nread = read(tty_lm,reply_data_tem,9);
		if(nread < 0){
			qDebug("nread=%d",nread);
			return -0x1004;
		}
		for(i = 0;i < nread;i++){
			printf("reply_data_tem[%d]=%#X\n",i,reply_data_tem[i]);
		}


		close(tty_lm);
	}
	else
		close(tty_lm);
	return -1;



}
int Serial_Device_Controller::check_pair_transistor_signal()
{

	qDebug("check_pair_transistor_signal");
	tty_lm = init(3,115200,8,1,0,0); //要对应 串口号和波特率
	if(tty_lm < 0){
		qDebug("tty_lm=%d",tty_lm);
		return -0x1001;
	}

	nwrite=write(tty_lm,check_pipe,6);
	if(nwrite < 0){
		qDebug("nwrite=%d",nwrite);
		return -0x1002;
	}


	nread = read(tty_lm,reply,1);
	if(nread < 0){
		qDebug("nread=%d",nread);
		return -0x1003;
	}
	for(i = 0;i < nread;i++){
		printf("reply[%d]=%#X\n",i,reply[i]);
	}

	if(reply[0] == 0xAA){
		sleep(2);
		nread = read(tty_lm,reply_data_pipe,7);
		if(nread < 0){
			qDebug("nread=%d",nread);
			return -0x1004;
		}
		for(i = 0;i < nread;i++){
			printf("reply_data_pipe[%d]=%#X\n",i,reply_data_pipe[i]);
		}


		if(reply_data_self[4]==0x00 && reply_data_self[5]==0x09){
			close(tty_lm);
			return 0x0003;//光电对管没有被遮挡，可以进行校准
		}

		if(reply_data_self[4]==0x01 && reply_data_self[5]==0x0A){
			close(tty_lm);
			return 0x0004;//光电对管被遮挡，不能进行校准
		}


		close(tty_lm);
	}
	else
		close(tty_lm);
	return -1;


}
int Serial_Device_Controller::laser_on()
{
	qDebug("laser_on");
	tty_lm = init(3,115200,8,1,0,0); //要对应 串口号和波特率
	if(tty_lm < 0){
		qDebug("tty_lm=%d",tty_lm);
		return -0x1001;
	}

	nwrite=write(tty_lm,shine,6);
	if(nwrite < 0){
		qDebug("nwrite=%d",nwrite);
		return -0x1002;
	}


	nread = read(tty_lm,reply,1);
	if(nread < 0){
		qDebug("nread=%d",nread);
		return -0x1003;
	}
	for(i = 0;i < nread;i++){
		printf("reply[%d]=%#X\n",i,reply[i]);
	}

	if(reply[0] == 0xAA){
		close(tty_lm);
		return 0;
	}
	else

		close(tty_lm);
	return -1;

}
int Serial_Device_Controller::shut_down()
{
	qDebug("shut_down");
	tty_lm = init(3,115200,8,1,0,0); //要对应 串口号和波特率
	if(tty_lm < 0){
		qDebug("tty_lm=%d",tty_lm);
		return -0x1001;
	}

	nwrite=write(tty_lm,shutdown,6);
	if(nwrite < 0){
		qDebug("nwrite=%d",nwrite);
		return -0x1002;
	}

	nread = read(tty_lm,reply,128);
	if(nread < 0){
		qDebug("nread=%d",nread);
		return -0x1003;
	}
	for(i = 0;i < nread;i++){
		printf("reply[%d]=%#X\n",i,reply[i]);
	}

	if(reply[0] == 0xAA){
		close(tty_lm);
		return 0;
	}
	else

		close(tty_lm);
	return -1;
}

//return old_code,new_code,error_code.
//differ with code DataBase.(code info txt OR Sqilt3)
int Serial_Device_Controller::get_code_info()
{
	tty_scaner = init(4,9600,8,1,0,0); //要对应 串口号和波特率
	if(tty_lm < 0){
		qDebug("tty_lm=%d",tty_lm);
		return -0x1001;
	}


	FILE * gpio_file=NULL;

	qDebug("get_code_info");
	gpio_file = fopen("/sys/class/gpio/export","w");
	fprintf(gpio_file,"%d",46);
	fclose(gpio_file);
	gpio_file = fopen("/sys/class/gpio/gpio46/direction","w");
	fprintf(gpio_file,"out");
	fclose(gpio_file);

	gpio_file = fopen("/sys/class/gpio/gpio46/value","w");
	fprintf(gpio_file,"%d",1);
	usleep(100);
	fclose(gpio_file);

	gpio_file = fopen("/sys/class/gpio/gpio46/value","w");
	fprintf(gpio_file,"%d",0);
	usleep(100);
	fclose(gpio_file);

	usleep(100);

	i=4;
	while(i){
		nread = read(tty_scaner,(void *)code_info,sizeof(code_info));
		// printf("nread=%d,data=%s\n",nread,code_info);
		if(nread > 30){
			printf("%s\n", code_info);

			gpio_file = fopen("/sys/class/gpio/gpio46/value","w");
			fprintf(gpio_file,"%d",0);
			fclose(gpio_file);

			qDebug("%s",code_info);
			return 0;
		}
		if(nread < 0){
			printf("read error\n");
		}
		usleep(100000);
		i=i-1;
	}
	close(tty_scaner);
	if(nread < 30)
		qDebug("short code\n");
	gpio_file = fopen("/sys/class/gpio/gpio46/value","w");
	fprintf(gpio_file,"%d",0);
	fclose(gpio_file);
	return 0x0005;//读取个数少于条码数


}

int Serial_Device_Controller::open_port(int serial_num)
{

	if (serial_num==1)//串口1
	{
		fd = open( "/dev/ttyO1", O_RDWR|O_NOCTTY);
		if (-1 == fd){
			perror("Can't Open Serial Port");
			return(-1);
		}
	}
	else if(serial_num==2)//串口2
	{
		fd = open( "/dev/ttyO2", O_RDWR|O_NOCTTY);
		if (-1 == fd){
			perror("Can't Open Serial Port");
			return(-1);
		}
	}
	else if (serial_num==3)//串口3
	{
		fd = open( "/dev/ttyO3", O_RDWR|O_NOCTTY,1);
		if (-1 == fd){
			perror("Can't Open Serial Port");
			return(-1);
		}
	}
	else if (serial_num==4)//串口4
	{
		fd = open( "/dev/ttyO4", O_RDWR|O_NOCTTY);
		if (-1 == fd){
			perror("Can't Open Serial Port");
			return(-1);
		}
	}
	else if (serial_num==5)//串口5
	{
		fd = open( "/dev/ttyO5", O_RDWR|O_NOCTTY);
		if (-1 == fd){
			perror("Can't Open Serial Port");
			return(-1);
		}
	}

	/*恢复串口为阻塞状态*/

	if(fcntl(fd, F_SETFL, 0)<0)
		printf("fcntl failed!\n");
	else
		//printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));

		/*测试是否为终端设备*/

		if(isatty(STDIN_FILENO)==0)
			printf("standard input is not a terminal device\n");
	//	else

	//printf("isatty success!\n");
	//printf("fd-open=%d\n",fd);
	return fd;
}


int  Serial_Device_Controller::set_speed(int fd, int speed)
{
	int i, status;
	struct termios Opt;

	tcgetattr(fd, &Opt);

	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++) {
		if (speed == name_arr[i]) {

			tcflush(fd, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);

			status = tcsetattr(fd, TCSANOW, &Opt);
			if  (status != 0) {
				printf("tcsetattr failed\n");
				return -1;
			}

			tcflush(fd,TCIOFLUSH);

			return 0;
		}
	}

	return -1;
}


int  Serial_Device_Controller::set_other_attribute(int fd, int databits, int stopbits, int parity, int mode)
{
	struct termios options;

	if (tcgetattr(fd, &options) != 0) {
		printf("tcgetattr failed\n");
		return -1;
	}

	options.c_cflag &= ~CSIZE;
	switch (databits) /*ÉÖÊ¾Ý»Ê*/
	{
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr,"Unsupported data size\n");
		return -1;
	}

	switch (parity)
	{
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB;   /* Clear parity enable */
		options.c_iflag &= ~INPCK;     /* Enable parity checking */
		break;
	case 'o':
	case 'O':
	case 1:
		options.c_cflag |= (PARODD | PARENB); /* ÉÖΪÆЧÑ*/
		options.c_iflag |= INPCK;             /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
	case 2:
		options.c_cflag |= PARENB;     /* Enable parity */
		options.c_cflag &= ~PARODD;    /* ת»»ΪżЧÑ*/
		options.c_iflag |= INPCK;      /* Disnable parity checking */
		break;
	case 'S':
	case 's':  /*as no parity*/
	case 0:
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported parity\n");
		return -1;
	}
	/* ÉÖֹͣλ*/
	switch (stopbits)
	{
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr,"Unsupported stop bits\n");
		return -1;
	}
	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;

	tcflush(fd,TCIFLUSH);
	options.c_iflag = 0;
	options.c_oflag = 0;
	options.c_lflag = 0;
	options.c_cc[VTIME] = 150; /* ÉÖ³¬ʱ15 seconds*/
	options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
	//	options.c_oflag 0;

	switch(mode)
	{
	case 1://rs485
		options.c_cflag |= RS485;
		printf("Enable RS485 !\n");
		break;
	case 2://HWHD
		options.c_cflag |= HWHD;
		printf("Enable HWHD !\n");
		break;
	case 3://Modem
		options.c_cflag |= Modem;
		printf("Enable Modem !\n");
		break;
	case 4://ISO7816T0
		options.c_cflag |= ISO7816T0;
		printf("Enable ISO7816 T=0 \n");
		break;
	case 5://ISO7816T1
		options.c_cflag |= ISO7816T1;
		printf("Enable ISO7816 T=1 \n");
		break;
	case 6://IrDA
		options.c_cflag |= IrDA;
		printf("Enable IrDA \n");
		break;
	default:
		break;

	}

	if (tcsetattr(fd,TCSANOW,&options) != 0)
	{
		perror("SetupSerial 3");
		return -1;
	}

#if 0
	tcgetattr(fd, &options);
	printf("c_iflag: %x\rc_oflag: %x\n", options.c_iflag, options.c_oflag);
	printf("c_cflag: %x\nc_lflag: %x\n", options.c_cflag, options.c_lflag);
	printf("c_line: %x\nc_cc[VTIME]: %d\nc_cc[VMIN]: %d\n", options.c_line, options.c_cc[VTIME], options.c_cc[VMIN]);
#endif
	return 0;
}
