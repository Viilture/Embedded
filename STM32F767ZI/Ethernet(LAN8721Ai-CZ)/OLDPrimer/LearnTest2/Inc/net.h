#ifndef NET_H_

#define NET_H_

//-----------------------------------------------

#include "stm32f7xx_hal.h"

#include <string.h>

#include <stdlib.h>

#include <stdint.h>

#include "lwip.h"

#include "lwip/tcp.h"

#include "lwip/tcp.h"

//-----------------------------------------------

typedef struct USART_prop{

uint8_t is_tcp_connect;//?????? ??????? ??????? ?????????? TCP ? ????????

uint8_t is_text;//?????? ??????? ???????? ????? ???????

} USART_prop_ptr;

//-----------------------------------------------

//-----------------------------------------------

#endif /* NET_H_ */