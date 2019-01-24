#ifndef _UART_H_
#define _UART_H_ 

#include "comm.h"


extern int uart_open(char * serial_port);    //uart_open("/dev/ttyUSB0"); 
extern void uart_close(int fd);
extern int uart_init(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity);
extern int uart_recv(int fd, char *rcv_buf,int data_len,struct timeval *timeout);
extern int uart_send(int fd, char *send_buf,int data_len);
extern int uart_dev_option(int *uart_fd, int speed);


#endif 

//recv_buf:   char recv_buf[4]={0};    8bit
//send_buf:   char send_buf[4]={0};    8bit
