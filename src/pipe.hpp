#ifndef _H_PIPE_
#define _H_PIPE_

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

/** Estructura de pipe como alternativa a los arrays
 *
 * Para crear un pipe, utilizar pipe((int*)p);
 *
 * read contiene el descriptor para leer
 *
 * write contiene el descriptor para escribir
 *
 * */

typedef struct _pipe{
        int read, write;
}pipeStruct;

bool crear_pipe(pipeStruct *p);

#endif
