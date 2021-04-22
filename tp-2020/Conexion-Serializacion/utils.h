/*
 * utils.h
 *
 *  Created on: 9 sep. 2020
 *      Author: utnso
 */

#ifndef CONEXION_SERIALIZACION_UTILS_H_
#define CONEXION_SERIALIZACION_UTILS_H_


#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<pthread.h>
#include<semaphore.h>
#include<commons/string.h>

#define IP "127.0.0.1"  //ojo esto
#define PUERTO "4444"    //ojo esto
 //de proceso cliente

typedef enum
{
	MENSAJE,
	PAQUETE,
	PEDIDO,   //agregado para probar
	CONSULTAR_RESTAURANTES,
	SELECCIONAR_RESTAURANTE,
	OBTENER_RESTAURANTE,
	CONSULTAR_PLATOS,
	CREAR_PEDIDO,
	ANIADIR_PLATO,
	GUARDAR_PLATO,
	CONFIRMAR_PEDIDO,
	PLATO_LISTO,
	CONSULTAR_PEDIDO,
	OBTENER_PEDIDO,
	FINALIZAR_PEDIDO,
	TERMINAR_PEDIDO,
	OBTENER_RECETA,
	RESPUESTA_OBTENER_RESTAURANTE,
	RESPUESTA_CONSULTAR_PLATOS,
	RESPUESTA_CREAR_PEDIDO,
	RESPUESTA_ANIADIR_PLATO,
	RESPUESTA_CONFIRMAR_PEDIDO,
	RESPUESTA_FINALIZAR_PEDIDO,
	RESPUESTA_GUARDAR_PEDIDO,
	RESPUESTA_GUARDAR_PLATO,
	RESPUESTA_PLATO_LISTO,
	RESPUESTA_OBTENER_PEDIDO
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;



int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(void);
t_paquete* crear_super_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);

//----------------------------------------------------------------

t_log* logger;
void* recibir_buffer(int*, int);

int iniciar_servidor(void);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);




#endif /* CONEXION_SERIALIZACION_UTILS_H_ */
