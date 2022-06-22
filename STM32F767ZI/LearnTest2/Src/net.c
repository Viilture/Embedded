#include "net.h"

//-----------------------------------------------

extern UART_HandleTypeDef huart6;

//-----------------------------------------------


USART_prop_ptr usartprop;
//-----------------------------------------------

void net_ini(void)

{


  usartprop.is_tcp_connect=0;

  usartprop.is_text=0;

}

//-----------------------------------------------