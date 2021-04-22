all: Cliente Comanda
Cliente: Modulo-Cliente/src/Modulo-Cliente.c Modulo-Cliente/src/estructuras_cliente.c 
	gcc Modulo-Cliente/src/Modulo-Cliente.c Modulo-Cliente/src/estructuras_cliente.c -o Cliente -lcommons -lpthread
Comanda: Modulo-Comanda/src/Modulo-Comanda.c  
	gcc Modulo-Comanda/src/Modulo-Comanda.c -o Comanda -lcommons -lpthread

