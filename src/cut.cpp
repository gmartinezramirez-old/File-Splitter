#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "cut.hpp"
#include "manejoArchivo.hpp"

#define READ 0
#define WRITE 1

/*************************************************
 * argv[0] => Nombre del programa
 * argv[1] => Nombre del fichero a leer
 * argv[2] => Inicio del archivo
 * argv[3] => Fin del archivo
 * argv[4] => Numero de hijo
 * argv[5] => Indica si somos el ultimo hijo o no
 ************************************************ */

int main(int argc, char *argv[])
{
    size_t inicio = 0, fin=0;
    int numero_hijo     = 0;
    int ultimo          = 0;
    char *nombreArchivo = NULL;
   
    //sscanf(argv[1], "%c", &nombreArchivo);
    sscanf(argv[2] , "%zu" , &inicio);     // Inicio del archivo
    sscanf(argv[3] , "%zu" , &fin);        // Fin del archivo
    sscanf(argv[4] , "%i"  , &numero_hijo); // Numero de hijos (numero de partes)
    sscanf(argv[5] , "%i"  , &ultimo);      // El ultimo
  
    // Abrir archivo original
    FILE *archivo       = fopen(argv[1], "r");

    // Archivo xor
    FILE *archivoXor    = NULL;

    // Archivo de salida es archivo.partNUMERO
    FILE *archivoSalida = fopen(nombreUnido(argv[1], numero_hijo, ".part"), "w");
   
    // tamano_file = tamaño del archivo original
    size_t tamano_file = largoFichero(argv[1]);
   
    if(ultimo == 1){
       
        /***************************************
         * Soy el ultimo hijo
         *    - Debo escribir el archivo xor
         **************************************/    
        
        const char *nombreXor = nombreConcatenar(argv[1], ".xor");
        archivoXor            = fopen((nombreXor), "w");
    }
  
    // Mover a la posicion requerida
    fseek(archivo, inicio, SEEK_SET);
   
    size_t i; 
    size_t fin2;
    
    // Verificar si hay que escribir ceros o no
    
    if(fin > tamano_file){
        
        // Debemos leer mas archivos que los que tenemos
        fin2 = tamano_file; //Tamanio del archivo
    }else{
        fin2 = fin; //Byte final del archivo
    }
  
    // fin2 indica cuanto hay que leer

    // Recorrer el archivo original, fin2 indica cuanto hay que leer antes de 
    // escribir ceros.
    for(i = inicio; i < fin2; i++){
        
        char byte_leido, pipe;
        
        // Leer 1 byte del archivo original
        fread(&byte_leido, sizeof(char), 1, archivo);
        
        if(inicio == 0){           
            
            /********************************************************
             * Soy 1er hijo
             *    - Debo solo mandar por el pipe lo leido del archivo
             ********************************************************/    
            
            // Escribir en el pipe lo leido
            write(WRITE, &byte_leido, sizeof(char));                        
        }else{

            /******************************************************************
             * No soy 1er hijo
             *     - Debo leer de un pipe y escribir lo que voy leyendo en otro
             ******************************************************************/
            
            // Leer lo del pipe
            read(READ, &pipe, sizeof(char));
           
            // Hacer Xor entre lo leido del archivo y lo leido del pipe
            pipe = pipe ^ byte_leido;
           
            // Si no existe el archivo.xor
            if(archivoXor == NULL){
                
                // Debemos enviar los datos del pipe a otro pipe
                write(WRITE, &pipe, sizeof(char));
                
            }else{
                // Existe el archivo Xor, escribir en el
                // Somos el ultimo y escribimos al archivo xor
                fwrite(&pipe, sizeof(char), 1, archivoXor);
            }
        }   
            // Escribir lo leido en archivo.partNumero
            fwrite(&byte_leido, sizeof(char), 1, archivoSalida);
    }

   /************************************
    * Soy ultimo hijo
    *    - Debo escribir a archivo.xor
    ***********************************/    

    // Si somos el ultimo hijo
    //  El archivo xor existe
    if(archivoXor != NULL){
        
        /*************************
         * Vamos a escribir ceros
         ************************/
        
        if(i < fin){
            // La cantidad de ceros que vamos a escribir es fin - i,
            // es decir recorro mas del tamanio del archivo
            // el numero_partes % tamanio_archivo != 0 (division no exacta)
            
            // La escribimos en archivo.data
            
            // Escribiendo archivo.data 
            const char *nombreArchivoData= nombreConcatenar(argv[1],".data"); 
            FILE *data = fopen(nombreArchivoData, "w");
            
            // Escibir ceros en archivo.data
            fprintf(data, "%zd", fin - i);
         
            // Cerrar archivo.data
            fclose(data);
        }
        
        for(; i < fin ; i++){
            
            // Escribiendo ceros
            
            char pipe, cero = 0;
            
            // Leer del pipe
            read(READ, &pipe, sizeof(char));
            
            // Hacer operacion xor
            pipe = pipe ^ 0;
            
            // Escribir en el archivo.xor
            fwrite(&pipe, sizeof(char), 1, archivoXor);
            
            // Escribir en archivo.partNumero
            fwrite(&cero, sizeof(char), 1, archivoSalida);
        }
    }
    
    // Cerrar archivo original
    fclose(archivo);

    // Cerrar archivo.partNumero
    fclose(archivoSalida);
  
    // Si existe archivo.xor, cerrarlo
    if(archivoXor){
        fclose(archivoXor);
    }

   return 0;
}








