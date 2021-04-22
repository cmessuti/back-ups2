/*
 * serializacionCliente.c
 *
 *  Created on: 18 sep. 2020
 *      Author: utnso
 */
#include "utils.h"
#include "serializacion.h"

//-----------------------GENERAL--------------------------
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

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_cadena(t_restaurante restaurante, op_code codigoDeOperacion, int socket){ //SE USA EN CASOS DONDE SE ENVIA SOLO UNA CADENA
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)
				 + strlen(restaurante.nombre_restaurante)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &restaurante.tamanio_nombre, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, restaurante.nombre_restaurante, strlen(restaurante.nombre_restaurante)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, codigoDeOperacion, socket);
}

void enviar_pedido_nombre(t_pedido_nombre respuesta, op_code codigoDeOperacion, int socket){ //SE USA EN CASOS DONDE SE ENVIA SOLO UNA CADENA
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*2
				 + strlen(respuesta.nombre)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &respuesta.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &respuesta.nombre_largo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, respuesta.nombre, strlen(respuesta.nombre)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, codigoDeOperacion, socket);
}

//----------GuardarPedido------------------
void enviar_guardar_pedido(char* nombre, int idPedido, int socket){
	t_pedido_nombre a_enviar= armar_guardar_pedido(nombre, idPedido);
	enviar_pedido(a_enviar, socket);
}

t_pedido_nombre armar_guardar_pedido(char* nombre, int idPedido){
	t_pedido_nombre* pedido=malloc(sizeof(t_pedido_nombre));
	pedido->nombre = malloc(strlen(nombre)+1);

	strcpy(pedido->nombre, nombre);
	pedido->nombre_largo = strlen(pedido->nombre)+1;
	pedido->id_pedido = idPedido;
	return *pedido;
}

void enviar_pedido(t_pedido_nombre pedido, int socket){//SE PUEDE MEJORAR
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*2
				 + strlen(pedido.nombre)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pedido.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &pedido.nombre_largo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre, strlen(pedido.nombre)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, PEDIDO, socket);

}

t_pedido_nombre* recibir_guardar_pedido(int socket_cliente)//
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


	return pedido;


}

void responder_guardar_pedido(char* respuesta, int id, int socket){
	t_pedido_nombre OKoFAIL= armar_guardar_pedido(respuesta, id);
	enviar_pedido_nombre(OKoFAIL, RESPUESTA_GUARDAR_PEDIDO, socket);
}

t_pedido_nombre* recibir_respuesta_guardar_pedido(int socket){
	return recibir_guardar_pedido(socket);
}




//---------SeleccionarRestaurante----------------
void enviar_seleccionar_restaurante(char* id,int posicion_x,int posicion_y,char* nombre,int socket){
	t_cliente cliente= armar_cliente(id, posicion_x, posicion_y);
	t_seleccionar_restaurante a_enviar = armar_seleccionar_restaurante(cliente, nombre);
	enviar_seleccion_restaurante(a_enviar, socket);

}

t_cliente armar_cliente(char* id, int posicion_x, int posicion_y){
	t_cliente* cliente = malloc(sizeof(t_cliente));
	cliente->tamanio_id_cliente=strlen(id)+1;

	cliente->id_cliente=malloc(strlen(id)+1);
	strcpy(cliente->id_cliente, id);
	cliente->posicion_x=posicion_x;
	cliente->posicion_y=posicion_y;

	return *cliente;
}

t_seleccionar_restaurante armar_seleccionar_restaurante(t_cliente cliente, char* nombre){
	t_seleccionar_restaurante* seleccion= malloc(sizeof(t_seleccionar_restaurante));
	seleccion->cliente=cliente;
	seleccion->nombre_restaurante = malloc(strlen(nombre)+1);

	strcpy(seleccion->nombre_restaurante, nombre);
	seleccion->nombre_largo = strlen(seleccion->nombre_restaurante)+1;

	return *seleccion;
}

