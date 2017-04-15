#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include "manejoError.hpp"

int manejoErrorEntrada(bool op_cut, bool op_join, char* nombreArchivo){

    if(op_cut && op_join){
        std::cout << "--cut y --join no pueden ser utilizadas juntas " << std::endl;
        return -1;
    }

    if(!op_cut && !op_join){
        std::cout << "Debe especificar una opcion --cut o --join " << std::endl;
        return -1;
    }

    return 0;

}

int comprobarFichero(char *nombreArchivo){
 
    if( access(nombreArchivo, F_OK) == -1){
        std::cout << " Error en el archivo, no ingreso un archivo valido" << std::endl;
        return -1;
    }

    return 0;
}

int manejoNombreFichero(char* nombreArchivo){

    if (nombreArchivo == NULL)
    {        
        std::cout << "Debe ingresar un nombre de algun archivo. " << std::endl;
        return -1;
    }
    
    return 0;
}

int contenidoFichero(char *nombreArchivo){

    FILE *fichero = NULL;

    fichero = fopen(nombreArchivo, "rb");

    if (fichero == NULL)
    {
        fclose(fichero);
        std::cout << "El fichero esta vacio. " << std::endl; 
        return -1;
    }

    return 0;
}

int manejoErrorCut(bool op_cut, bool op_parte, int numeroPartes){

    if( op_cut == true){
        
        if(numeroPartes == 0){
            std::cout << "Debe ingresar un numero valido para la opcion --cut" << std::endl;
            return -1; 
        }
        
        if(op_parte == false){
            std::cout << "Debe especificar un argumento para -p" << std::endl;
            return -1;
        }
        
    }

return 0;
}

    
