#include "Modulo-Sindicato.h"
/*#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include "utils.h"
#include <errno.h>
#include <string.h>
//#include "../../Conexion-Serializacion/serializacion.h"
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct Buffer_Pedido {
    t_pedido Pedido;
    t_restaurante Restaurante;
    t_receta Receta;
    t_posicion Posicion;
} Buffer_Pedido;

int main(void) {
	int conexion;
	char* ip;
	char* puerto;
	char* cadena = malloc(125);
	char* comando = malloc(25);
	char* msj;
	start_filesystem();

//	t_log* logger;
//	t_config* config;

//	logger = iniciar_logger();
//	config = leer_config();

	Buffer_Pedido *buf;
	buf = malloc(sizeof(Buffer_Pedido));
	printf("Bienvenido a la consola de Sindicato\n");
	while (1) {
		printf("Por favor indicar un comando: ");
		msj = scanf("%s", msj);
		//cadena = esperar_mensaje();
		char** cadenaSeparada = string_split(cadena, " ");

		comando = *cadenaSeparada;

		if(strcmp(comando, "CrearReceta") == 0) {
			pthread_t hilo;

			buf->Pedido.Comida = *(cadenaSeparada+1);
			buf->Receta.Pasos = *(cadenaSeparada+2);
			buf->Receta.TiempoPasos = *(cadenaSeparada+3);

			pthread_create(&hilo, NULL, CrearReceta_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}

		if(strcmp(comando, "CrearRestaurante") == 0) {
			pthread_t hilo;

			buf->Restaurante.NombreDeRes = *(cadenaSeparada+1);
			buf->Restaurante.CantidadCocineros = *(cadenaSeparada+2);
			buf->Restaurante.Posicion.x_y = *(cadenaSeparada+3);
			buf->Restaurante.AfinidadCocineros = *(cadenaSeparada+4);
			buf->Restaurante.Platos = *(cadenaSeparada+5);
			buf->Restaurante.PrecioPlatos = *(cadenaSeparada+6);
			buf->Restaurante.CantidadHornos = *(cadenaSeparada+7);

			pthread_create(&hilo, NULL, CrearRestaurante_Sindicato, &buf);
			pthread_join(hilo, NULL);
			printf("Restaurante creado");
		}
		if(strcmp(comando,"ConsultarPlatos") == 0) {
			pthread_t hilo;

			buf->Restaurante.NombreDeRes = *(cadenaSeparada+1);

			pthread_create(&hilo, NULL, ConsultarPlatos_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}
		
		if(strcmp(comando,"GuardarPedido") == 0) {
			pthread_t hilo;

			buf->Restaurante.NombreDeRes = *(cadenaSeparada+1);
			buf->Pedido.IdPedido = (int)*(cadenaSeparada+2);

			pthread_create(&hilo, NULL, GuardarPedido_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}
		
		if(strcmp(comando,"GuardarPlato") == 0) {
			pthread_t hilo;

			buf->Restaurante.NombreDeRes = *(cadenaSeparada+1);
			buf->Pedido.IdPedido= (int)*(cadenaSeparada+2);
			buf->Pedido.Comida = *(cadenaSeparada+3);
			buf->Pedido.CantidadPlatos = (int)*(cadenaSeparada+4);

			pthread_create(&hilo, NULL, GuardarPlato_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}

		if(strcmp(comando,"ConfirmarPedido") == 0) {
			pthread_t hilo;

			buf->Restaurante.NombreDeRes = *(cadenaSeparada+1);
			buf->Pedido.IdPedido = (int)*(cadenaSeparada+2);

			pthread_create(&hilo, NULL, ConfirmarPedido_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}

		if(strcmp(comando,"ObtenerPedido") == 0) {
			pthread_t hilo;

			buf->Restaurante.NombreDeRes = *(cadenaSeparada+1);
			buf->Pedido.IdPedido = (int)*(cadenaSeparada+2);

			pthread_create(&hilo, NULL, ObtenerPedido_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}

		if(strcmp(comando,"ObtenerRestaurante") == 0) {
			pthread_t hilo;

			buf->Restaurante.NombreDeRes = *(cadenaSeparada+1);

			pthread_create(&hilo, NULL, ObtenerRestaurante_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}

		if(strcmp(comando,"PlatoListo") == 0) {
			pthread_t hilo;

			buf->Restaurante.NombreDeRes = *(cadenaSeparada+1);
			buf->Pedido.IdPedido = (int)*(cadenaSeparada+2);
			buf->Pedido.Comida = *(cadenaSeparada+3);

			pthread_create(&hilo, NULL, PlatoListo_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}

		if(strcmp(comando,"ObtenerReceta") == 0) {
			pthread_t hilo;

			buf->Pedido.Comida = *(cadenaSeparada+1);

			pthread_create(&hilo, NULL, ObtenerReceta_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}
		
		if(strcmp(comando,"TerminarPedido") == 0) {
			pthread_t hilo;

			buf->Restaurante.NombreDeRes = *(cadenaSeparada+1);
			buf->Pedido.IdPedido = (int)*(cadenaSeparada+2);

			pthread_create(&hilo, NULL, TerminarPedido_Sindicato, &buf);
			pthread_join(hilo, NULL);
		}

	}
	
	free(buf);
	free(comando);
	//terminar_programa(conexion, logger, config);
	return 0;
}

void* CrearReceta_Sindicato(void* buf) {
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	if (existeReceta(buffer->Pedido.Comida))
		return "Fail";

	char* NombreDeReceta;
	NombreDeReceta = "AFIP/Files/Recetas/";
	strcat(NombreDeReceta, buffer->Pedido.Comida);
	mkdir(NombreDeReceta, 0700);

	char fileroute[150] = "AFIP/Files/Recetas/";
	strcat(fileroute, buffer->Pedido.Comida);
	strcat(fileroute, ".AFIP");

	FILE* fptr;
	
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "w");
	
	if (!fptr)
		return "Fail";

	fprintf(fptr, "PASOS=%s", buffer->Receta.Pasos);
	fprintf(fptr, "TIEMPO_PASOS=%s", buffer->Receta.TiempoPasos);

	fclose(fptr);
	pthread_mutex_unlock(&mutex);
	
	return "Ok";
}

void* CrearRestaurante_Sindicato(void* buf) {
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	if (existeRestaurante(buffer->Restaurante.NombreDeRes))
		return (void*) "Fail";
	
	char* ResRoute;
	ResRoute = "AFIP/Files/Restaurantes/";
	strcat(ResRoute, buffer->Restaurante.NombreDeRes);
	mkdir(ResRoute, 0700);

	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, buffer->Restaurante.NombreDeRes);
	strcat(fileroute, "/Info.AFIP");

	FILE* fptr;
	
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "w");
	
	if (!fptr)
		return (void*) "Fail";

	fprintf(fptr, "CANTIDAD_COCINEROS=%s", buffer->Restaurante.CantidadCocineros);
	fprintf(fptr, "POSICION=%s", buffer->Restaurante.Posicion.x_y);
	fprintf(fptr, "AFINIDAD_COCINEROS=%s", buffer->Restaurante.AfinidadCocineros);
	fprintf(fptr, "PLATOS=%s", buffer->Restaurante.Platos);
	fprintf(fptr, "PRECIO_PALTOS=%s", buffer->Restaurante.PrecioPlatos);
	fprintf(fptr, "CANTIDAD_HORNOS=%s", buffer->Restaurante.CantidadHornos);

	fclose(fptr);
	pthread_mutex_unlock(&mutex);

	return "Ok";
}

void* ConsultarPlatos_Sindicato(void* buf) {
	char* NombreDeRes;
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	NombreDeRes = buffer->Restaurante.NombreDeRes;

	if (existeRestaurante(NombreDeRes))
		return ObtenerPlatos(NombreDeRes);
	
	return "No existe el restaurante indicado.";
}

void* GuardarPedido_Sindicato(void* buf) {
	char* NombreDeRes;
	int IdPedido;
	char* estado;
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;
	
	NombreDeRes = buffer->Restaurante.NombreDeRes;
	IdPedido = buffer->Pedido.IdPedido;

	if (existeRestaurante(NombreDeRes)) {
		
		if (existenPedidos(NombreDeRes))
			estado = CrearSiguientePedido(NombreDeRes, IdPedido, buffer->Pedido);

		else
			estado = CrearPrimerPedido(NombreDeRes, buffer->Pedido);

		return estado;
	}

	return "No existe el restaurante indicado.";
}

void* GuardarPlato_Sindicato(void* buf) {
	char* estado;
	char* NombreDeRes;
	char* Comida;
	int IdPedido;
	int Cantidad;
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	NombreDeRes = buffer->Restaurante.NombreDeRes;
	Comida = buffer->Pedido.Comida;
	IdPedido = buffer->Pedido.IdPedido;
	Cantidad = buffer->Pedido.CantidadPlatos;
	
	if (existeRestaurante(NombreDeRes)) {
		
		if (existePedido(NombreDeRes, IdPedido)) {
			
			if(obtenerEstadoPedido(NombreDeRes, IdPedido) == "PENDIENTE") {
				
				if(existePlatoEnPedido(NombreDeRes, IdPedido, Comida))
					estado = SumarCantidadAPedido(NombreDeRes, IdPedido, Comida, Cantidad);
				
				else
					estado = ActualizarPrecioPedido(NombreDeRes, IdPedido, Comida, Cantidad);
					
					if (estado == "Ok")
						estado = AgregarNuevoPlatoAPedido(NombreDeRes, IdPedido, Comida, Cantidad);
				
				return estado;
			}

			return "El pedido no se encuentra en estado \'Pendiente\'.";
		}

		return "No existe el pedido indicado.";
	}

	return "No existe el restaurante indicado.";
}

void* ConfirmarPedido_Sindicato(void* buf) {
	char* estado;
	char* NombreDeRes;
	int IdPedido;
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	NombreDeRes = buffer->Restaurante.NombreDeRes;
	IdPedido = buffer->Pedido.IdPedido;

	if (existeRestaurante(NombreDeRes)) {
		
		if (existePedido(NombreDeRes, IdPedido)) {
			
			if(obtenerEstadoPedido(NombreDeRes, IdPedido) == "PENDIENTE") {
				estado = ActualizarEstadoPedido(NombreDeRes, IdPedido, "CONFIRMADO");

				return estado;
			}

			return "El pedido no se encuentra en estado \'Pendiente\'.";
		}

		return "No existe el pedido indicado.";
	}

	return "No existe el restaurante indicado.";
}

void* ObtenerPedido_Sindicato(void* buf) {
	char* NombreDeRes;
	int IdPedido;
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	NombreDeRes = buffer->Restaurante.NombreDeRes;
	IdPedido = buffer->Pedido.IdPedido;

	if (existeRestaurante(NombreDeRes)) {
		
		if (existePedido(NombreDeRes, IdPedido))
			return ObtenerDatosPedido(NombreDeRes, IdPedido);

		return "No existe el pedido indicado.";
	}

	return "No existe el restaurante indicado.";
}

void* ObtenerRestaurante_Sindicato(void* buf) {
	char* NombreDeRes;
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	NombreDeRes = buffer->Restaurante.NombreDeRes;

	if (existeRestaurante(NombreDeRes))
		return ObtenerDatosRestaurate(NombreDeRes);
	
	return "No existe el restaurante indicado.";
}

void* PlatoListo_Sindicato(void* buf) {
	char* estado;
	char* NombreDeRes;
	char* Comida;
	int IdPedido;
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	NombreDeRes = buffer->Restaurante.NombreDeRes;
	IdPedido = buffer->Pedido.IdPedido;
	Comida = buffer->Pedido.Comida;
	
	if (existeRestaurante(NombreDeRes)) {
	
		if (existePedido(NombreDeRes, IdPedido)) {
			
			if(obtenerEstadoPedido(NombreDeRes, IdPedido) == "CONFIRMADO") {

				if (existePlatoEnPedido(NombreDeRes, IdPedido, Comida)) {
					estado = AgregarPlatoAListo(NombreDeRes, IdPedido, Comida);

					return estado;
				}

				return "El plato no existe en el pedido.";
			}
		
			return "El pedido no se encuentra en estado \'Confirmado\'.";
		}

		return "No existe el pedido indicado.";
	}

	return "No existe el restaurante indicado.";
}

void* TerminarPedido_Sindicato(void* buf) {
	char* estado;
	char* NombreDeRes;
	int IdPedido;
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	NombreDeRes = buffer->Restaurante.NombreDeRes;
	IdPedido = buffer->Pedido.IdPedido;

	if (existeRestaurante(NombreDeRes)) {
		
		if (existePedido(NombreDeRes, IdPedido)) {
			
			if(obtenerEstadoPedido(NombreDeRes, IdPedido) == "CONFIRMADO") {
				estado = ActualizarEstadoPedido(NombreDeRes, IdPedido, "TERMINADO");
				
				return estado;
			}

			return "El pedido no se encuentra en estado \'Confirmado\'.";
		}

		return "No existe el pedido indicado";
	}

	return "No existe el restaurante indicado";
}

void* ObtenerReceta_Sindicato(void* buf) {
	char* Comida;
	struct Buffer_Pedido* buffer = (struct Buffer_Pedido*) buf;

	Comida = buffer->Pedido.Comida;

	if (existeReceta(Comida))
		return ObtenerInfoReceta(Comida);
	
	return "La receta solicitada no existe.";
}

int existeRestaurante(char* NombreDeRes) {
	
	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	
	FILE* fptr;
	
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "r");
	pthread_mutex_unlock(&mutex);

	if (fptr)
		pthread_mutex_lock(&mutex);
		fclose(fptr);
		pthread_mutex_unlock(&mutex);
		return 1;
	
	pthread_mutex_lock(&mutex);
	fclose(fptr);
	pthread_mutex_unlock(&mutex);
	return 0;
}

char* ObtenerPlatos(char* NombreDeRes) {
	
	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	strcat(fileroute, "/Info.AFIP");
	
	FILE* fptr;

	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "r");
	if (!fptr)
		return "Fail";
	
	char** fstr;
	char linestr[100];
	if (fgets(linestr, 100, fptr) != NULL)
		if (fgets(linestr, 100, fptr) != NULL)
			if (fgets(linestr, 100, fptr) != NULL)
				if (fgets(linestr, 100, fptr) != NULL)
					fstr = string_split(linestr, "=");
	
	fclose(fptr);
	pthread_mutex_unlock(&mutex);
	return *(fstr+1);
}

int existenPedidos(char* NombreDeRes) {
	
	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	strcat(fileroute, "/Pedido1.AFIP");

	FILE* fptr;
	
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "r");
	pthread_mutex_unlock(&mutex);

	if (fptr)
		pthread_mutex_lock(&mutex);
		fclose(fptr);
		pthread_mutex_unlock(&mutex);
		return 1;
	
	pthread_mutex_lock(&mutex);
	fclose(fptr);
	pthread_mutex_unlock(&mutex);
	return 0;
}

int existePedido(char* NombreDeRes, int IdPedido) {
	
	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	strcat(fileroute, "/");
	strcat(fileroute, IdPedido);
	strcat(fileroute, ".AFIP");
	
	FILE* fptr;
	
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "r");
	pthread_mutex_unlock(&mutex);

	if (fptr)
		pthread_mutex_lock(&mutex);
		fclose(fptr);
		pthread_mutex_unlock(&mutex);
		return 1;

	pthread_mutex_lock(&mutex);
	fclose(fptr);
	pthread_mutex_unlock(&mutex);
	return 0;
}

char* CrearSiguientePedido(char* NombreDeRes, int IdPedido, t_pedido pedido) {
	
	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	strcat(fileroute, "/");
	strcat(fileroute, IdPedido);
	strcat(fileroute, ".AFIP");

	FILE* fptr;
	
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "w");
	
	if (!fptr)
		return "Fail";

	fprintf(fptr, "ESTADO_PEDIDO=%s", pedido.EstadoPedido);
	fprintf(fptr, "LISTA_PLATOS=%s", pedido.ListaPlatos);
	fprintf(fptr, "CANTIDAD_PLATOS=%s", pedido.CantidadPlatos);
	fprintf(fptr, "CANTIDAD_LISTA=%s", pedido.CantidadLista);
	fprintf(fptr, "PRECIO_TOTAL=%f", pedido.PrecioTotal);
	fclose(fptr);
	pthread_mutex_unlock(&mutex);

	return "Ok";
}

char* CrearPrimerPedido(char* NombreDeRes, t_pedido pedido) {

	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	strcat(fileroute, "/Pedido1.AFIP");

	FILE* fptr;
	
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "w");
	
	if (!fptr)
		return "Fail";

	fprintf(fptr, "ESTADO_PEDIDO=%s", pedido.EstadoPedido);
	fprintf(fptr, "LISTA_PLATOS=%s", pedido.ListaPlatos);
	fprintf(fptr, "CANTIDAD_PLATOS=%s", pedido.CantidadPlatos);
	fprintf(fptr, "CANTIDAD_LISTA=%s", pedido.CantidadLista);
	fprintf(fptr, "PRECIO_TOTAL=%d", pedido.PrecioTotal);
	fclose(fptr);
	pthread_mutex_unlock(&mutex);

	return "Ok";

}

char* obtenerEstadoPedido(char* NombreDeRes, int IdPedido){

	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	strcat(fileroute, "/");
	strcat(fileroute, IdPedido);
	strcat(fileroute, ".AFIP");

	FILE* fptr;

	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "r");
	if (!fptr)
		return "Fail";
	
	char** fstr;
	char linestr[100];
	if (fgets(linestr, 100, fptr) != NULL)
		fstr = string_split(linestr, "=");
	
	fclose(fptr);
	pthread_mutex_unlock(&mutex);
	return *(fstr+1);

}

int existePlatoEnPedido(char* NombreDeRes, int IdPedido, char* Comida){
	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	strcat(fileroute, "/");
	strcat(fileroute, IdPedido);
	strcat(fileroute, ".AFIP");

	FILE* fptr;
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "r");
	
	if (!fptr)
		return 0;
	
	char linestr[100];
	char** fstr;

	if (fgets(linestr, 100, fptr) != NULL)
		fstr = string_split(linestr, "=");
	fclose(fptr);
	pthread_mutex_unlock(&mutex);

	if (*(fstr+1) == EOF)
		return 0;
	
	return 1;
}

char* SumarCantidadAPedido(char* NombreDeRes, int IdPedido, char* Comida, int Cantidad){
	/*
	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	strcat(fileroute, "/");
	strcat(fileroute, IdPedido);
	strcat(fileroute, ".AFIP");

	FILE* fptr;
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "r");
	
	if (!fptr)
		return "Fail";
	
	char* linestr;
	char** fstr;

	if (fgets(linestr, 100, fptr) != NULL)
		fstr = string_split(linestr, "=");
	fclose(fptr);
	pthread_mutex_unlock(&mutex);

	return "Ok";
	*/
}

