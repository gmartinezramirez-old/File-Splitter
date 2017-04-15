#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "pipe.hpp"

/**Crea una pipe
 *
 * @param p Una estructura p del tipo pipeT, es donde se va a crear la pipe
 *
 * @return true si se crea la pipe, false en caso de error*/

bool crear_pipe(pipeStruct *p){

    if(pipe((int*)p) == -1){
        
        fputs("Error, no se puede crear la pipe: ", stderr);
        fputs(strerror(errno), stderr);
        fputs("\n", stderr);
        return false;
    }else{
        return true;
    }
}


