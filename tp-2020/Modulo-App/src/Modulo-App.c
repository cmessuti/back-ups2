#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Modulo-App.h"
#include "utils.h"

int main(void) {
	puts("!!!Soy Modulo App!!!"); /* prints !!!Soy Modulo App!!! */
	return EXIT_SUCCESS;
}

/*---------------ADMINISTRACIÓN DE MENSAJES-------------------*/
char* ConsultarRestaurante_App(){
	if (hayRestaurantesConectados())
		return getRestaurantesConectados();
	
	return "Resto Default";
}

Cliente SeleccionarRestaurante_App(int ID_CLIENTE, char* NombreDeRes){
	
	Cliente clnt;
	clnt.num_cliente = ID_CLIENTE;
	clnt.NombreDeRes = NombreDeRes;
	clnt.estado = New;
	clnt.delivery.id_delivery = 0;
	clnt.delivery.pos_x = 0;
	clnt.delivery.pos_y = 0;

	return clnt;
}

char* ConsultarPlatos_App(Cliente clnt){
	if(estaConectado(clnt.NombreDeRes))
		return ConsultarPlatos_Sindicato(clnt.NombreDeRes);
	
	return "Platos por default";
}

char* CrearPedido_App(Cliente clnt){
	
	clnt.id_pedido = CrearPedido_Restaurant(clnt.NombreDeRes);
	
	char* msj;
	msj = GuardarPedido_Comanda();
	if (msj == "Fail")
		return "Hubo un error en la creación del pedido, por favor intente nuevamente";
	return clnt.id_pedido;
}

char* AgregarPlato_App(Cliente clnt){
	char* msj;
	
	msj = AgregarPlato_Restaurant(clnt.NombreDeRes, clnt.id_pedido);
	if (msj == "Fail")
		return "Hubo un error en el agregado del plato, por favor intente nuevamente";
	
	msj = AgregarPlato_Comanda(clnt.NombreDeRes, clnt.id_pedido);
	if (msj == "Fail")
		return "Hubo un error en el agregado del plato, por favor intente nuevamente";
	
	return "Ok";
}

void PlatoListo_App(char* NombreDeRes, char* id_pedido, char* id_plato){
	
	PlatoListo_Comanda(NombreDeRes, id_pedido, id_plato);
	
	char* pedido;
	pedido = ObtenerPedido_Comanda(NombreDeRes, id_pedido);
	
	if (PedidoEstaListo(pedido))
		PedidoListoARepartir(NombreDeRes, id_pedido);
}

char* ConfirmarPedido_App(Cliente clnt){
	char* pedido;
	pedido = ObtenerPedido_Comanda(clnt.NombreDeRes, clnt.id_pedido);

	char* msj;
	msj = ConfirmarPedido_Restaurant(clnt.NombreDeRes, pedido);
	
	if (msj == "Fail")
		return "Hubo un error confirmado el pedido, por favor intente nuevamente";
	
	// ---------------------------------------------------
	// HACER EL PCB Y DEJARLO EN EL CICLO DE PLANIFICACION
	// ---------------------------------------------------

	ConfirmarPedido_Comanda(clnt.NombreDeRes, clnt.id_pedido);
	ConfirmarPedido_Cliente(clnt.NombreDeRes, clnt.id_pedido);
	
	return "Ok";
}

char* ConsultarPedido_App(char* NombreDeRes, char* id_pedido){
	char* pedido;
	pedido = ObtenerPedido_Comanda(NombreDeRes, id_pedido);

	char* msj;
	msj = "";

	strcat(msj, pedido[11]);
	strcat(msj, '\n');
	
	int i;
	i = 21;							// ------------------------------------
									//    ------------------------------
	while (pedido != EOF){			//				
		strcat(msj, pedido[i]);		//    		REVISAR ESTA PARTE
		strcat(msj, '\n');			//
		i += 10;					//    ------------------------------
	}								// ------------------------------------

	return msj;
}

/*----------------PLANIFICACIÓN DE PEDIDOS--------------------*/
/*------------------------------------------------------------*/



int hayRestaurantesConectados(){
/*
------FALTA DESARROLLO------
*/
}
char* getRestaurantesConectados(){
/*
------FALTA DESARROLLO------
*/
}

int estaConectado(char* NombreDeRes){
/*
------FALTA DESARROLLO------
*/	
}
int PedidoEstaListo(char* id_pedido){
/*
------FALTA DESARROLLO------
*/	
}

char* PedidoListoARepartir(char* NombreDeRes, char* id_pedido){
/*
------FALTA DESARROLLO------
*/	
}