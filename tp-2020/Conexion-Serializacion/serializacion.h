/*
 * serializacionCliente.h
 *
 *  Created on: 18 sep. 2020
 *      Author: utnso
 */

#ifndef CONEXION_SERIALIZACION_SERIALIZACION_H_
#define CONEXION_SERIALIZACION_SERIALIZACION_H_

#include "utils.h"

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
}t_seleccionar_restaurante;//por ahi no es necesario mandar el cliente;

typedef struct
{
	op_code codigo_operacion;
} t_codigo;

typedef struct{
	uint32_t tamanio_nombre;
	char* nombre_restaurante;
}t_restaurante;    //tmb se usa en obtener receta, y en consultarPlatos

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
	char* nombre_restaurante;		//=estado de pedido
	uint32_t id_pedido;
	uint32_t tamanio_comida;
	char* nombre_comida;           //Se usa en respuesta_obtener_pedido, nombre_comida= listado de platos
}t_plato_listo;

typedef struct{                //sin usar
	uint32_t id_pedido;
}t_id_pedido;

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


void serializar_y_enviar(t_buffer* buffer, op_code codigo_operacion, int);
void enviar_cadena(t_restaurante, op_code, int);
void enviar_pedido_nombre(t_pedido_nombre respuesta, op_code codigoDeOperacion, int socket);

void enviar_consultar_restaurantes(int socket_cliente);


void enviar_guardar_pedido(char*, int, int);
t_pedido_nombre armar_guardar_pedido(char*, int);
void enviar_pedido(t_pedido_nombre, int);
t_pedido_nombre* recibir_guardar_pedido(int);
void responder_guardar_pedido(char* respuesta, int id, int socket); //esta es la v2
t_pedido_nombre* recibir_respuesta_guardar_pedido(int socket);


void enviar_seleccionar_restaurante(char*,int,int,char*,int);
t_cliente armar_cliente(char*, int, int);
t_seleccionar_restaurante armar_seleccionar_restaurante(t_cliente,char*);
void enviar_seleccion_restaurante(t_seleccionar_restaurante , int );
t_seleccionar_restaurante* recibir_seleccionar_restaurante(int);


void enviar_obtener_restaurante(char*, int);
t_restaurante armar_restaurante(char* nombre);
void enviar_restaurante(t_restaurante restaurante, int socket);
t_restaurante* recibir_restaurante(int socket_cliente);
void responder_obtener_restaurante(char* afinidades, int posX, int posY, char* recetas, char* precios, int cantHornos, int cantPedidos, int conexion);
t_respuesta_obtener_restaurante armar_respuesta_restaurante(char* afinidades, int posX, int posY, char* recetas, char* precios, int cantHornos, int cantPedidos);
void enviar_respuesta_restaurante(t_respuesta_obtener_restaurante respuesta, int socket);
t_respuesta_obtener_restaurante* recibir_respuesta_obtener_restaurante(int socket_cliente);


void enviar_consultar_platos(char*nombre, int socket);
void responder_consultar_platos(char* platos, int socket);
t_restaurante* recibir_respuesta_consultar_platos(int socket_cliente);
void enviar_restaurante_2(t_restaurante restaurante, int socket);


void enviar_crear_pedido(int socket_cliente);
void responder_crear_pedido(int idPedido, int socket);
int* recibir_respuesta_crear_pedido(int socket_cliente);



void enviar_aniadir_plato(char* nombre, int idPedido, int socket);
void enviar_plato_pedido(t_pedido_nombre pedido, int socket);
t_pedido_nombre* recibir_aniadir_plato(int socket_cliente);
void responder_aniadir_plato(char* respuesta, int socket);
t_restaurante* recibir_respuesta_aniadir_plato(int socket);

void responder_aniadir_platoV2(char* respuesta, int idPedido, char* plato, int socket);
t_plato_listo* recibir_respuesta_aniadir_platoV2(int socket);
void enviar_respuesta_aniadir_plato(t_plato_listo OKoFAIL, op_code codigo_de_operacion, int socket); // SE PUEDE USAR GENERAL



void enviar_guardar_plato(char* nombre_restaurante, int idPedido, char* comida, int cantidad, int socket);
t_guardar_plato armar_guardar_plato(char* nombre, int idPedido, char* comida, int cantidad);
void enviar_plato_y_cantidad(t_guardar_plato pedido, int socket);
t_guardar_plato* recibir_guardar_plato(int socket_cliente);
void responder_guardar_plato(char* respuesta, int socket); //usar la v2
t_restaurante* recibir_respuesta_guardar_plato(int socket);

void responder_guardar_platoV2(char* respuesta, int id, char* plato, int socket);
t_plato_listo* recibir_respuesta_guardar_platoV2(int socket);


void enviar_confirmar_pedido(char* nombre, int idPedido, int socket);
void enviar_confirmacion_pedido(t_pedido_nombre pedido, int socket);
t_pedido_nombre* recibir_confirmar_pedido(int socket_cliente);
void responder_confirmar_pedido(char* respuesta, int idPedido, int socket);
t_pedido_nombre* recibir_respuesta_confirmar_pedido(int socket);


void enviar_plato_listo(char* nombre_restaurante, int idPedido, char* comida, int socket);
t_plato_listo armar_plato_listo(char* nombre, int idPedido, char* comida);
void enviar_platoListo(t_plato_listo pedido, int socket);
t_plato_listo* recibir_plato_listo(int socket_cliente);
void responder_plato_listo(char* respuesta, int socket); //usar la v2
t_restaurante* recibir_respuesta_plato_listo(int socket);

void responder_plato_listoV2(char* respuesta,int id, char* plato, int socket);
t_plato_listo* recibir_respuesta_plato_listoV2(int socket);

void enviar_consultar_pedido(int idPedido, int socket);
int* recibir_consultar_pedido(int socket_cliente);


void enviar_obtener_pedido(char* nombre, int idPedido, int socket);
void enviar_obtencion_pedido(t_pedido_nombre pedido, int socket);
t_pedido_nombre* recibir_obtener_pedido(int socket_cliente);
void responder_obtener_pedido(char* estado, int id, char* platos, int socket);
t_plato_listo* recibir_respuesta_obtener_pedido(int socket);


void enviar_finalizar_pedido(char* nombre, int idPedido, int socket);
void enviar_finalizacion_pedido(t_pedido_nombre pedido, int socket);
t_pedido_nombre* recibir_finalizar_pedido(int socket_cliente);
void responder_finalizar_pedido(char* respuesta, int idPedido, int socket);
t_pedido_nombre* recibir_respuesta_finalizar_pedido(int socket);


void enviar_terminar_pedido(char* nombre, int idPedido, int socket);
void enviar_pedido_terminado(t_pedido_nombre pedido, int socket);
t_pedido_nombre* recibir_terminar_pedido(int socket_cliente);


void enviar_obtener_receta(char*, int);
void enviar_obtencion_receta(t_restaurante, int);
t_restaurante* recibir_obtener_receta(int);






//HANDSHAKE
void enviar_handshake_cliente(char* id_cliente, int posicion_x, int posicion_y, int conexion);
void enviar_handshake_generico(op_code codigo_operacion, int conexion);


#endif /* CONEXION_SERIALIZACION_SERIALIZACION_H_ */
