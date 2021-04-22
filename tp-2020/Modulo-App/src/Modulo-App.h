#ifndef _MOD_APP_H_
#define _MOD_APP_H_

enum STATE {New, Ready, Blocked, Exec, Exit};

typedef struct Cliente {
	int num_cliente;
	char* id_pedido;
	char* NombreDeRes;
	enum STATE estado;
	Delivery delivery;
} Cliente;

typedef struct Delivery {
	int id_delivery;
	int pos_x, pos_y;
} Delivery;

char* ConsultarRestaurante_App();
Cliente SeleccionarRestaurante_App(int, char*);
char* CrearPedido_App(Cliente);
char* AgregarPlato_App(Cliente);
void PlatoListo_App(char*, char*, char*);
char* ConfirmarPedido_App(Cliente);
char* ConsultarPedido_App(char*, char*);

int hayNombreDeResConectados();
char* getNombreDeResConectados();
int estaConectado(char*);
int PedidoEstaListo(char*);
char* PedidoListoARepartir(char*, char*);

#endif