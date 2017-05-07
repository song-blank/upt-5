#include "usb_device_controller.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cyusb.h"
#include <qdebug.h>

#include <QDebug>
#define LEFT_PART 0
#define RIGHT_PART 1

#define OK      0
#define FAILED  -1

usb_device_controller::usb_device_controller()
{
	timeout =0;
	h1 = NULL;
	//flag =0;
	//  h1 = n=NULL;



}

int usb_device_controller:: get_camera_data()
{


    int r = 0;
	qDebug("in get_camera_data..");
    r = init_USB();//may be can move to usb_device_controller::usb_device_controller()...
    if(r == 0x1020){
        return 0x1020;
    }
    if(r == 0x1021){
        return 0x1021;
    }

	flag = LEFT_PART;
    r = init_camera();
    if(r == 0x1022){
        return 0x1022;
    }
    r = get_origin_data();
        if(r == 0x1023){
            return 0x1023;
        }
    r = select_data(buf);
    if(r == 0x1024){
        return 0x1024;
    }
	combine_data();

	flag = RIGHT_PART;
    r = init_camera();
    if(r == 0x1022){
        return 0x1022;
    }
    r = get_origin_data();
        if(r == 0x1023){
            return 0x1023;
        }
    r = select_data(buf);
    if(r == 0x1024){
        return 0x1024;
    }
	combine_data();
	change2rgb();
	change2gray();

	return 0;

}

int usb_device_controller::init_USB()
{

	int r;

	r = cyusb_open();

	if ( r < 0 ) {
		printf("Error opening library\n");
        return 0x1020;
	}
	else if ( r == 0 ) {
		printf("No device found\n");
        return 0x1021;
	}
	if ( r > 1 ) {
		printf("More than 1 devices of interest found. Disconnect unwanted devices\n");
        return 0x1020;
	}

	h1 = cyusb_gethandle(0);

	if ( cyusb_getvendor(h1) != 0x04b4 ) {
		printf("Cypress chipset not detected\n");
		cyusb_close();
        return 0x1020;
	}

	r = cyusb_kernel_driver_active(h1, 0);
	if ( r != 0 ) {
		printf("kernel driver active. Exitting\n");
		cyusb_close();
        return 0x1020;
	}

	r = cyusb_claim_interface(h1, 0);
	if ( r != 0 ) {
		printf("Error in claiming interface\n");
		cyusb_close();
        return 0x1020;
	}
    else
		printf("Successfully claimed interface\n");
		return 0;

}

//判断数据头和尾
int usb_device_controller::select_data(unsigned char *buf)
{
	w = 0;
	w2 = 0;


	z = 0;
	for(i=0; i < P_ORIGIN_SIZE ; i++){
		if((buf[i] == 0x00)){
			for(n=0; n< 512;n++){
				if(buf[i] == 0){
					z++;
					//     printf("z=%d,n=%d,i=%d,dbuf[i]=%x\n",z,n,i,buf[i]);
					i++;
				}
				else{
					z = 0;
					n = 0;
					i++;
				}

				if(i > P_LINE*P_LIST*2+1024){
					qDebug("overflow hand i=%d",i);
                    return 0x1024;

				}
				if( z== 511){
					w = i;
					qDebug("break hand w=%d",w);

					goto date_tail;
				}


			}
		}
	}

date_tail:
	printf(" buf[%d]=%x,buf[w+1]=%x\n",w,buf[w],buf[w+1]);
	printf(" buf[%d]=%x,buf[w+1]=%x\n",w+704000,buf[w+704000],buf[w+704000+1]);
	z = 0;
	for(i = w+704000; i < w+P_LINE*P_LIST*2+512 ; i++){
		if((buf[i] == 0x00)){
			for(n=0; n< 512;n++){
				if(buf[i] == 0){
					z++;
					//   printf("z=%d,n=%d,i=%d,dbuf[i]=%x\n",z,n,i,buf[i]);
					i++;
				}
				else{
					z = 0;
					n = 0;
					i++;
				}
				if(i > w+P_LINE*P_LIST*2+512){
					qDebug("overflow tail i=%d",i);
                    return 0x1024;

				}
				if( z== 511){
					w2 = i;
					qDebug("break tail w=%d",w2);
					goto end;
					// break;
				}

			}
		}
	}

end:
	//  printf("w=%d, dbuf[w]=%x,dbuf[w+1]=%x,dbuf[i = w+700*1024*2]=%d\n\n",w2,buf[w],buf[w+1],buf[w+700*1024*2]);

	qDebug("begin W=%d",w);

	return OK;

}

