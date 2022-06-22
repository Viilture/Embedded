/*
  ************************************************* ************************************************* ***********
  ************************************************* ************************************************* ***********
  ************************************************* ************************************************* ***********

  Файл: tcpServerRAW.c
  Изменено: ControllersTech.com
  Обновлено: 26-июл-2021

  ************************************************* ************************************************* ***********
  Авторские права (C) 2017 ControllersTech.com

  Это бесплатное программное обеспечение под лицензией GNU, вы можете распространять и / или изменять его в соответствии с условиями
  Стандартной общественной лицензии GNU версии 3, опубликованной Free Software Foundation.
  Эта программная библиотека является общедоступной в образовательных целях без ГАРАНТИЙ, и Автор не несет ответственности за любые убытки, причиненные напрямую.
  или косвенно с помощью этого программного обеспечения, подробнее об этом читайте в Стандартной общественной лицензии GNU.

  ************************************************* ************************************************* ***********
* /


/ **
 * Copyright (c) 2001-2004 Шведский институт компьютерных наук.
 * Все права защищены.
 *
 * Распространение и использование в исходной и двоичной формах, с модификациями или без них,
 * разрешены при соблюдении следующих условий:
 *
 * 1. При повторном распространении исходного кода должно сохраняться указанное выше уведомление об авторских правах,
 * этот список условий и следующий отказ от ответственности.
 * 2. При повторном распространении в двоичной форме должно воспроизводиться указанное выше уведомление об авторских правах,
 * этот список условий и следующий отказ от ответственности в документации
 * и / или другие материалы, поставляемые с распространением.
 * 3. Имя автора не может использоваться для поддержки или продвижения продуктов.
 * получено из этого программного обеспечения без специального предварительного письменного разрешения.
 *
 * ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ АВТОРОМ `` КАК ЕСТЬ '' И ЛЮБОЙ ЯВНОЙ ИЛИ ПОДРАЗУМЕВАЕМЫЙ
 * ГАРАНТИИ, ВКЛЮЧАЯ, НО НЕ ОГРАНИЧИВАясь, ПОДРАЗУМЕВАЕМЫЕ ГАРАНТИИ
 * КОММЕРЧЕСКАЯ ЦЕННОСТЬ И ПРИГОДНОСТЬ ДЛЯ ОПРЕДЕЛЕННОЙ ЦЕЛИ ОТКАЗЫВАЕТСЯ. НИ В КОЕМ СЛУЧАЕ
 * АВТОР НЕСЕТ ОТВЕТСТВЕННОСТЬ ЗА ЛЮБЫЕ ПРЯМЫЕ, КОСВЕННЫЕ, СЛУЧАЙНЫЕ, ОСОБЫЕ,
 * ОБЯЗАТЕЛЬНЫЕ ИЛИ КОСВЕННЫЕ УБЫТКИ (ВКЛЮЧАЯ, НО НЕ ОГРАНИЧИВАЯСЬ, ЗАКУПКИ
 * ЗАМЕНЫ ТОВАРОВ ИЛИ УСЛУГ; ПОТЕРЯ ИСПОЛЬЗОВАНИЯ, ДАННЫХ ИЛИ ПРИБЫЛИ; ИЛИ БИЗНЕС
 * ПРЕРЫВАНИЕ), ОДНАКО ВЫЗВАННЫМ И ПО ЛЮБОЙ ТЕОРИИ ОТВЕТСТВЕННОСТИ, ЛИБО В
 * ВОЗНИКАЮЩИЙ ДОГОВОР, СТРОГОЙ ОТВЕТСТВЕННОСТЬ ИЛИ ИСКУССТВО (ВКЛЮЧАЯ НЕБРЕЖНОСТЬ ИЛИ ИНОЕ)
 * В ЛЮБОМ СЛУЧАЕ ОТ ИСПОЛЬЗОВАНИЯ ДАННОГО ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ, ДАЖЕ В СЛУЧАЕ ПРЕДУПРЕЖДЕНИЯ О ВОЗМОЖНОСТИ
 * ТАКОГО ПОВРЕЖДЕНИЯ.
 *
 * Этот файл является частью и дополнением стека lwIP TCP / IP.
 *
 * Кредиты принадлежат Адаму Данкелсу (и текущим сопровождающим) этого программного обеспечения.
 *
 * Christiaan Simons переписал этот файл, чтобы получить более стабильное приложение.
 *
 ** /

 / * Этот файл был изменен ST * /

#include "tcpserverRAW.h"

#include "lwip/tcp.h"



5 000 / 5 000
Результаты перевода
/ *
  ************************************************* ************************************************* ***********
  ************************************************* ************************************************* ***********
  ************************************************* ************************************************* ***********

  Файл: tcpServerRAW.c
  Изменено: ControllersTech.com
  Обновлено: 26-июл-2021

  ************************************************* ************************************************* ***********
  Авторские права (C) 2017 ControllersTech.com

  Это бесплатное программное обеспечение под лицензией GNU, вы можете распространять и / или изменять его в соответствии с условиями
  Стандартной общественной лицензии GNU версии 3, опубликованной Free Software Foundation.
  Эта программная библиотека является общедоступной в образовательных целях без ГАРАНТИЙ, и Автор не несет ответственности за любые убытки, причиненные напрямую.
  или косвенно с помощью этого программного обеспечения, подробнее об этом читайте в Стандартной общественной лицензии GNU.

  ************************************************* ************************************************* ***********
* /


/ **
 * Copyright (c) 2001-2004 Шведский институт компьютерных наук.
 * Все права защищены.
 *
 * Распространение и использование в исходной и двоичной формах, с модификациями или без них,
 * разрешены при соблюдении следующих условий:
 *
 * 1. При повторном распространении исходного кода должно сохраняться указанное выше уведомление об авторских правах,
 * этот список условий и следующий отказ от ответственности.
 * 2. При повторном распространении в двоичной форме должно воспроизводиться указанное выше уведомление об авторских правах,
 * этот список условий и следующий отказ от ответственности в документации
 * и / или другие материалы, поставляемые с распространением.
 * 3. Имя автора не может использоваться для поддержки или продвижения продуктов.
 * получено из этого программного обеспечения без специального предварительного письменного разрешения.
 *
 * ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ АВТОРОМ `` КАК ЕСТЬ '' И ЛЮБОЙ ЯВНОЙ ИЛИ ПОДРАЗУМЕВАЕМЫЙ
 * ГАРАНТИИ, ВКЛЮЧАЯ, НО НЕ ОГРАНИЧИВАясь, ПОДРАЗУМЕВАЕМЫЕ ГАРАНТИИ
 * КОММЕРЧЕСКАЯ ЦЕННОСТЬ И ПРИГОДНОСТЬ ДЛЯ ОПРЕДЕЛЕННОЙ ЦЕЛИ ОТКАЗЫВАЕТСЯ. НИ В КОЕМ СЛУЧАЕ
 * АВТОР НЕСЕТ ОТВЕТСТВЕННОСТЬ ЗА ЛЮБЫЕ ПРЯМЫЕ, КОСВЕННЫЕ, СЛУЧАЙНЫЕ, ОСОБЫЕ,
 * ОБЯЗАТЕЛЬНЫЕ ИЛИ КОСВЕННЫЕ УБЫТКИ (ВКЛЮЧАЯ, НО НЕ ОГРАНИЧИВАЯСЬ, ЗАКУПКИ
 * ЗАМЕНЫ ТОВАРОВ ИЛИ УСЛУГ; ПОТЕРЯ ИСПОЛЬЗОВАНИЯ, ДАННЫХ ИЛИ ПРИБЫЛИ; ИЛИ БИЗНЕС
 * ПРЕРЫВАНИЕ), ОДНАКО ВЫЗВАННЫМ И ПО ЛЮБОЙ ТЕОРИИ ОТВЕТСТВЕННОСТИ, ЛИБО В
 * ВОЗНИКАЮЩИЙ ДОГОВОР, СТРОГОЙ ОТВЕТСТВЕННОСТЬ ИЛИ ИСКУССТВО (ВКЛЮЧАЯ НЕБРЕЖНОСТЬ ИЛИ ИНОЕ)
 * В ЛЮБОМ СЛУЧАЕ ОТ ИСПОЛЬЗОВАНИЯ ДАННОГО ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ, ДАЖЕ В СЛУЧАЕ ПРЕДУПРЕЖДЕНИЯ О ВОЗМОЖНОСТИ
 * ТАКОГО ПОВРЕЖДЕНИЯ.
 *
 * Этот файл является частью и дополнением стека lwIP TCP / IP.
 *
 * Кредиты принадлежат Адаму Данкелсу (и текущим сопровождающим) этого программного обеспечения.
 *
 * Christiaan Simons переписал этот файл, чтобы получить более стабильное приложение.
 *
 */

 /* Этот файл был изменен ST */

