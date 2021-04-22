/*
 * Modulo-Restaurante.h
 *
 *  Created on: 20 sep. 2020
 *      Author: utnso
 */

#ifndef MODULO_RESTAURANTE_H_
#define MODULO_RESTAURANTE_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

#include "utils.h"

t_log* g_logger;
t_config* g_config;

void iniciar_logger(void);

#endif /* MODULO_RESTAURANTE_H_ */
