/*
 * conexiones.c
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#include "../Servidor-tp0/utils.h"

int iniciar_servidor(void)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

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

    log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;
	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

//podemos usar la lista de valores para poder hablar del for y de como recorrer la lista
t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
	return NULL;
}

//------------prueba------------------
t_pedido_nombre* recibir_guardar_pedido(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	int tamanio;
	t_pedido_nombre* pedido = malloc(sizeof(t_pedido_nombre));

	buffer = recibir_buffer(&size, socket_cliente);

	memcpy(&(pedido->id_pedido), buffer, sizeof(uint32_t));
	buffer+=sizeof(uint32_t);

	memcpy(&(pedido->nombre_largo), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);
	pedido->nombre=malloc(pedido->nombre_largo);
	memcpy(pedido->nombre, buffer, pedido->nombre_largo);

	//printf("El nombre del pedido es: %s, y el id es: %d \n", pedido->nombre, pedido->id_pedido);

	return pedido;


}

t_seleccionar_restaurante* recibir_seleccionar_restaurante(int socket_cliente){
	int size;
	int desplazamiento=0;
	void * buffer;
	int tamanio;
	t_seleccionar_restaurante* seleccion = malloc(sizeof(t_seleccionar_restaurante));

	buffer = recibir_buffer(&size, socket_cliente);

	memcpy(&(seleccion->cliente.tamanio_id_cliente), buffer, sizeof(uint32_t));
	buffer+=sizeof(uint32_t);
	seleccion->cliente.id_cliente=malloc(seleccion->cliente.tamanio_id_cliente);
	memcpy(seleccion->cliente.id_cliente, buffer, seleccion->cliente.tamanio_id_cliente);
	buffer+=seleccion->cliente.tamanio_id_cliente;
	memcpy(&(seleccion->cliente.posicion_x), buffer, sizeof(uint32_t));
	buffer+=sizeof(uint32_t);
	memcpy(&(seleccion->cliente.posicion_y), buffer, sizeof(uint32_t));
	buffer+=sizeof(uint32_t);
	memcpy(&(seleccion->nombre_largo), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);
	seleccion->nombre_restaurante=malloc(seleccion->nombre_largo);
	memcpy(seleccion->nombre_restaurante, buffer, seleccion->nombre_largo);


	return seleccion;
}


t_restaurante* recibir_restaurante(int socket_cliente)//
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	int tamanio;
	t_restaurante* restaurante = malloc(sizeof(t_restaurante));

	buffer = recibir_buffer(&size, socket_cliente);


	memcpy(&(restaurante->tamanio_nombre), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);
	restaurante->nombre_restaurante=malloc(restaurante->tamanio_nombre);
	memcpy(restaurante->nombre_restaurante, buffer, restaurante->tamanio_nombre);


	return restaurante;
}

t_pedido_nombre* recibir_aniadir_plato(int socket_cliente)
{

	return recibir_guardar_pedido(socket_cliente);

}

t_guardar_plato* recibir_guardar_plato(int socket_cliente)//NO FUNCIONA
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	int tamanio;
	t_guardar_plato* pedido = malloc(sizeof(t_guardar_plato));

	buffer = recibir_buffer(&size, socket_cliente);

	memcpy(&(pedido->tamanio_nombre), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);
	pedido->nombre_restaurante=malloc(pedido->tamanio_nombre);
	memcpy(pedido->nombre_restaurante, buffer, pedido->tamanio_nombre);
	buffer+=pedido->tamanio_nombre;
	memcpy(&(pedido->id_pedido), buffer, sizeof(uint32_t));
	buffer+=sizeof(uint32_t);

	memcpy(&(pedido->tamanio_comida), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);
	pedido->nombre_comida=malloc(pedido->tamanio_comida);
	memcpy(pedido->nombre_comida, buffer, pedido->tamanio_comida);
	buffer+= pedido->tamanio_comida;
	memcpy(&(pedido->cantidad), buffer, sizeof(uint32_t));

	return pedido;


}

t_pedido_nombre* recibir_confirmar_pedido(int socket_cliente)//
{

	return recibir_guardar_pedido(socket_cliente);

}

t_plato_listo* recibir_plato_listo(int socket_cliente)//
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	int tamanio;
	t_plato_listo* pedido = malloc(sizeof(t_plato_listo));

	buffer = recibir_buffer(&size, socket_cliente);

	memcpy(&(pedido->tamanio_nombre), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);
	pedido->nombre_restaurante=malloc(pedido->tamanio_nombre);
	memcpy(pedido->nombre_restaurante, buffer, pedido->tamanio_nombre);
	buffer+=pedido->tamanio_nombre;
	memcpy(&(pedido->id_pedido), buffer, sizeof(uint32_t));
	buffer+=sizeof(uint32_t);

	memcpy(&(pedido->tamanio_comida), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);
	pedido->nombre_comida=malloc(pedido->tamanio_comida);
	memcpy(pedido->nombre_comida, buffer, pedido->tamanio_comida);

	return pedido;

}

int* recibir_consultar_pedido(int socket_cliente)//
{

	int size;
	int desplazamiento = 0;
	void * buffer;
	int tamanio;
	int* pedido=malloc(sizeof(uint32_t));

	buffer = recibir_buffer(&size, socket_cliente);

	memcpy(pedido, buffer, sizeof(uint32_t));

	return pedido;

}

t_pedido_nombre* recibir_obtener_pedido(int socket_cliente)//
{

	return recibir_guardar_pedido(socket_cliente);

}

t_pedido_nombre* recibir_finalizar_pedido(int socket_cliente)//
{

	return recibir_guardar_pedido(socket_cliente);

}

t_pedido_nombre* recibir_terminar_pedido(int socket_cliente)//
{

	return recibir_guardar_pedido(socket_cliente);

}

t_restaurante* recibir_obtener_receta(int socket_cliente){
	return recibir_restaurante(socket_cliente);
}

//agregado para prueba
void enviar_consultar_restaurantes(int socket_cliente){
	t_codigo* consulta = malloc(sizeof(t_codigo));

	consulta->codigo_operacion = CONSULTAR_RESTAURANTES;

	int bytes = sizeof(consulta);


	send(socket_cliente, consulta, bytes, 0);

	free(consulta);
}

t_respuesta_obtener_restaurante* recibir_respuesta_obtener_restaurante(int socket_cliente)//
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	int tamanio;
	t_respuesta_obtener_restaurante* respuesta = malloc(sizeof(t_respuesta_obtener_restaurante));

	buffer = recibir_buffer(&size, socket_cliente);


	memcpy(&(respuesta->tamanio_afinidades), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);

	respuesta->afinidades=malloc(respuesta->tamanio_afinidades);
	memcpy(respuesta->afinidades, buffer, respuesta->tamanio_afinidades);
	buffer+= respuesta->tamanio_afinidades;

	memcpy(&(respuesta->posX), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);

	memcpy(&(respuesta->posY), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);

	memcpy(&(respuesta->tamanio_recetas), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);

	respuesta->recetas=malloc(respuesta->tamanio_recetas);
	memcpy(respuesta->recetas, buffer, respuesta->tamanio_recetas);
	buffer+= respuesta->tamanio_recetas;

	memcpy(&(respuesta->tamanio_precios), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);

	respuesta->precios=malloc(respuesta->tamanio_precios);
	memcpy(respuesta->precios, buffer, respuesta->tamanio_precios);
	buffer+= respuesta->tamanio_precios;

	memcpy(&(respuesta->cant_hornos), buffer, sizeof(uint32_t));
	buffer+= sizeof(uint32_t);

	memcpy(&(respuesta->cant_pedidos), buffer, sizeof(uint32_t));
	//buffer+= sizeof(uint32_t);

	return respuesta;
}

void responder_obtener_restaurante(char* afinidades, int posX, int posY, char* recetas, char* precios, int cantHornos, int cantPedidos, int conexion){
	t_respuesta_obtener_restaurante respuesta= armar_respuesta_restaurante(afinidades, posX, posY, recetas, precios, cantHornos, cantPedidos);
	enviar_respuesta_restaurante(respuesta, conexion);
}

t_respuesta_obtener_restaurante armar_respuesta_restaurante(char* afinidades, int posX, int posY, char* recetas, char* precios, int cantHornos, int cantPedidos){
	t_respuesta_obtener_restaurante* respuesta= malloc(sizeof(t_respuesta_obtener_restaurante));
	respuesta->afinidades=malloc(strlen(afinidades)+1);

	strcpy(respuesta->afinidades,afinidades);
	respuesta->tamanio_afinidades = strlen(respuesta->afinidades)+1;
	respuesta->posX=posX;
	respuesta->posY=posY;
	respuesta->recetas=malloc(strlen(recetas)+1);
	strcpy(respuesta->recetas, recetas);
	respuesta->tamanio_recetas = strlen(respuesta->recetas)+1;
	respuesta->precios=malloc(strlen(precios)+1);
	strcpy(respuesta->precios, precios);
	respuesta->tamanio_precios = strlen(respuesta->precios)+1;
	respuesta->cant_hornos=cantHornos;
	respuesta->cant_pedidos=cantPedidos;
	return *respuesta;
}


void enviar_respuesta_restaurante(t_respuesta_obtener_restaurante respuesta, int socket){ //OBTENER RESTAURANTE
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*7
				 + strlen(respuesta.afinidades)+1 + strlen(respuesta.precios)+1 + strlen(respuesta.recetas)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &respuesta.tamanio_afinidades, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, respuesta.afinidades, strlen(respuesta.afinidades)+1);
	offset += respuesta.tamanio_afinidades;
	memcpy(stream + offset, &respuesta.posX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &respuesta.posY, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &respuesta.tamanio_recetas, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, respuesta.recetas, strlen(respuesta.recetas)+1);
	offset += respuesta.tamanio_recetas;
	memcpy(stream + offset, &respuesta.tamanio_precios, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, respuesta.precios, strlen(respuesta.precios)+1);
	offset += respuesta.tamanio_precios;
	memcpy(stream + offset, &respuesta.cant_hornos, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &respuesta.cant_pedidos, sizeof(uint32_t));
	//offset += sizeof(uint32_t);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, RESPUESTA_OBTENER_RESTAURANTE, socket);
}

void serializar_y_enviar(t_buffer* buffer, op_code codigo_operacion, int socket){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo_operacion;//desde aca es igual a todos?
	paquete->buffer = buffer;

	void* a_enviar = malloc(buffer->size + sizeof(op_code) + sizeof(uint32_t));
	int offset2= 0;

	memcpy(a_enviar + offset2, &(paquete->codigo_operacion), sizeof(op_code));
	offset2 += sizeof(op_code);
	memcpy(a_enviar + offset2, &(paquete->buffer->size), sizeof(uint32_t));
	offset2 += sizeof(uint32_t);
	memcpy(a_enviar + offset2, paquete->buffer->stream, paquete->buffer->size);

	send(socket, a_enviar, buffer->size + sizeof(op_code)+ sizeof(uint32_t), 0);
	printf("se envio correcto\n");
	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

}