//合拼数据
int usb_device_controller::combine_data()
{
	w =w +1;


	//把数据存放数组中

	int c=0;
	qDebug("flag==%d\n",flag);
	if(flag == LEFT_PART){
		for( i = 0; i < P_LINE; i++){
			for( j = 0; j < P_LIST; j++){

				camera_date_buf[i][j] = (buf[ w + 2*c  ] | (( buf[ w + 2*c + 1]) << 8)) & 0x0fff;
				c = c +1;

			}
		}

		free(buf);
	}




	qDebug("flag==%d\n",flag);
	if(flag == RIGHT_PART){
		for( i = 0; i < P_LINE; i++){
			for( j = 0; j < P_LIST; j++){
				camera_date_buf[i][j+512] = (buf[ w + 2*c  ] | (( buf[ w + 2*c + 1]) << 8)) & 0x0fff;
				c = c +1;

			}
		}
		free(buf);
	}




	qDebug("flag==%d\n",flag);


	if((fpcam = fopen("cam.txt","a")) == NULL){
		printf("fopen Error\n");
		return -1;
	}
	if(flag ==RIGHT_PART){
		for( i = 0; i < P_LINE; i++){
			for( j = 0; j < P_LIST*2; j++){
				fprintf(fpcam,"%04x ",camera_date_buf[i][j]);


				if(j == P_LIST*2-1){
					fprintf(fpcam,"\n");
				}

				//  printf("%04x \n",camera_date_buf[i][j]);
			}
		}
	}
	fclose(fpcam);

	return 0;


}
//转rgb
int usb_device_controller::change2rgb()
{
	qDebug("change_rgb");
	for( i = 1; i < ( P_LINE - 2 ); i++){
		n = i ;
		m = 1;
		b = 0;
		if( (i%2) == 1 ){
			for(c = 0;c < ((P_LIST*2 - 2)/2);c++){
				//1*1
				data4RGB[a][b][1] = ((camera_date_buf[n][m] & 0x0fff)) * 256 /4096;//G
				data4RGB[a][b][0] = ((( camera_date_buf[n-1][m] + camera_date_buf[n+1][m] ) / 2) & 0x0fff)*256/4096;//R
				data4RGB[a][b][2] = ((( camera_date_buf[n][m-1] + camera_date_buf[n][m+1] ) / 2) & 0x0fff)*256/4096;//B
				m = m + 1;
				b = b + 1;
				//1*2
				data4RGB[a][b][2] = (camera_date_buf[n][m] & 0x0fff)*256/4096;//B
				data4RGB[a][b][0] = (((camera_date_buf[n-1][m-1] + camera_date_buf[n-1][m+1]+camera_date_buf[n+1][m-1]+camera_date_buf[n+1][m+1])/4) & 0x0fff)*256/4096;//R
				data4RGB[a][b][1] = (((camera_date_buf[n-1][m] + camera_date_buf[n][m-1] + camera_date_buf[n][m+1] + camera_date_buf[n+1][m])/4) & 0x0fff)*256/4096;//G

				b++;
				m++;

			}

		}
		if((i%2) == 0){
			b = 0;
			for(c = 0;c < ((P_LIST*2 - 2) / 2);c++){
				//2*1
				data4RGB[a][b][0] = (camera_date_buf[n][m] & 0x0fff)*256/4096;//R
				data4RGB[a][b][2] = (((camera_date_buf[n-1][m-1] + camera_date_buf[n-1][m+1] + camera_date_buf[n+1][m-1] + camera_date_buf[n+1][m+1])/4) & 0x0fff)*256/4096;//B
				data4RGB[a][b][1] = (((camera_date_buf[n-1][m] + camera_date_buf[n][m-1]+camera_date_buf[n][m+1]+camera_date_buf[n+1][m])/4) & 0x0fff)*256/4096;//G
				m = m + 1;
				b = b + 1;
				//2*2
				data4RGB[a][b][1] = (camera_date_buf[n][m] & 0x0fff)*256/4096;//G
				data4RGB[a][b][0] = ((( camera_date_buf[n][m-1] + camera_date_buf[n][m+1] ) / 2) & 0x0fff)*256/4096;//R
				data4RGB[a][b][2] = ((( camera_date_buf[n-1][m] + camera_date_buf[n+1][m] ) / 2) & 0x0fff)*256/4096;//B

				m++;
				b++;

			}
		}

		a++;

	}
	return 0;


}