char* ActualizarPrecioPedido(char* NombreDeRes, int IdPedido, char* Comida, int Cantidad){
	
	char fileroute[150] = "AFIP/Files/Restaurantes/";
	strcat(fileroute, NombreDeRes);
	strcat(fileroute, "/Info.AFIP");

	FILE* fptr;
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "r");
	
	if (!fptr)
		return "Fail";
	
	char linestr[100];
	char** fstr;

	if (fgets(linestr, 100, fptr) != NULL)
		if (fgets(linestr, 100, fptr) != NULL)
			fstr = string_split(linestr, "=");
	
	int pos;

	for(int i=0; i<100; i++) {
		if (strcmp(*(fstr+i), NULL))
			break;

		if(strcmp(*(fstr+i), Comida))
			pos = i;	
			break;	
	}

	if (fgets(linestr, 100, fptr) != NULL)
		fstr = string_split(linestr, "=");

	int price_a;
	price_a = atoi(*(fstr+pos));

	fclose(fptr);
	pthread_mutex_unlock(&mutex);

	char route[150] = "AFIP/Files/Restaurantes/";
	strcat(route, NombreDeRes);
	strcat(route, "/");
	strcat(route, IdPedido);
	strcat(route, ".AFIP");

	pthread_mutex_lock(&mutex);
	fptr = fopen(route, "a");
	if (!fptr)
		return "Fail";
	
	char** fstr_i;
	char linestr_i[100];
	if (fgets(linestr_i, 100, fptr) != NULL)
		if (fgets(linestr_i, 100, fptr) != NULL)
			if (fgets(linestr_i, 100, fptr) != NULL)
				if (fgets(linestr_i, 100, fptr) != NULL)
					if (fgets(linestr_i, 100, fptr) != NULL)
						fstr_i = string_split(linestr_i, "=");

	int price_b;
	price_b = atoi(*(fstr_i+1)) + price_a;

	/*
		ACÁ ES NECESARIO IR HASTA LA LINEA DEL ARCHIVO E IMPRIMIR
		EL NUEVO VALOR EN LA LINEA DE PRECIO TOTAL DEL PEDIDO
	*/


	pthread_mutex_unlock(&mutex);
	return "Ok";

}

