/*
 * servidor.c
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#include "../Servidor-tp0/servidor.h"

int main(void)
{
	void iterator(char* value)
	{
		printf("%s\n", value);
	}

	logger = log_create("log.log", "Servidor", 1, LOG_LEVEL_DEBUG);

	int server_fd = iniciar_servidor();
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);


	t_list* lista;
	//para prueba-------------------------------------------
	char* afinidades = "[Milanesas, Empanadas, Ensalada]";
	int posX=1;
	int posY=2;
	//char*recetas=malloc(22);
	char* recetas="[Milanesa, Empanada]";
	//char*precios=malloc(12);
	char* precios="[120, 40]";
	int cantHornos = 3;
	int cantPedidos=4;
	//--------------------------
	while(1)
	{
		int cod_op = recibir_operacion(cliente_fd);
		//t_respuesta_obtener_restaurante* respuesta_restaurante;

		switch(cod_op)
		{
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case CONSULTAR_RESTAURANTES:
			printf("Se pidio consultar restaurantes\n");//funciona, pero ver porq queda a la espera del proximo comando para imprimir por pantalla
			//enviar_consultar_restaurantes(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			printf("Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case PEDIDO:; //agregado para prueba
			t_pedido_nombre* prueba;
			prueba = recibir_guardar_pedido(cliente_fd);
			printf("Me llego el pedido de nombre:%s, y de id:%d\n", prueba->nombre, prueba->id_pedido);
			free(prueba);
			break;
		case SELECCIONAR_RESTAURANTE:;
			t_seleccionar_restaurante* seleccion;
			seleccion = recibir_seleccionar_restaurante(cliente_fd);
			printf("Me llego la seleccion del restaurante: %s, del cliente: %s, con posicion x en: %d, posicion y en: %d\n", seleccion->nombre_restaurante,seleccion->cliente.id_cliente, seleccion->cliente.posicion_x, seleccion->cliente.posicion_y);
			free(seleccion);
			break;
		case OBTENER_RESTAURANTE:;
			t_restaurante* restaurante;
			restaurante = recibir_restaurante(cliente_fd);
			printf("Me llego la obtencion del restaurante: %s\n", restaurante->nombre_restaurante);
			free(restaurante);
			responder_obtener_restaurante(afinidades, posX, posY, recetas, precios,cantHornos, cantPedidos, cliente_fd);
			printf("envie la respuesta\n");
			break;
		case CONSULTAR_PLATOS:;
			t_restaurante* restaurante_a_consultar;
			restaurante_a_consultar = recibir_restaurante(cliente_fd);
			printf("Mostrar Platos del restaurante %s\n", restaurante->nombre_restaurante);
			free(restaurante_a_consultar);
			break;
		case CREAR_PEDIDO:;
			printf("Me llego la peticion para CREAR PEDIDO\n");
			break;
		case ANIADIR_PLATO:;
			t_pedido_nombre* pedido;
			pedido = recibir_aniadir_plato(cliente_fd);
			printf("recibi el plato %s, del id %d\n", pedido->nombre, pedido->id_pedido);
			free(pedido);
			break;
		case GUARDAR_PLATO:;
			t_guardar_plato* plato_a_guardar;
			plato_a_guardar= recibir_guardar_plato(cliente_fd);
			printf("Me llego el plato: %s, para agregar al pedido de id: %d, del restaurante: %s, cantidad: %d\n", plato_a_guardar->nombre_comida, plato_a_guardar->id_pedido, plato_a_guardar->nombre_restaurante, plato_a_guardar->cantidad);
			free(plato_a_guardar);
			break;
		case CONFIRMAR_PEDIDO:;
			t_pedido_nombre* confirmacion;
			confirmacion = recibir_confirmar_pedido(cliente_fd);
			printf("Me llego el restaurante: %s, pedido %d\n", confirmacion->nombre, confirmacion->id_pedido);
			free(confirmacion);
			break;
		case PLATO_LISTO:;
			t_plato_listo* plato_listo;
			plato_listo = recibir_plato_listo(cliente_fd);
			printf("Me llego el restaurante %s, comida %s, pedido %d\n", plato_listo->nombre_restaurante, plato_listo->nombre_comida, plato_listo->id_pedido);
			free(plato_listo);
			break;
		case CONSULTAR_PEDIDO:;//CAMBIAR?
			int* id_pedido;
			id_pedido = recibir_consultar_pedido(cliente_fd);
			printf("consulta del pedido %d\n", *id_pedido);
			free(id_pedido);
			break;
		case OBTENER_PEDIDO:;
			t_pedido_nombre* pedido_a_obtener;
			pedido_a_obtener= recibir_obtener_pedido(cliente_fd);
			printf("pedido a obtener del restaurante %s, id %d\n", pedido_a_obtener->nombre, pedido_a_obtener->id_pedido);
			free(pedido_a_obtener);
			break;
		case FINALIZAR_PEDIDO:;
			t_pedido_nombre* pedido_a_finalizar;
			pedido_a_finalizar= recibir_finalizar_pedido(cliente_fd);
			printf("pedido a finalizar del restaurante %s, id %d\n", pedido_a_finalizar->nombre, pedido_a_finalizar->id_pedido);
			free(pedido_a_finalizar);
			break;
		case TERMINAR_PEDIDO:;
			t_pedido_nombre* pedido_a_terminar;
			pedido_a_terminar= recibir_terminar_pedido(cliente_fd);
			printf("pedido a terminar del restaurante %s, id %d\n", pedido_a_terminar->nombre, pedido_a_terminar->id_pedido);
			free(pedido_a_terminar);
			break;
		case OBTENER_RECETA:;
			t_restaurante* obtener_receta;
			obtener_receta= recibir_obtener_receta(cliente_fd);
			printf("se pide receta del plato %s\n", obtener_receta->nombre_restaurante);
			free(obtener_receta);
			break;
		case RESPUESTA_OBTENER_RESTAURANTE:;
			t_respuesta_obtener_restaurante* respuesta_restaurante;
			respuesta_restaurante = recibir_respuesta_obtener_restaurante(cliente_fd);
			printf("Recibi las afinidades: %s, posX: %d, posY: %d, recetas: %s, precios: %s, cantidad hornos: %d, cantidad pedidos: %d\n", respuesta_restaurante->afinidades, respuesta_restaurante->posX, respuesta_restaurante->posY, respuesta_restaurante->recetas, respuesta_restaurante->precios, respuesta_restaurante->cant_hornos, respuesta_restaurante->cant_pedidos);
			free(respuesta_restaurante);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}
	return EXIT_SUCCESS;
}