#include "tcpserverRAW.h"

#include "lwip/tcp.h"


/* состояния протокола */
enum tcp_server_states
{
  ES_NONE = 0,
  ES_ACCEPTED,
  ES_RECEIVED,
  ES_CLOSING
};

 /* структура для поддержки информации о соединении, передаваемой в качестве аргумента
    в обратные вызовы LwIP */
struct tcp_server_struct
{
  u8_t state;             /* текущее состояние подключения */
  u8_t retries;
  struct tcp_pcb *pcb;    /* указатель на текущий tcp_pcb */
  struct pbuf *p;         /* указатель на полученный / передаваемый pbuf */
};


static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_server_error(void *arg, err_t err);
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es);

static void tcp_server_handle (struct tcp_pcb *tpcb, struct tcp_server_struct *es);


/* Реализация для TCP-сервера
   1. Создайте блок TCP.
   2. Привяжите блок к адресу сервера и порту.
   3. Слушайте входящие запросы от клиента.
   4. Примите запрос, и теперь сервер готов к передаче данных.
 */
void tcp_server_init(void)
{
	/* 1. создать новую плату tcp */
	struct tcp_pcb *tpcb;

	tpcb = tcp_new();

	err_t err;

	/* 2. привязать _pcb к порту 7 (протокол) */
	ip_addr_t myIPADDR;
	IP_ADDR4(&myIPADDR, 192, 168, 0, 111);
	err = tcp_bind(tpcb, &myIPADDR, 7);

	if (err == ERR_OK)
	{
		/* 3. запускаем tcp прослушивание _pcb */
		tpcb = tcp_listen(tpcb);

		/* 4. инициализировать функцию обратного вызова LwIP tcp_accept */
		tcp_accept(tpcb, tcp_server_accept);
	}
	else
	{
		/* освобождаем плату */
		memp_free(MEMP_TCP_PCB, tpcb);
	}
}

