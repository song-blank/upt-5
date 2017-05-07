#ifndef USB_DEVICE_CONTROLLER_H
#define USB_DEVICE_CONTROLLER_H

#include <stdio.h>
#include "cyusb.h"

#define P_LINE 700	//行
#define P_LIST 512	//列
#define P_ORIGIN_SIZE P_LINE*P_LIST*2*2+512*3
#define P_GRAY_SIZE 698*1022*4

class usb_device_controller
{
public:
    usb_device_controller();
    int get_camera_data();

private:
    int init_USB();
    int init_camera();
    int get_origin_data(void);
    int select_data(unsigned char *buf);
    int combine_data();
    int change2rgb();
    int change2gray();

public:
    int camera_date_buf[P_LINE][P_LIST*2];
    int data4RGB[P_LINE][P_LIST * 2][3];
    int data4gray[P_LINE-2][P_LIST*2-2];
private:

  //  static int timeout_provided;
    int timeout;
    cyusb_handle *h1;

    int flag;

    int i, j, k;
    int n ;
    int m ;
    int x ;
    int y ;
    int a ;
    int b ;
    int c ;
    int w ;
    int w2 ;
    int z ;
    int begin_end ;



    unsigned char c_buf[32];
    unsigned char *buf;
    int transferred ;
    // unsigned char *dbuf;

    FILE *fpcam;
    FILE *fpcamy;
};

#endif // USB_DEVICE_CONTROLLER_H
