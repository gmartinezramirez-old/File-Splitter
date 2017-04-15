#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "manejoArchivo.hpp"

#define READ 0
#define WRITE 1

/****************************************
 * argv[1] => Nombre del fichero original
 * argv[2] => Primero
 * argv[3] => Ultimo
 *************************************** */

int main(int argc, const char *argv[])
{

    int primero      = 0;
    int ultimo       = 0;
    int numero_parte = 0;

    sscanf(argv[2], "%i", &primero);      // 1er Hijo
    sscanf(argv[3], "%i", &ultimo);       // Ultimo Hijo
    sscanf(argv[4], "%i", &numero_parte); // Numero de partes

    /************************************************************************
     * Soy el ULTIMO hijo:
     *   - Recrear el archivo que no existe con los datos pasados por el pipe
     *     y la lectura del archivo.xor
     ************************************************************************/

    if(ultimo){
        
        FILE *archivoSalida = NULL;
        FILE *archivoXor    = NULL;
        
        char xor_leido, pipe;
       
        //const char *nombreXor           = nombreConcatenar(argv[1], ".xor");
        char *nombreXor = nombreConcatenarChar(argv[1], ".xor");
        const char *nombreParteFaltante = nombreUnido(argv[1], numero_parte, ".part");
        
        std::cout << "El archivo xor es : " << nombreXor << std::endl;
        std::cout << "Restaurando la parte:  " << nombreParteFaltante << std::endl;
        
        archivoSalida = fopen(nombreParteFaltante, "w");
        archivoXor    = fopen(nombreXor, "r");
       
        // Si el archivo.xor no existe entonces no se puede armar la parte que falta
        if(archivoXor == NULL){
            fprintf(stderr, "No se puede abrir el archivo xor:   %s: %s\n", nombreXor, strerror(errno));
            fputs("No se puede recrear el archivo faltante\n", stderr);
            return -1;
        }
        
        size_t tamanioXor = largoFicheroConst(nombreXor);
        
        /****************************************************************************
         *  Leyendo la pipe y el archivo.xor
         *      - Calcular el xor entre pipe y el archivo.xor, esto da como resultado
         *        la parte que falta.
         ***************************************************************************/        
     
        // Recorrer el archivo.xor
        for(size_t i = 0; i < tamanioXor; i++){
           
            // Leer la pipe
            int ret = read(READ, &pipe, sizeof(char));
            
            // Leer 1 byte del archivo.xor
            fread(&xor_leido, sizeof(char), 1, archivoXor);
            
            // Hacer la operacion xor
            pipe = pipe ^ xor_leido;
            
            // Escribir el resultado en el archivo de salida(Parte que falta)
            fwrite(&pipe, sizeof(char), 1, archivoSalida);
        }
        
    }else{
        
        /***************************************************************
         * No soy el ultimo
         *   - El archivo existe:
         *     - Se tiene que leer y enviar lo leido por una pipe
         **************************************************************/ 
       
        const char *nombreParte = nombreUnido(argv[1], numero_parte, ".part");
       
        // Abrir parte del archivo
        FILE *lectura           = fopen(nombreParte, "r");
        
        char byte, pipe;
        
        size_t tamanioParte = largoFicheroConst(nombreParte);
       
        // Leemos la parte y la enviamos por la pipe
        for(size_t i = 0; i < tamanioParte; i++){
           
            // Leo 1 byte de la parte del archivo
            fread(&byte, sizeof(char), 1, lectura);
            
            if(primero){
                
                /*************************************************
                 * Soy el 1er hijo
                 *   - No tengo leer por el pipe, solo escribir               
                 *************************************************/
                
                // Escribo en la pipe
                write(WRITE, &byte, sizeof(char));
                
            }else{
                
                /****************************************************
                 * No soy el 1er hijo
                 *    - Debo leer por la pipe y escribir por la pipe
                 ***************************************************/
                    
                // Leo por la pipe
                read(READ, &pipe, sizeof(char));
                
                // Hago la operacion xor
                pipe = pipe ^ byte;
                
                // Escribo la operacion xor en un pipe
                write(WRITE, &pipe, sizeof(char));
            }   
        }
    } 
    
    return 0;
}