/**
  * @brief  This function is the implementation of tcp_accept LwIP callback
  * @param  arg: not used
  * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
  * @param  err: not used
  * @retval err_t: error status
  */
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  err_t ret_err;
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  /* устанавливаем приоритет для вновь принятого TCP-соединения newpcb */
  tcp_setprio(newpcb, TCP_PRIO_MIN);

  /* выделяем структуры для поддержки информации о TCP-соединении */
  es = (struct tcp_server_struct *)mem_malloc(sizeof(struct tcp_server_struct));
  if (es != NULL)
  {
    es->state = ES_ACCEPTED;
    es->pcb = newpcb;
    es->retries = 0;
    es->p = NULL;

    /* передать вновь выделенную структуру es в качестве аргумента в newpcb */
    tcp_arg(newpcb, es);

    /* инициализировать функцию обратного вызова lwip tcp_recv для newpcb */
    tcp_recv(newpcb, tcp_server_recv);

    /* инициализировать функцию обратного вызова lwip tcp_err для newpcb */
    tcp_err(newpcb, tcp_server_error);

    /* инициализировать функцию обратного вызова lwip tcp_poll для newpcb */
    tcp_poll(newpcb, tcp_server_poll, 0);

    ret_err = ERR_OK;
  }
  else
  {
    /*  close tcp connection */
    tcp_server_connection_close(newpcb, es);
    /* return memory error */
    ret_err = ERR_MEM;
  }
  return ret_err;
}


