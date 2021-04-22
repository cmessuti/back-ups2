/*
 * conexiones.h
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#ifndef CONEXIONES_H_
#define CONEXIONES_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>

#define IP "127.0.0.1"
#define PUERTO "4444"

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
	RESPUESTA_PLATO_LISTO
}op_code;

typedef struct {
	uint32_t id_pedido;
	uint32_t nombre_largo;
	char* nombre;
}t_pedido_nombre;


typedef struct {
	uint32_t tamanio_id_cliente;
	char* id_cliente;
	uint32_t posicion_x;
	uint32_t posicion_y;
}t_cliente;

typedef struct {
	t_cliente cliente;
	uint32_t nombre_largo;
	char* nombre_restaurante;
}t_seleccionar_restaurante;

typedef struct{
	uint32_t tamanio_nombre;
	char* nombre_restaurante;
}t_restaurante;

typedef struct{
	uint32_t tamanio_nombre;
	char* nombre_restaurante;
	uint32_t id_pedido;
	uint32_t tamanio_comida;
	char* nombre_comida;
	uint32_t cantidad;
}t_guardar_plato;

typedef struct{                     //conviene usar t_restaurante?
	uint32_t tamanio_nombre;
	char* nombre_restaurante;
	uint32_t id_pedido;
	uint32_t tamanio_comida;
	char* nombre_comida;
}t_plato_listo;

typedef struct
{
	op_code codigo_operacion;
} t_codigo; //agregado para pruebas

typedef struct{
	uint32_t tamanio_afinidades;
	char* afinidades;
	uint32_t posX;
	uint32_t posY;
	uint32_t tamanio_recetas;
	char* recetas;
	uint32_t tamanio_precios;
	char* precios;
	uint32_t cant_hornos;
	uint32_t cant_pedidos;
}t_respuesta_obtener_restaurante;

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


t_log* logger;

void* recibir_buffer(int*, int);

int iniciar_servidor(void);
int esperar_cliente(int);
t_list* recibir_paquete(int);
void recibir_mensaje(int);
int recibir_operacion(int);

t_pedido_nombre* recibir_guardar_pedido(int socket_cliente);
t_seleccionar_restaurante* recibir_seleccionar_restaurante(int);
t_restaurante* recibir_restaurante(int socket_cliente);
t_pedido_nombre* recibir_aniadir_plato(int socket_cliente);
t_guardar_plato* recibir_guardar_plato(int socket_cliente);
t_pedido_nombre* recibir_confirmar_pedido(int socket_cliente);
t_plato_listo* recibir_plato_listo(int socket_cliente);
int* recibir_consultar_pedido(int socket_cliente);
t_pedido_nombre* recibir_obtener_pedido(int socket_cliente);
t_pedido_nombre* recibir_finalizar_pedido(int socket_cliente);
t_pedido_nombre* recibir_terminar_pedido(int socket_cliente);
t_restaurante* recibir_obtener_receta(int socket_cliente);
void enviar_consultar_restaurantes(int socket_cliente);//agregado para prueba

t_respuesta_obtener_restaurante* recibir_respuesta_obtener_restaurante(int socket_cliente);
void responder_obtener_restaurante(char* afinidades, int posX, int posY, char* recetas, char* precios, int cantHornos, int cantPedidos, int conexion);
t_respuesta_obtener_restaurante armar_respuesta_restaurante(char* afinidades, int posX, int posY, char* recetas, char* precios, int cantHornos, int cantPedidos);
void enviar_respuesta_restaurante(t_respuesta_obtener_restaurante respuesta, int socket);
void serializar_y_enviar(t_buffer* buffer, op_code codigo_operacion, int socket);
#endif /* CONEXIONES_H_ */
