#ifndef _MOD_SINDICATO_H_
#define _MOD_SINDICATO_H_

typedef struct Pedido {
    char* IdPedido;
    char* EstadoPedido;
    char* ListaPlatos;
    char* CantidadPlatos;
    char* CantidadLista;
    int PrecioTotal;
    char* Comida;
} t_pedido;

typedef struct Posicion {
    char* x_y;
} t_posicion;

typedef struct t_restaurante {
    char* NombreDeRes;
    char* CantidadCocineros;
    t_posicion Posicion;
    char* AfinidadCocineros;
    char* Platos;
    char* PrecioPlatos;
    int CantidadHornos;
} t_restaurante;


typedef struct Receta {
    char* Pasos;
    char* TiempoPasos;
} t_receta;

void* ConsultarPlatos_Sindicato(void*);
void* GuardarPedido_Sindicato(void*);
void* GuardarPlato_Sindicato(void*);
void* ConfirmarPedido_Sindicato(void*);
void* ObtenerPedido_Sindicato(void*);
void* ObtenerRestaurante_Sindicato(void*);
void* PlatoListo_Sindicato(void*);
void* TerminarPedido_Sindicato(void*);
void* ObtenerReceta_Sindicato(void*);
void* CrearRestaurante_Sindicato(void*);
void* CrearReceta_Sindicato(void*);

int existeRestaurante(char*);
char* ObtenerPlatos(char*);
int existenPedidos(char*);
int existePedido(char*, int);
char* CrearSiguientePedido(char*, int, t_pedido);
char* CrearPrimerPedido(char*, t_pedido);
char* obtenerEstadoPedido(char*, int);
int existePlatoEnPedido(char*, int, char*);
char* SumarCantidadAPedido(char*, int, char*, int);
char* ActualizarPrecioPedido(char*, int, char*, int);
char* AgregarNuevoPlatoAPedido(char*, int, char*, int);
char* ActualizarEstadoPedido(char*, int, char*);
char* ObtenerDatosPedido(char*, int);
char* ObtenerDatosRestaurate(char*);
char* AgregarPlatoAListo(char*, int, char*);
int existeReceta(char*);
char* ObtenerInfoReceta(char*);

#endif