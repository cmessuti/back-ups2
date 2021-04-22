#include<netdb.h>
#include<string.h>
#include<unistd.h>

t_log* iniciar_logger(void);
int iniciar_servidor(t_log* logger, t_config* config);
t_config* leer_config(void);