void enviar_seleccion_restaurante(t_seleccionar_restaurante seleccion, int socket){
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*4
				 + strlen(seleccion.nombre_restaurante)+1+strlen(seleccion.cliente.id_cliente)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &seleccion.cliente.tamanio_id_cliente, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, seleccion.cliente.id_cliente, strlen(seleccion.cliente.id_cliente)+1);
	offset += seleccion.cliente.tamanio_id_cliente;
	memcpy(stream + offset, &seleccion.cliente.posicion_x, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &seleccion.cliente.posicion_y, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &seleccion.nombre_largo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, seleccion.nombre_restaurante, strlen(seleccion.nombre_restaurante)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE, Y SE ENVIA
	serializar_y_enviar(buffer, SELECCIONAR_RESTAURANTE, socket);
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


//------------------------ConsultarRestaurantes-----------------

void enviar_consultar_restaurantes(int socket_cliente){
	t_codigo* consulta = malloc(sizeof(t_codigo));

	consulta->codigo_operacion = CONSULTAR_RESTAURANTES;

	int bytes = sizeof(consulta);


	send(socket_cliente, consulta, bytes, 0);

	free(consulta);
}



//-----------------------ObtenerRestaurante-------------------------

void enviar_obtener_restaurante(char* nombre, int socket){
	t_restaurante restaurante= armar_restaurante(nombre);
	enviar_restaurante(restaurante, socket);
}

t_restaurante armar_restaurante(char* nombre){
	t_restaurante* restaurante= malloc(sizeof(t_restaurante));
	restaurante->nombre_restaurante=malloc(strlen(nombre)+1);

	strcpy(restaurante->nombre_restaurante,nombre);
	restaurante->tamanio_nombre = strlen(restaurante->nombre_restaurante)+1;
	return *restaurante;
}


void enviar_restaurante(t_restaurante restaurante, int socket){ //OBTENER RESTAURANTE
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)
				 + strlen(restaurante.nombre_restaurante)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &restaurante.tamanio_nombre, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, restaurante.nombre_restaurante, strlen(restaurante.nombre_restaurante)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, OBTENER_RESTAURANTE, socket);
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

void responder_obtener_restaurante(char* afinidades, int posX, int posY, char* recetas, char* precios, int cantHornos, int cantPedidos, int conexion){
	t_respuesta_obtener_restaurante respuesta= armar_respuesta_restaurante(afinidades, posX, posY, recetas, precios, cantHornos, cantPedidos);
	enviar_respuesta_restaurante(respuesta, conexion);
}

t_respuesta_obtener_restaurante armar_respuesta_restaurante(char* afinidades, int posX, int posY, char* recetas, char* precios, int cantHornos, int cantPedidos){
	t_respuesta_obtener_restaurante* respuesta= malloc(sizeof(t_restaurante));
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


//--------------ConsultarPlatos--------------------
void enviar_consultar_platos(char*nombre, int socket){
	t_restaurante restaurante= armar_restaurante(nombre);
	enviar_restaurante_2(restaurante, socket);
}

void enviar_restaurante_2(t_restaurante restaurante, int socket){ //CONSULTAR PLATO
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)
				 + strlen(restaurante.nombre_restaurante)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &restaurante.tamanio_nombre, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, restaurante.nombre_restaurante, strlen(restaurante.nombre_restaurante)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, CONSULTAR_PLATOS, socket);
}
// para recibir se usa recibir_restaurante.

void responder_consultar_platos(char* platos, int socket){//PROBAR LA RESPUESTA
	t_restaurante lista_platos= armar_restaurante(platos);
	enviar_cadena(lista_platos, RESPUESTA_CONSULTAR_PLATOS, socket);
}



t_restaurante* recibir_respuesta_consultar_platos(int socket_cliente){
	return recibir_restaurante(socket_cliente);
}





//--------------------CrearPedido------------------FUNCIONA
void enviar_crear_pedido(int socket_cliente){ //usa estructuras de consultar_restaurante
	t_codigo* consulta = malloc(sizeof(t_codigo));

	consulta->codigo_operacion = CREAR_PEDIDO;

	int bytes = sizeof(consulta);


	send(socket_cliente, consulta, bytes, 0);

	free(consulta);
}

void responder_crear_pedido(int idPedido, int socket){
	//t_pedido_nombre a_enviar= armar_guardar_pedido(nombre, idPedido);//usa la funcion de guardar pedido
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*2;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &idPedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, RESPUESTA_CREAR_PEDIDO, socket);
}



int* recibir_respuesta_crear_pedido(int socket_cliente)//
{
	return recibir_consultar_pedido(socket_cliente);
}


//------------------AñadirPlato----------------------------------FUNCIONA