char* AgregarNuevoPlatoAPedido(char* NombreDeRes, int IdPedido, char* Comida, int Cantidad){
/*
------FALTA DESARROLLO------
*/
}

char* ActualizarEstadoPedido(char* NombreDeRes, int IdPedido, char* ESTADO){
/*
------FALTA DESARROLLO------
*/
}

char* ObtenerDatosPedido(char* NombreDeRes, int IdPedido){
/*
------FALTA DESARROLLO------
*/
}

char* ObtenerDatosRestaurate(char* NombreDeRes){
/*
------FALTA DESARROLLO------
*/
}

char* AgregarPlatoAListo(char* NombreDeRes, int IdPedido, char* Comida){
/*
------FALTA DESARROLLO------
*/
}

int existeReceta(char* Receta){
	char fileroute[150] = "AFIP/Files/Recetas/";
	strcat(fileroute, Receta);
	strcat(fileroute, ".AFIP");
	
	FILE* fptr;
	
	pthread_mutex_lock(&mutex);
	fptr = fopen(fileroute, "r");
	pthread_mutex_unlock(&mutex);

	if (fptr)
		pthread_mutex_lock(&mutex);
		fclose(fptr);
		pthread_mutex_unlock(&mutex);
		return 1;

	pthread_mutex_lock(&mutex);
	fclose(fptr);
	pthread_mutex_unlock(&mutex);
	return 0;
}

