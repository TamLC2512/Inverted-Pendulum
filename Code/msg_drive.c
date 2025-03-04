#include "msg_drive.h"
#include <stdlib.h>
#include <stdio.h>

void basic_msg(char uart, int val)
{
	char num[10];
	//itoa(val,num, 10);
	sprintf(num, "%d", val);
	UART_SEND(uart,num);
	str_empty(num);
	UART_TX(2,'\n');
}


