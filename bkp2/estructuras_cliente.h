#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<netdb.h>
#include<string.h>


t_log* iniciar_logger(void);
t_config* leer_config(void);

int crear_conexion(char* ip, char* puerto);

