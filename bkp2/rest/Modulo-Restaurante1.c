/*
 ============================================================================
 Name        : Modulo-Restaurante.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Modulo Restaurante in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	puts("!!!Soy Modulo Restaurante!!!"); /* prints !!!Soy Modulo Restaurante!!! */
	return EXIT_SUCCESS;
}

//-------DECLARACION DE FUNCIONES PRINCIPALES--------------
char* ConsultarPlatos(char*);
int CrearPedido();
char* AnadirPlato(char*, int);
char* ConfirmarPedido(char*, int);
char* ConsultarPedido(int);
//---------------------------------------------------------


//-------DECLARACION DE FUNCIONES INTERNAS-----------------
int existeRestaurante(char*);
char* ObtenerPlatos(char*);


//-------DESARROLLO DE FUNCIONES PRINCIPALES---------------
char* ConsultarPlatos(char* NombreDeRes) {

	if (existeRestaurante(NombreDeRes))
		return ObtenerPlatos(NombreDeRes);

	return "No existe el restaurante indicado.";
}

int CrearPedido() {
	int idPedido;
	// lo que se debe hacer es generar un id de pedido
	// e invocar guardarPedido a Sindicato, pasándole ese ID
	// y el nombre del restaurante. Ese restaurante a pasar es él mismo,
	// o sea quien recibe el CrearPedido.
	// No sé cómo se pasa eso y si hacer todo desde acá o invocando a una función interna.
	return idPedido;
}

char* AnadirPlato(char* Plato, int idPedido) {
	// deberá retornar ok/fail
	// lo que debe hacer es enviar a Sindicato GuardarPlato
	// le debe pasar Plato, IdPedido, y además nombreRestaurante(es el mismo, no sé cómo mandarlo)
	// y la cantidad de platos a agregar (no sé de donde saca ese valor)
	}

char* ConfirmarPedido(int IdPedido) {
	// deberá retornar ok/fail
	// lo que debe hacer es enviar a Sindicato ObtenerPedido
	// crear un PCB y enviar luego también a Sindicato ObtenerReceta
}

char* ConsultarPedido(int IdPedido) {
	// lo que debe hacer es enviar a Sindicato ObtenerPedido
	// pasándo el idPedido recibido y nombreRestaurante(es el mismo, no sé cómo mandarlo)
	// debe devolver lo que devuelve ObtenerDatosPedido, esa es una función
	// interna que usa Sindicato dentro de ObtenerPedido
}
//---------------------------------------------------------

//-------DESARROLLO DE FUNCIONES INTERNAS------------------
int existeRestaurante(char* NombreDeRes) {
/*
------FALTA DESARROLLO------
*/
}

char* ObtenerPlatos(char* NombreDeRes) {
/*
------FALTA DESARROLLO------
*/
}