/**
  * @brief  This function is the implementation for tcp_recv LwIP callback
  * @param  arg: pointer on a argument for the tcp_pcb connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  pbuf: pointer on the received pbuf
  * @param  err: error information regarding the reveived pbuf
  * @retval err_t: error code
  */
static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct tcp_server_struct *es;
  err_t ret_err;

  LWIP_ASSERT("arg != NULL",arg != NULL);

  es = (struct tcp_server_struct *)arg;

  /* если мы получили пустой tcp-фрейм от клиента => закрыть соединение */
  /* if we receive an empty tcp frame from client => close connection */
  if (p == NULL)
  {
	/* удаленное соединение закрыто */
    /* remote host closed connection */
    es->state = ES_CLOSING;
    if(es->p == NULL)
    {  /* мы закончили отправку, закрываем соединение */
       /* we're done sending, close connection */
       tcp_server_connection_close(tpcb, es);
    }
    else
    {
      /* мы еще не закончили */
      /* подтверждаем полученный пакет */
      /* we're not done yet */
      /* acknowledge received packet */
      tcp_sent(tpcb, tcp_server_sent);

      /* отправляем оставшиеся данные */
      /* send remaining data*/
      tcp_server_send(tpcb, es);
    }
    ret_err = ERR_OK;
  }
  /* else: от клиента получен непустой фрейм, но по какой-то причине err! = ERR_OK */
  /* else : a non empty frame was received from client but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
	/* бесплатно получено pbuf */
    /* free received pbuf*/
    if (p != NULL)
    {
      es->p = NULL;
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if(es->state == ES_ACCEPTED)
  {
 /* первый фрагмент данных в p-> payload */
    /* first data chunk in p->payload */
    es->state = ES_RECEIVED;
    /* сохраняем ссылку на входящий pbuf (цепочка) */
    /* store reference to incoming pbuf (chain) */
    es->p = p;
    /* инициализируем функцию обратного вызова LwIP tcp_sent */
    /* initialize LwIP tcp_sent callback function */
    tcp_sent(tpcb, tcp_server_sent);
    /* обрабатываем полученные данные */
    /* handle the received data */
    tcp_server_handle(tpcb, es);

    ret_err = ERR_OK;
  }
  else if (es->state == ES_RECEIVED)
  {/* от клиента получено больше данных, и предыдущие данные уже отправлены */
    /* more data received from client and previous data has been already sent*/
    if(es->p == NULL)
    {
      es->p = p;
      /* обрабатываем полученные данные */
      /* handle the received data */
      tcp_server_handle(tpcb, es);
    }
    else
    {
      struct pbuf *ptr;
      /* цепочка pbufs до конца того, что мы получили ранее */
      /* chain pbufs to the end of what we recv'ed previously  */
      ptr = es->p;
      pbuf_chain(ptr,p);
    }
    ret_err = ERR_OK;
  }
  else if(es->state == ES_CLOSING)
  {/* нечетный случай, удаленное закрытие дважды, данные мусора */
    /* odd case, remote side closing twice, trash data */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  else
  {/* неизвестно es-> состояние, данные корзины */
    /* unknown es->state, trash data  */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_err callback function (called
  *         when a fatal tcp_connection error occurs.
  * @param  arg: pointer on argument parameter
  * @param  err: not used
  * @retval None
  */
static void tcp_server_error(void *arg, err_t err)
{
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(err);

  es = (struct tcp_server_struct *)arg;
  if (es != NULL)
  {
    /*  free es structure */
    mem_free(es);
  }
}

/**
  * @brief  This function implements the tcp_poll LwIP callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
  * @retval err_t: error code
  */
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct tcp_server_struct *es;

  es = (struct tcp_server_struct *)arg;
  if (es != NULL)
  {
    if (es->p != NULL)
    {
      tcp_sent(tpcb, tcp_server_sent);
      /* все еще есть pbufs для отправки */
      /* there is a remaining pbuf (chain) , try to send data */
      tcp_server_send(tpcb, es);
    }
    else
    {
    	/* если больше нет данных для отправки и клиент закрыл соединение */
      /* no remaining pbuf (chain)  */
      if(es->state == ES_CLOSING)
      {
        /*  close tcp connection */
        tcp_server_connection_close(tpcb, es);
      }
    }
    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data)
  * @param  None
  * @retval None
  */
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(len);

  es = (struct tcp_server_struct *)arg;
  es->retries = 0;

  if(es->p != NULL)
  {
    /* still got pbufs to send */
    tcp_sent(tpcb, tcp_server_sent);
    tcp_server_send(tpcb, es);
  }
  else
  {
    /* if no more data to send and client closed connection*/
    if(es->state == ES_CLOSING)
      tcp_server_connection_close(tpcb, es);
  }
  return ERR_OK;
}


/**
  * @brief  This function is used to send data for tcp connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  es: pointer on _state structure
  * @retval None
  */
static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;

  while ((wr_err == ERR_OK) &&
         (es->p != NULL) &&
         (es->p->len <= tcp_sndbuf(tpcb)))
  {

    /* get pointer on pbuf from es structure */
    ptr = es->p;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

    if (wr_err == ERR_OK)
    {
      u16_t plen;
      u8_t freed;

      plen = ptr->len;

      /* continue with next pbuf in chain (if any) */
      es->p = ptr->next;

      if(es->p != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(es->p);
      }

     /* chop first pbuf from chain */
      do
      {
        /* try hard to free pbuf */
        freed = pbuf_free(ptr);
      }
      while(freed == 0);
     /* we can read more data now */
     tcp_recved(tpcb, plen);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later / harder, defer to poll */
     es->p = ptr;
   }
   else
   {
     /* other problem ?? */
   }
  }
}

/**
  * @brief  This functions closes the tcp connection
  * @param  tcp_pcb: pointer on the tcp connection
  * @param  es: pointer on _state structure
  * @retval None
  */
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{

  /* remove all callbacks */
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);

  /* delete es structure */
  if (es != NULL)
  {
    mem_free(es);
  }

  /* close tcp connection */
  tcp_close(tpcb);
}

/* Handle the incoming TCP Data */

static void tcp_server_handle (struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	struct tcp_server_struct *esTx = 0;

	/* get the Remote IP */
	ip4_addr_t inIP = tpcb->remote_ip;
	uint16_t inPort = tpcb->remote_port;

	/* Extract the IP */
	char *remIP = ipaddr_ntoa(&inIP);

	esTx->state = es->state;
	esTx->pcb = es->pcb;
	esTx->p = es->p;

	char buf[100];
	memset (buf, '\0', 100);

	strncpy(buf, (char *)es->p->payload, es->p->tot_len);
	strcat (buf, "+ Hello from TCP SERVER\n");


	esTx->p->payload = (void *)buf;
	esTx->p->tot_len = (es->p->tot_len - es->p->len) + strlen (buf);
	esTx->p->len = strlen (buf);

	tcp_server_send(tpcb, esTx);

	pbuf_free(es->p);

}
