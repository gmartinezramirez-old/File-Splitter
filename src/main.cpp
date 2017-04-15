#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include "main.hpp"
#include "manejoArchivo.hpp"
#include "manejoError.hpp"
#include "manejoCut.hpp"
#include "manejoJoin.hpp"
#include "espera.hpp"


/*********************************************************************
 * Estructura que recibe la funcion getopt_long
 * 1er String => Nombre de la opcion larga
 * 2do String => Indica si necesita argumento, 0 No, 1 Si 
 * 3er String => Como se devuelven los valores, 0 devuelve el 4to String
 * 4to String => Valor que se devuelve o la que carga la 3era variable
 *********************************************************************/


struct option long_options[] =
         {
           {"cut"  , 1 , 0 , 'c'} , 
           {"join" , 1 , 0 , 'j'} , 
           {"part" , 1 , 0 , 'p'} , 
           {0      , 0 , 0 , 0}
         };


int main(int argc, char *argv[])
{

    bool op_cut=false, op_parte=false, op_join=false;
    char *nombreArchivo = NULL;
    int   numeroPartes  = 0;
    char c              = 1;
 
    while (c){
        
        int option_index = 0;
     
        c = getopt_long (argc, argv, "cjp:", long_options, &option_index);
        
        switch(c){
            case 'c':
                //cut
                op_cut        = true;
                nombreArchivo = optarg;
                break;
            
            case'j':
                //join
                op_join       = true;
                nombreArchivo = optarg;
                break;
            
            case 'p':
                //partes
                op_parte     = true;
                numeroPartes = atoi(optarg);
                break;
            
            case '?':    
                if( optopt == 'c' ){
                    fprintf (stderr, "Opcion --cut requiere un argumento.\n");
                }
                break;
            
            default:
                std::cout <<" " << std::endl;
                c=0;
        }
    }

    /**************************************
     * Manejo de errores entrada de archivo
     ***************************************/

    if( nombreArchivo==NULL){
        return -1;
    }

    // comprobar fichero solo en la opcion --cut
    if( op_cut && comprobarFichero(nombreArchivo) == -1){
        return -1;
    }

    // Verificar si se ingreso un nombre valido
    if( manejoNombreFichero(nombreArchivo) == -1){
        return -1;
    }

    // Verificar integridad del contenido del fichero solo en la opcion --cut
    if( op_cut && contenidoFichero(nombreArchivo) == -1){
        return -1;
    }

    //No se pueden ocupar ambas opciones a la vez
    if(manejoErrorEntrada(op_cut, op_join, nombreArchivo) == -1){
        return -1;
    }
    
    /**************************************
     * Manejo de errores de la entrada cut
     **************************************/

    if( manejoErrorCut(op_cut, op_parte, numeroPartes) == -1 ){
        return -1;
    }

    /**************************************
     * Manejo de errores en la entrada join
     * ************************************/

    /* Si no se especifican cuantas partes unir para join
     * sacar numero de partes de un archivo de texto
     *
     */

    // Primero se guardara el numero de partes pasado por cut

   /* if( (op_cut) && (numeroPartes != 0)){
        
        char *nombreGuardarNumero = nombreConcatenarChar(nombreArchivo, ".tmp");
        FILE *archivoTemp = fopen(nombreGuardarNumero, "w");
        
    }
    */

    // Opcion --cut : Ejecutar programa cut
    if(op_cut){
            
        if(cut(nombreArchivo, numeroPartes) != 0){
            std::cout << "Ocurrieron errores al cortar el archivo" << std::endl;
        }
    }

   // Opcion --join : Ejecutar programa join 
   if(op_join){
            
        if(join(nombreArchivo, numeroPartes) != 0){
            std::cout << "Ocurrieron errores al tratar de unir los archivos" << std::endl;        
        }
    }
    
    // Esperar a que los hijos terminen 
    espera();

    return 0;
    
}