void enviar_aniadir_plato(char* nombre, int idPedido, int socket){//USO ESTRUCTURAS DE GUARDAR pedido
	t_pedido_nombre a_enviar= armar_guardar_pedido(nombre, idPedido);
	enviar_plato_pedido(a_enviar, socket);
}

void enviar_plato_pedido(t_pedido_nombre pedido, int socket){
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*2
				 + strlen(pedido.nombre)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pedido.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &pedido.nombre_largo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre, strlen(pedido.nombre)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, ANIADIR_PLATO, socket);

}

t_pedido_nombre* recibir_aniadir_plato(int socket_cliente)//PROBAR ESTE
{
	return recibir_guardar_pedido(socket_cliente);

}

void responder_aniadir_plato(char* respuesta, int socket){
	t_restaurante OKoFAIL= armar_restaurante(respuesta);
	enviar_cadena(OKoFAIL, RESPUESTA_ANIADIR_PLATO, socket);
}

t_restaurante* recibir_respuesta_aniadir_plato(int socket){
	return recibir_restaurante(socket);
}


void responder_aniadir_platoV2(char* respuesta, int id_pedido, char* plato, int socket){
	t_plato_listo OKoFAIL= armar_plato_listo(respuesta, id_pedido, plato);
	enviar_respuesta_aniadir_plato(OKoFAIL, RESPUESTA_ANIADIR_PLATO, socket);
}

t_plato_listo* recibir_respuesta_aniadir_platoV2(int socket){
	return recibir_plato_listo(socket);
}

void enviar_respuesta_aniadir_plato(t_plato_listo OKoFAIL, op_code codigo_de_operacion, int socket){
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*3
				 + strlen(OKoFAIL.nombre_restaurante)+1 + strlen(OKoFAIL.nombre_comida)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &OKoFAIL.tamanio_nombre, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, OKoFAIL.nombre_restaurante, strlen(OKoFAIL.nombre_restaurante)+1);
	offset += OKoFAIL.tamanio_nombre;
	memcpy(stream + offset, &OKoFAIL.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &OKoFAIL.tamanio_comida, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, OKoFAIL.nombre_comida, strlen(OKoFAIL.nombre_comida)+1);


	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, codigo_de_operacion, socket);
}

//----------------------------GuardarPlato------------

void enviar_guardar_plato(char* nombre_restaurante, int idPedido, char* comida, int cantidad, int socket){
	t_guardar_plato a_enviar= armar_guardar_plato(nombre_restaurante, idPedido, comida, cantidad);
	enviar_plato_y_cantidad(a_enviar, socket);
}

t_guardar_plato armar_guardar_plato(char* nombre, int idPedido, char* comida, int cantidad){
	t_guardar_plato* pedido=malloc(sizeof(t_guardar_plato));
	pedido->nombre_restaurante = malloc(strlen(nombre)+1);

	strcpy(pedido->nombre_restaurante, nombre);
	pedido->tamanio_nombre = strlen(pedido->nombre_restaurante)+1;
	pedido->id_pedido = idPedido;

	pedido->nombre_comida=malloc(strlen(comida)+1);
	strcpy(pedido->nombre_comida, comida);
	pedido->tamanio_comida = strlen(pedido->nombre_comida)+1;
	pedido->cantidad = cantidad;
	return *pedido;
}

void enviar_plato_y_cantidad(t_guardar_plato pedido, int socket){
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*4
				 + strlen(pedido.nombre_restaurante)+1 + strlen(pedido.nombre_comida)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pedido.tamanio_nombre, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre_restaurante, strlen(pedido.nombre_restaurante)+1);
	offset += pedido.tamanio_nombre;
	memcpy(stream + offset, &pedido.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &pedido.tamanio_comida, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre_comida, strlen(pedido.nombre_comida)+1);
	offset += pedido.tamanio_comida;
	memcpy(stream + offset, &pedido.cantidad, sizeof(uint32_t));


	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, GUARDAR_PLATO, socket);

}

t_guardar_plato* recibir_guardar_plato(int socket_cliente)//
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

void responder_guardar_plato(char* respuesta, int socket){
	t_restaurante OKoFAIL= armar_restaurante(respuesta);
	enviar_cadena(OKoFAIL, RESPUESTA_GUARDAR_PLATO, socket);
}

t_restaurante* recibir_respuesta_guardar_plato(int socket){
	return recibir_restaurante(socket);
}

