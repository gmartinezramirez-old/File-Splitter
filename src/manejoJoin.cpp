#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <iostream>

#include "manejoArchivo.hpp"
#include "manejoJoin.hpp"
#include "espera.hpp"
#include "pipe.hpp"

#define READ 0
#define WRITE 1


int join(const char *nombreArchivo, int numeroPartes){

    std::cout << "Join" << std::endl;
    std::cout << "Uniendo las partes para crear el archivo original" << std::endl;

    const char *nombreParte = NULL;
    int numeroParte   = 0;
    int cuantosFaltan = 0;
    bool falta = false;

    // Revisar si falta una parte 


    for(numeroParte = 0; numeroParte < numeroPartes; numeroParte++){
        
        nombreParte = nombreUnido(nombreArchivo, numeroParte, ".part");
        
        if( access(nombreParte, F_OK) == -1){
            cuantosFaltan++;
            
        }        
    }

    //test
    //std::cout << "Faltan " <<   cuantosFaltan  << std::endl;

    if (cuantosFaltan > 1){
        std::cout << "Faltan: " << cuantosFaltan << " Partes " << std::endl;
        std::cout << "Falta mas de una parte, es imposible restaurar" << "  --Fin del programa" << std::endl;
        //exit(EXIT_FAILURE);
        return -1;
    }

    for(numeroParte = 0; numeroParte < numeroPartes; numeroParte++){
        
        nombreParte = nombreUnido(nombreArchivo, numeroParte, ".part");
        
        if( access(nombreParte, F_OK) == -1){
            //cuantosFaltan++;
            falta = true;
            break;
        }        
    }

    /**************************
     * Falta una parte 
     * 
     * Hay que reconstruirla
     *
     *************************/

    if(falta){
        
        std::cout << "Falta una parte, se restaurara la parte que falta " << std::endl;
        
        pipeStruct *pipeNuevo, *pipeUltima = NULL;
        
        pid_t pid = 0;
        
        // Hay que leer las partes validas, se va a saltar la parte que falta
        
        for(int i=0; i<numeroPartes; i++){
          
            // Saltar la parte que falta
            
            if( i!=numeroParte ){
                
                pipeNuevo = new pipeStruct();
                crear_pipe(pipeNuevo);
                
                pid = fork();
                
                if(pid == 0){
                    // Soy hijo
                    
                    if(pipeUltima == NULL){
                        
                        // Soy 1er hijo, solo escribo por la pipe
                        dup2(pipeNuevo->write, WRITE);       
                    }else{
                        
                        /**************************************************
                         * No soy 1er hijo
                         * Leo por la pipe anterior y escribo en nueva pipe
                         **************************************************/
                        
                        // Leo pipe anterior 
                        dup2(pipeUltima->read, READ);
                        
                        // Escribo en nueva pipe
                        dup2(pipeNuevo->write, WRITE);
                    }
                    
                  //char buff_primero = NULL;
                  //char buff_ultimo  = NULL;
                  //char buff_numero_hijo = NULL;
                    
                    char buff_primero[5];
                    char buff_ultimo[5];
                    char buff_numero_hijo[5];
                    
                    memset(buff_primero     , '\0' , 5);
                    memset(buff_ultimo      , '\0' , 5);
                    memset(buff_numero_hijo , '\0' , 5);
                    
                    /*****************************************
                     * i==0 Es el primero
                     * Si falta la parte 0, el primero es i==1
                     ****************************************/
                        
                    // Si la parte que falta no es 0 (primera parte), el primero es 0
                    if(numeroParte != 0){   
                        sprintf(buff_primero, "%i", i == 0);
                    }else{
                        
                        // Si la parte que falta es 0 (primera parte), el primero es 1
                        sprintf(buff_primero, "%i", i == 1); 
                    }
                    
                    sprintf(buff_ultimo, "0");
                    sprintf(buff_numero_hijo, "%i", i);
                   
                    // Ejecutar programa join
                    if(execl("join", "join", nombreArchivo, buff_primero, buff_ultimo, buff_numero_hijo, NULL, NULL) == -1)
                    {
                        std::cout << "No se puede ejecutar el programa para unir" << std::endl;
                        return -1;
                    }
                    
                }else{
                    
                    //Soy padre
                    pipeUltima = pipeNuevo;
                }    
            } // Fin if i != numeroParte           
        } // Fin for
        
        pid = fork();

        /*************************************************************
         * Se termina de iniciar los procesos que deben enviar datos
         * por las pipes.
         * Ahora se crea el ultimo proceso: 
         *     - Recibe datos de otros procesos.
         *     - Lee el archivo Xor.
         *     - Recrea el archivo perdido.
         ************************************************************/     
        
        if(pid == 0){
            dup2(pipeUltima->read, READ);
           
            //char buff_primero = NULL;
            //char buff_ultimo = NULL;
            //char buff_numero_hijo = NULL;
            
            char buff_primero[5];
            char buff_ultimo[5];
            char buff_numero_hijo[5];
         
            memset(buff_primero     , '\0' , 5);
            memset(buff_ultimo      , '\0' , 5);
            memset(buff_numero_hijo , '\0' , 5);
            
            // 1: existo
            sprintf(buff_primero     , "%i" , 0);
            sprintf(buff_ultimo      , "%i" , 1);  // Ultimo hijo
            sprintf(buff_numero_hijo , "%i" , numeroParte);
            
            // Ultimo hijo se encarga de unir las partes
           
            if(execl("join", "join", nombreArchivo, buff_primero, buff_ultimo, buff_numero_hijo, NULL, NULL) == -1)
            {
                std::cout << "No se puede ejecutar el programa para unir: " << std::endl;
                return -1;
            }           
        }
    } // Fin if Falta una parte

    /*******************************
     * No hace falta ningun archivo
     *******************************/

    // Espero que hijos terminen
    espera();

    /******************************
     * Unir archivos
     *****************************/

    // Rescatar numero de ceros del archivo .data
    const char *nombreDatos = nombreConcatenar(nombreArchivo, ".data");
    FILE *archivo           = fopen(nombreArchivo, "w");
    FILE *archivoDatos      = fopen(nombreDatos, "r");

    size_t ceros;

    //Si hay archivo.dato 
    if(archivoDatos){
        // Leer el numero de ceros del archivo.data
        fscanf(archivoDatos, "%zd", &ceros);
    }else{
        // Si no hay archivo.dato, no hay ceros
        ceros = 0;
    }

    // Recorrer numero de partes
    for(int i= 0; i < numeroPartes; i++){
        
        char *nombreParte_i = nombreUnidoChar(nombreArchivo, i, ".part");
        //const char *nombreParte_i = nombreUnido(nombreArchivo, i, ".part");
        FILE *nombreArchivoParte  = fopen(nombreParte_i, "r");   
       
        // Si no existe la parte del archivo
        if( nombreArchivoParte == NULL){
            fprintf(stderr, "Error abriendo la parte %s\n", nombreParte_i);
            return -1;
        }else{
            // Si existe la parte del archivo
            // Ir uniendo
            
            // Tamanio de la parte
            size_t tamanioParte = largoFicheroConst(nombreParte_i);
            
            // Ir leyendo el tamanio de la parte, de 1 byte 
            for(size_t j = 0; j < tamanioParte ; j++){
               
                // Si estamos en la ultima parte
                if( !(i < numeroPartes -1 )){
                    
                    // Verificar si estamos leyendo ceros              
                    if( j >= tamanioParte - ceros){                     
                        break;
                    }
                }
                
                // Si no estamos en la ultima parte
                // Ir leyendo y escribiendo en el archivo
                char byte;
               
                // Leer 1 byte de la parte del archivo (archivo.partNumero)
                fread(&byte  , sizeof(char) , 1 , nombreArchivoParte);
                
                // Escribir el byte leido en el archivo original
                fwrite(&byte , sizeof(char) , 1 , archivo);
                
            }            
        }
    } // Fin for  
 
    std::cout << "Se termino de restaurar el archivo original " << std::endl;
   
    // Esperar a que los hijos terminen
    espera();

    return 0;
}
