#include<commons/log.h>
#include<commons/config.h>
#include "estructuras_cliente.h"

int main(void){

	//creo variables para la conexion de mi cliente
	int conexion;
	char* ip;
	char* puerto;
	//genero el archivo de log y lo empiezo a escribir
	t_log* logger =	iniciar_logger();
	log_info(logger,"Comienzo a escribir mi log");
	//variable para archivo de configuracion
	t_config* config = leer_config();
	//obtengo valores para conexion y las logueo
	ip=config_get_string_value(config,"IP");
	log_info(logger, ip);
	puerto=config_get_string_value(config,"PUERTO");
	log_info(logger, puerto);
	//creo la conexion
	conexion=crear_conexion(ip,puerto);
	return 0;
}






t_log* iniciar_logger(){
	return log_create("LogDeCliente.log", "LOGDECLIENTE", 1, LOG_LEVEL_INFO);
}

t_config* leer_config(){
	return config_create("/home/utnso/workspace/tp-2020-2c-compumundohipermegared/Modulo-Cliente/ClienteTest.config");
}




