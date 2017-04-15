#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>

#include "manejoArchivo.hpp"
#include "manejoCut.hpp"
#include "espera.hpp"
#include "pipe.hpp"

#define READ 0
#define WRITE 1

int cut(char *nombreArchivo, int numeroPartes){

    std::cout << "Separando en : " << numeroPartes << " partes" << std::endl;

    pipeStruct *pipeNuevo, *pipeUltima = NULL;

    size_t tamanioFichero = largoFichero(nombreArchivo);
    size_t tamanioPartes  = largoPartes(tamanioFichero, numeroPartes);
    size_t falta          = cuantoFalta(tamanioFichero, tamanioPartes, numeroPartes); 

    size_t numeroParte = 0;
    pid_t pid          = 0;

    for(size_t i = 0; numeroParte < numeroPartes; i = i+ tamanioPartes){
        
        pipeNuevo= new pipeStruct();
        crear_pipe(pipeNuevo);
        
        pid = fork();
        
        if(pid == 0){
            // Soy hijo
            
            if(pipeUltima == NULL){
                
                /*******************************
                 * Soy 1er hijo
                 *   - Solo escribo en la pipe
                 ******************************/
                
                // Escribir por la pipe
                dup2(pipeNuevo->write, WRITE);
            }else{
                
                /***************************************
                 * No somos el 1er hijo
                 *   - Debemos leer por la pipe anterior
                 *   - Escribir por la nueva pipe
                ****************************************/
                
                // Leer por la pipe anterior
                dup2(pipeUltima->read, READ);
                
                // Escribir por la nueva pipe
                dup2(pipeNuevo->write, WRITE);
            }
            
            // Informacion necesaria para el hijo
            
            char buff_inicio[15];
            char buff_fin[15];
            char buff_numero_hijo[15];
            char buff_ultimo[15];           
            
            //char buff_inicio     = NULL;
            //char buff_fin        = NULL;
            //char buff_numeroHijo = NULL;
            //char buff_ultimo     = NULL;
            
            memset(buff_inicio      , '\0' , 14);
            memset(buff_fin         , '\0' , 14);
            memset(buff_numero_hijo , '\0' , 14);
            memset(buff_ultimo      , '\0' , 5);
            
            sprintf(buff_inicio      , "%zu" , i);
            sprintf(buff_fin         , "%zu" , i + tamanioPartes);
            sprintf(buff_numero_hijo , "%i"  , numeroParte);
         
            // Lo siguiente indica si el numero de partes es correspondiente
            // con el tamaño calculado, buff_ultimo indica si la ultima parte
            // son ceros o informacion.
            
            if(numeroParte + 1 == numeroPartes){
                // Estoy en la ultima parte
                // Indico a buff_ultimo que estoy en la ultima parte
                // buff_ultimo = 1
                sprintf(buff_ultimo, "%i%", 1); 
            }else{
                // No estoy en la ultima parte
                // Indico a buff ultimo que no estoy en la ultima parte
                // buff_ultimo = 0
                sprintf(buff_ultimo, "%i", 0);              
            }
           
            // Ejecutar cut
            if(execl("cut", "cut", nombreArchivo, buff_inicio, buff_fin, buff_numero_hijo, buff_ultimo, NULL, NULL) == -1){
                std::cout << "No se puede ejecutar el programa para cortar " << std::endl;
                return -1;
            }
            
            return 0;

        }else{
            // Soy padre
            
            pipeUltima = pipeNuevo;
            numeroParte++;        
        }    
        
    }// Fin for

    // Espero a que los hijos terminen
    espera();

    std::cout << "Se crearon las partes. " << std::endl;

    return 0;

}