void responder_guardar_platoV2(char* respuesta, int id, char*comida, int socket){
	t_plato_listo OKoFAIL= armar_plato_listo(respuesta, id, comida);
	enviar_respuesta_aniadir_plato(OKoFAIL, RESPUESTA_GUARDAR_PLATO, socket);
}

t_plato_listo* recibir_respuesta_guardar_platoV2(int socket){
	return recibir_plato_listo(socket);
}


//----------------------CONFIRMAR PEDIDO (CASO DE COMANDA Y SINDICATO), HACE FALTA DARLO VUELTA?
void enviar_confirmar_pedido(char* nombre, int idPedido, int socket){
	t_pedido_nombre a_enviar= armar_guardar_pedido(nombre, idPedido);//usa la funcion de guardar pedido
	enviar_confirmacion_pedido(a_enviar, socket);
}


void enviar_confirmacion_pedido(t_pedido_nombre pedido, int socket){
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*2
				 + strlen(pedido.nombre)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pedido.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &pedido.nombre_largo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre, strlen(pedido.nombre)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, CONFIRMAR_PEDIDO, socket);

}

t_pedido_nombre* recibir_confirmar_pedido(int socket_cliente)//
{

	return recibir_guardar_pedido(socket_cliente);

}

void responder_confirmar_pedido(char* respuesta, int id, int socket){
	t_pedido_nombre OKoFAIL= armar_guardar_pedido(respuesta, id);
	enviar_pedido_nombre(OKoFAIL, RESPUESTA_CONFIRMAR_PEDIDO, socket);
}

t_pedido_nombre* recibir_respuesta_confirmar_pedido(int socket){
	return recibir_guardar_pedido(socket);
}


//PLATO LISTO-------------------------------------------------------------------
void enviar_plato_listo(char* nombre_restaurante, int idPedido, char* comida, int socket){
	t_plato_listo a_enviar= armar_plato_listo(nombre_restaurante, idPedido, comida);
	enviar_platoListo(a_enviar, socket);
}

t_plato_listo armar_plato_listo(char* nombre, int idPedido, char* comida){
	t_plato_listo* plato_listo=malloc(sizeof(t_plato_listo));
	plato_listo->nombre_restaurante = malloc(strlen(nombre)+1);

	strcpy(plato_listo->nombre_restaurante, nombre);
	plato_listo->tamanio_nombre = strlen(plato_listo->nombre_restaurante)+1;
	plato_listo->id_pedido = idPedido;

	plato_listo->nombre_comida=malloc(strlen(comida)+1);
	strcpy(plato_listo->nombre_comida, comida);
	plato_listo->tamanio_comida = strlen(plato_listo->nombre_comida)+1;

	return *plato_listo;
}

void enviar_platoListo(t_plato_listo pedido, int socket){
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*3
				 + strlen(pedido.nombre_restaurante)+1 + strlen(pedido.nombre_comida)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pedido.tamanio_nombre, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre_restaurante, strlen(pedido.nombre_restaurante)+1);
	offset += pedido.tamanio_nombre;
	memcpy(stream + offset, &pedido.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &pedido.tamanio_comida, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre_comida, strlen(pedido.nombre_comida)+1);


	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, PLATO_LISTO, socket);

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

void responder_plato_listo(char* respuesta, int socket){
	t_restaurante OKoFAIL= armar_restaurante(respuesta);
	enviar_cadena(OKoFAIL, RESPUESTA_PLATO_LISTO, socket);
}

t_restaurante* recibir_respuesta_plato_listo(int socket){
	return recibir_restaurante(socket);
}


//CONSULTAR PEDIDO-- OJO CON ESTE, CAMBIAR?
void enviar_consultar_pedido(int idPedido, int socket){
	//t_pedido_nombre a_enviar= armar_guardar_pedido(nombre, idPedido);//usa la funcion de guardar pedido
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*2;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &idPedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, CONSULTAR_PEDIDO, socket);
}



int* recibir_consultar_pedido(int socket_cliente)//
{

	int size;
	int desplazamiento = 0;
	void * buffer;
	int tamanio;
	int* pedido=malloc(sizeof(int));

	buffer = recibir_buffer(&size, socket_cliente);

	memcpy(pedido, buffer, sizeof(uint32_t));

	return pedido;

}