//转灰度
int usb_device_controller::change2gray()
{


	if((fpcamy = fopen("camy.txt","a")) == NULL){
		printf("fopen Error\n");
		return -1;
	}

	for( a=0; a < (P_LINE - 2); a++){
		for( b=0; b<(P_LIST*2 - 2); b++){

			data4gray[a][b] = (data4RGB[a][b][0]*299 +data4RGB[a][b][1]*587 +data4RGB[a][b][2]*114 +500)/1000;
			// printf("gray[%d][%d]=%d,RGB[155][5][1]=%d\n",a,b,data4gray[a][b],data4RGB[155][5][2]);

			fprintf(fpcamy,"%03d ",data4gray[a][b]);
			if(b  == P_LIST*2-3){
				fprintf(fpcamy,"\n");
			}



		}
	}

    fclose(fpcamy);
	cyusb_close();
    qDebug("change2gray finish");
	return 0;

}
//camera配置
int usb_device_controller::init_camera()
{
	memset(c_buf,'\0',32);
	short wLength = 4;

	int r = 0;

	if(flag == LEFT_PART){
        //	r = cyusb_control_transfer(h1,0x40,0xb1,0x3012,0x384,c_buf,wLength,timeout*1000);
        r = cyusb_control_transfer(h1,0x40,0xaa,0x01,0x01,c_buf,wLength,timeout*1000);
        if(r < 0){
            printf("cyusb_control_transfer\n");
            return 0x1022;
        }
		sleep(1);
		//
        r = cyusb_control_transfer(h1,0x40,0xb1,0x3004,0x0,c_buf,wLength,timeout*1000);
		if(r < 0){
			printf("cyusb_control_transfer\n");
            return 0x1022;
		}

        r = cyusb_control_transfer(h1,0x40,0xb1,0x3008,0x1ff,c_buf,wLength,timeout*1000);
		if(r < 0){
			printf("cyusb_control_transfer\n");
            return 0x1022;
		}
		//printf("c_buf=%02x,%02x,%02x,%02x\n",c_buf[0],c_buf[1],c_buf[2],c_buf[3]);

		//r = cyusb_control_transfer(h1,0x40,0xaa,0x01,0x01,c_buf,wLength,timeout*1000);
		//if(r < 0){
		//	printf("cyusb_control_transfer\n");
		//}
		//		printf("c_buf=%02x,%02x,%02x,%02x\n",c_buf[0],c_buf[1],c_buf[2],c_buf[3]);
        sleep(1);
	}

	if(flag == RIGHT_PART){
		//	r = cyusb_control_transfer(h1,0x40,0xaa,0x01,0x01,c_buf,wLength,timeout*1000);
		//	sleep(1);
        r = cyusb_control_transfer(h1,0x40,0xb1,0x3004,0x200,c_buf,wLength,timeout*1000);
		if(r < 0){
			printf("cyusb_control_transfer\n");
            return 0x1022;
		}
    //	printf("c_buf=%02x,%02x,%02x,%02x\n",c_buf[0],c_buf[1],c_buf[2],c_buf[3]);
        r = cyusb_control_transfer(h1,0x40,0xb1,0x3008,0x3ff,c_buf,wLength,timeout*1000);
		if(r < 0){
			printf("cyusb_control_transfer\n");
            return 0x1022;
		}
    //	printf("c_buf=%02x,%02x,%02x,%02x\n",c_buf[0],c_buf[1],c_buf[2],c_buf[3]);

		//	r = cyusb_control_transfer(h1,0x40,0xb1,0x300a,0x10dc,c_buf,wLength,timeout*1000);
		//	r = cyusb_control_transfer(h1,0x40,0xaa,0x01,0x01,c_buf,wLength,timeout*1000);
		//	if(r < 0){
		//		printf("cyusb_control_transfer\n");
		//	}
		//	printf("c_buf=%02x,%02x,%02x,%02x\n",c_buf[0],c_buf[1],c_buf[2],c_buf[3]);
        sleep(1);
		flag = RIGHT_PART;
        r = cyusb_control_transfer(h1,0x40,0xb1,0x3012,0x384,c_buf,wLength,timeout*1000);
        if(r < 0){
            printf("cyusb_control_transfer\n");
            return 0x1022;
        }
	}
	return 0;

}

//读取数据
int usb_device_controller::get_origin_data(void)
{

	int res = 0;

	transferred = 0;
	//    buf = new unsigned char[P_SIZE];
	buf = (unsigned char *)malloc(P_ORIGIN_SIZE);

	res = cyusb_bulk_transfer(h1, 0x86, buf, P_ORIGIN_SIZE, &transferred, 10000);
	printf("r=%d,transferred=%d\n",res,transferred);

	if ( res == 0 ) {
		printf("cyusb_bulk_transfer ok\n\n");

		return OK;
	}
	else {
		cyusb_error(res);
		cyusb_close();
        return 0x1023;
	}

	return 0;
}

