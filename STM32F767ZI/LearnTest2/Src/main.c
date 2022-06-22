/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip.h"
#include "lwip/tcp.h"

/* Private variables ---------------------------------------------------------*/
extern uint8_t IP_ADDRESS[4];
extern uint8_t NETMASK_ADDRESS[4];
extern uint8_t GATEWAY_ADDRESS[4];
UART_HandleTypeDef huart3;
PCD_HandleTypeDef hpcd_USB_OTG_FS;
char str1[100];
u8_t data[100];

struct tcp_pcb *client_pcb;
__IO uint32_t message_count=0;
//-----------------------------------------------

enum client_states
{ ES_NOT_CONNECTED = 0,
  ES_CONNECTED,
  ES_RECEIVED,
  ES_CLOSING,
};
//-------------------------------------------------------
//-------------------------------------------------------
struct client_struct
{
  enum client_states state; /* connection status */
  struct tcp_pcb *pcb; /* pointer on the current tcp_pcb */
  struct pbuf *p_tx; /* pointer on pbuf to be transmitted */
};
struct client_struct *es;
//-----------------------------------------------
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_client_send(struct tcp_pcb *tpcb, struct client_struct * es);
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct client_struct * es);


////////////////////////////////////////////////////////////////////////////////\
//-----------------------------------------------
//Соединение с сервером
void tcp_client_connect(void)
{ ip_addr_t DestIPaddr;
  client_pcb = tcp_new();
  if (client_pcb != NULL)
  {
    IP4_ADDR( &DestIPaddr, 192, 168,
		0, 130);
    tcp_connect(client_pcb,&DestIPaddr,5001,tcp_client_connected);
    
  }
	
/*Мы создали экземпляр структуры соединения, затем занесли в переменную IP-адрес сервера 
	и вызвали функцию соединения с сервером, в качестве параметров мы в данную функцию 
	передали экземпляр структуры, адрес сервера, порт сервера, а также callback-функцию, 
	которая будет вызываться в случае соединения с сервером.*/
}
//-----------------------------------------------
//функцию, которая будет вызываться при создании соединения с сервером, так называемая callback-функция)
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{//struct client_struct *es = NULL;
	es = NULL;
  if (err == ERR_OK)
  {es = (struct client_struct *)mem_malloc(sizeof(struct client_struct));
  if (es != NULL)
  {es->state = ES_CONNECTED;
  es->pcb = tpcb;
  es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)data) , PBUF_POOL);
		if (es->p_tx)
{/* copy data to pbuf */
  pbuf_take(es->p_tx, (char*)data, strlen((char*)data));
  /* pass newly allocated es structure as argument to tpcb */
  tcp_arg(tpcb, es);
  /* initialize LwIP tcp_recv callback function */
  tcp_recv(tpcb, tcp_client_recv);
  /* initialize LwIP tcp_sent callback function */
  tcp_sent(tpcb, tcp_client_sent);
  /* initialize LwIP tcp_poll callback function */
  tcp_poll(tpcb, tcp_client_poll, 1);
  /* send data */
  tcp_client_send(tpcb,es);
  return ERR_OK;
}
}
  else
  {
    tcp_client_connection_close(tpcb, es);
    return ERR_MEM;
  }
  }
  else
  {
    tcp_client_connection_close(tpcb, es);
  }
  return err;
}
//-----------------------------------------------
//-----------------------------------------------

static err_t tcp_client_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{ struct client_struct *es;
  err_t ret_err;
	es = (struct client_struct *)arg;
	if (p == NULL)
{es->state = ES_CLOSING;
  if(es->p_tx == NULL)
  {
    tcp_client_connection_close(tpcb, es);
  }
  ret_err = ERR_OK;
}
else if(err != ERR_OK)
{if (p != NULL)
  {
    pbuf_free(p);
  }
  ret_err = err;
}
else if(es->state == ES_CONNECTED)
{  message_count++;
  tcp_recved(tpcb, p->tot_len);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
  es->p_tx = p;
  strncpy(str1,es->p_tx->payload,es->p_tx->len);
  str1[es->p_tx->len] = '\0';
  
  ret_err = ERR_OK;
}
else if (es->state == ES_RECEIVED)
{ HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
  ret_err = ERR_OK;
}
else
{ /* Acknowledge data reception */
  tcp_recved(tpcb, p->tot_len);
  /* free pbuf and do nothing */
  pbuf_free(p);
  ret_err = ERR_OK;
}
return ret_err;
 
}
//----------------------------------------------------------

static void tcp_client_send(struct tcp_pcb *tpcb, struct client_struct * es)
{struct pbuf *ptr;
  err_t wr_err = ERR_OK;
  while ((wr_err == ERR_OK) &&
  (es->p_tx != NULL) &&
  (es->p_tx->len <= tcp_sndbuf(tpcb)))
  {ptr = es->p_tx;
  wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
		if (wr_err == ERR_OK)
{es->p_tx = ptr->next;
  if(es->p_tx != NULL)
  {
    pbuf_ref(es->p_tx);
  }
  pbuf_free(ptr);
}
else if(wr_err == ERR_MEM)
{es->p_tx = ptr;
}
else
{
/* other problem ?? */
}
  }

}
//----------------------------------------------------------
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{struct client_struct *es;
  LWIP_UNUSED_ARG(len);
  es = (struct client_struct *)arg;
  if(es->p_tx != NULL)
  {
    tcp_client_send(tpcb, es);
  }
	return ERR_OK;
}
//----------------------------------------------------------
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{err_t ret_err;
struct client_struct *es;
es = (struct client_struct*)arg;
	if (es != NULL)
{
  if (es->p_tx != NULL)
  {
  }
  else
  {
    if(es->state == ES_CLOSING)
    {
      tcp_client_connection_close(tpcb, es);
    }
  }
  ret_err = ERR_OK;
}
else
{tcp_abort(tpcb);
  ret_err = ERR_ABRT;
}
  return ret_err;
}
//----------------------------------------------------------
//-----------------------------------------------
//----------------------------------------------------------

static void tcp_client_connection_close(struct tcp_pcb *tpcb, struct client_struct *es)
{/* remove callbacks */
  tcp_recv(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_poll(tpcb, NULL,0);
  if (es != NULL)
  {mem_free(es);}
  /* close tcp connection */
  tcp_close(tpcb);
}
//----------------------------------------------------------
//-----------------------------------------------

void sendstring(char* buf_str)
{ tcp_sent(client_pcb, tcp_client_sent);
  tcp_write(client_pcb, (void*)buf_str, strlen(buf_str), 1);
  tcp_output(client_pcb);
}
//-----------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int main(void)
{  IP_ADDRESS[0] = 192;
  IP_ADDRESS[1] = 168;
  IP_ADDRESS[2] = 0;
  IP_ADDRESS[3] = 160;
  NETMASK_ADDRESS[0] = 255;
  NETMASK_ADDRESS[1] = 255;
  NETMASK_ADDRESS[2] = 255;
  NETMASK_ADDRESS[3] = 0;
  GATEWAY_ADDRESS[0] = 192;
  GATEWAY_ADDRESS[1] = 168;
  GATEWAY_ADDRESS[2] = 1;
  GATEWAY_ADDRESS[3] = 160;

  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_LWIP_Init();
  
	char send=0xFF;
  while (1)
  {//HAL_Delay(500);
		MX_LWIP_Process();
    //tcp_client_connect();
		//sendstring(&send);
		//tcp_client_connection_close(es->pcb,es);


  }
  
}
///////////////////////////////////////////////////////////////////////
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