//OBTENER PEDIDO
void enviar_obtener_pedido(char* nombre, int idPedido, int socket){
	t_pedido_nombre a_enviar= armar_guardar_pedido(nombre, idPedido);//usa la funcion de guardar pedido
	enviar_obtencion_pedido(a_enviar, socket);
}



void enviar_obtencion_pedido(t_pedido_nombre pedido, int socket){
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*2
				 + strlen(pedido.nombre)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pedido.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &pedido.nombre_largo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre, strlen(pedido.nombre)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, OBTENER_PEDIDO, socket);

}

t_pedido_nombre* recibir_obtener_pedido(int socket_cliente)//
{

	return recibir_guardar_pedido(socket_cliente);

}

void responder_obtener_pedido(char* estado, int id, char*comidas, int socket){
	t_plato_listo OKoFAIL= armar_plato_listo(estado, id, comidas);
	enviar_respuesta_aniadir_plato(OKoFAIL, RESPUESTA_OBTENER_PEDIDO, socket);
}

t_plato_listo* recibir_respuesta_obtener_pedido(int socket){
	return recibir_plato_listo(socket);
}


//FINALIZAR PEDIDO
void enviar_finalizar_pedido(char* nombre, int idPedido, int socket){
	t_pedido_nombre a_enviar= armar_guardar_pedido(nombre, idPedido);//usa la funcion de guardar pedido
	enviar_finalizacion_pedido(a_enviar, socket);
}



void enviar_finalizacion_pedido(t_pedido_nombre pedido, int socket){
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*2
				 + strlen(pedido.nombre)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pedido.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &pedido.nombre_largo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre, strlen(pedido.nombre)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, FINALIZAR_PEDIDO, socket);

}

t_pedido_nombre* recibir_finalizar_pedido(int socket_cliente)//
{

	return recibir_guardar_pedido(socket_cliente);

}

void responder_finalizar_pedido(char* respuesta, int id, int socket){
	t_pedido_nombre OKoFAIL= armar_guardar_pedido(respuesta, id);
	enviar_pedido_nombre(OKoFAIL, RESPUESTA_FINALIZAR_PEDIDO, socket);
}

t_pedido_nombre* recibir_respuesta_finalizar_pedido(int socket){
	return recibir_guardar_pedido(socket);
}

//TERMINAR PEDIDO( PONER ARGUMENTOS AL REVES?)
void enviar_terminar_pedido(char* nombre, int idPedido, int socket){
	t_pedido_nombre a_enviar= armar_guardar_pedido(nombre, idPedido);//usa la funcion de guardar pedido
	enviar_pedido_terminado(a_enviar, socket);
}



void enviar_pedido_terminado(t_pedido_nombre pedido, int socket){
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)*2
				 + strlen(pedido.nombre)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &pedido.id_pedido, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, &pedido.nombre_largo, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, pedido.nombre, strlen(pedido.nombre)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, TERMINAR_PEDIDO, socket);

}

t_pedido_nombre* recibir_terminar_pedido(int socket_cliente)//
{

	return recibir_guardar_pedido(socket_cliente);

}


//OBTENER RECETA - A MODIFICACION
void enviar_obtener_receta(char*nombre, int socket){
	t_restaurante plato= armar_restaurante(nombre);
	enviar_obtencion_receta(plato, socket);
}

void enviar_obtencion_receta(t_restaurante restaurante, int socket){ //CONSULTAR PLATO
	//SE CARGA EL TAD EN EL BUFFER
	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = sizeof(uint32_t)
				 + strlen(restaurante.nombre_restaurante)+1;

	void* stream = malloc(buffer->size);
	int offset = 0;

	memcpy(stream + offset, &restaurante.tamanio_nombre, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(stream + offset, restaurante.nombre_restaurante, strlen(restaurante.nombre_restaurante)+1);

	buffer->stream = stream;

	//SE AGREGA HEADER, SE ARMA PAQUETE Y SE ENVIA
	serializar_y_enviar(buffer, OBTENER_RECETA, socket);
}

t_restaurante* recibir_obtener_receta(int socket_cliente){
	return recibir_restaurante(socket_cliente);
}


//HANDSHAKES
void enviar_handshake_generico(op_code handshake, int socket_cliente){
	t_codigo* consulta = malloc(sizeof(t_codigo));

	consulta->codigo_operacion = handshake;

	int bytes = sizeof(consulta);


	send(socket_cliente, consulta, bytes, 0);

	free(consulta);
}