char* ObtenerInfoReceta(char* Receta){
/*
------FALTA DESARROLLO------
*/
}

void start_filesystem() {
	
	mkdir("AFIP", 0700);
	if (errno == EEXIST)
		return;

	char* filetree[5];

	filetree[0] = "AFIP/Blocks";
	filetree[1] = "AFIP/Metadata";
	filetree[2] = "AFIP/Files";
	filetree[3] = "AFIP/Files/Restaurantes";
	filetree[4] = "AFIP/Files/Recetas";
	
	for (int i = 0; i < 5; i++) {
		if (mkdir(filetree[i], 0700))
			perror(filetree[i]);
	}

	FILE* fptr;
	fptr = fopen("AFIP/Metadata/Metadata.AFIP", "w");
	fprintf(fptr, "BLOCK_SIZE=00\n");
	fprintf(fptr, "BLOCKS=00\n");
	fprintf(fptr, "MAGIC_NUMBER=AFIP\n");

	/*
	HAY QUE USAR LAS COMMONS PARA ARMAR EL BITARRAY
	EL MISMO VA EN AFIP/Metadata/bitmap.bin
	*/
}

/*
t_log* iniciar_logger(void){
	return log_create("Sindicato.log","Cliente",1,LOG_LEVEL_INFO);
}

t_config* leer_config(void){
	return config_create("Sindicato.config");
}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	log_destroy(logger);
	config_destroy(config);
	close(conexion);
}
*/
