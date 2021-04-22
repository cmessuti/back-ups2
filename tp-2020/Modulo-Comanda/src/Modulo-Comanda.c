#include<commons/log.h>
#include<commons/config.h>
#include "Modulo-Comanda.h"



int main(void)
{
  t_log* logger = iniciar_logger();
  t_config* config = leer_config();
  int server_fd = iniciar_servidor(logger,config);
  return 0;
}

  t_log* iniciar_logger(){
  return log_create("LogDeComanda.log", "LOGDECOMANDA", 1, LOG_LEVEL_INFO);
}

  int iniciar_servidor(t_log* logger, t_config* config)
  {
  	int socket_servidor;
  	char* ip;
  	char* puerto;

      struct addrinfo hints, *servinfo, *p;

      memset(&hints, 0, sizeof(hints));
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_flags = AI_PASSIVE;

  	  ip=config_get_string_value(config,"IP");
  	  puerto=config_get_string_value(config,"PUERTO");

      getaddrinfo(ip, puerto, &hints, &servinfo);

      for (p=servinfo; p != NULL; p = p->ai_next)
      {
          if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
              continue;

          if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
              close(socket_servidor);
              continue;
          }
          break;
      }

  	listen(socket_servidor, SOMAXCONN);

      freeaddrinfo(servinfo);

     /* log_trace(logger, "Listo para escuchar a mi cliente");*/
      log_info(logger,"Listo para escuchar a mi cliente");

      return socket_servidor;
  }


  t_config* leer_config(){
  	return config_create("/home/utnso/workspace/tp-2020-2c-compumundohipermegared/Modulo-Comanda/Comanda.config");
  }
