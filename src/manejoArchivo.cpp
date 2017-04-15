
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <alloca.h>
#include <string>
#include <cstring>

#include "manejoArchivo.hpp"

FILE *fileManejo;

size_t largoFicheroConst(const char *filename){

    size_t tamanioArchivo = 0;
    fileManejo            = fopen(filename, "rb");

    fseek(fileManejo, 0, SEEK_END);
    tamanioArchivo = ftell(fileManejo);    
    fseek(fileManejo, 0, SEEK_SET);

    fclose(fileManejo);
    return tamanioArchivo;
}


size_t largoFichero(char *filename){

    size_t tamanioArchivo = 0;
    fileManejo            = fopen(filename, "rb");

    fseek(fileManejo, 0, SEEK_END);
    tamanioArchivo = ftell(fileManejo);    
    fseek(fileManejo, 0, SEEK_SET);

    fclose(fileManejo);
    return tamanioArchivo;
}

size_t largoPartes(size_t tamanioArchivo, size_t numeroPartes){

    size_t tamanioPartes = 0;
    tamanioPartes        = tamanioArchivo/numeroPartes;

    if(tamanioArchivo%numeroPartes != 0){
        tamanioPartes += 1;
    }

    return tamanioPartes;
}

size_t cuantoFalta(size_t tamanioArchivo, size_t tamanioPartes, size_t numeroPartes){

    size_t cuantoFalta = 0;
    cuantoFalta        = tamanioPartes*numeroPartes - tamanioArchivo;

    return cuantoFalta;
}

const char *nombreUnidoId(const char *nombreFichero, int id){

    //int to string
    std::string numeroParte;
    std::stringstream out;
    out<<id+1;
    numeroParte= out.str();

    std::string str = std::string(nombreFichero) + numeroParte;

    //Pasar a const char*
    const char *nombreFinal = str.c_str();

    return nombreFinal;
}

char *nombreUnidoChar(const char *nombreFichero, int id, std::string extension){

    //int to string 
    std::string numeroParte;
    std::stringstream out;
    out<<id+1;
    numeroParte= out.str();

    std::string parte            = extension;
    std::string nombreParteFinal = std::string(nombreFichero) + parte;
    std::string str              = nombreParteFinal           + numeroParte;
    
    //Pasar a const char*
   //const char *nombreFinal = str.c_str();
    
    //String to char
    char *nombreFinal = new char[str.size() + 1];
    memcpy(nombreFinal, str.c_str(), str.size() + 1);

    return nombreFinal; 
}

const char *nombreUnido(const char *nombreFichero, int id, std::string extension){

    //int to string 
    std::string numeroParte;
    std::stringstream out;
    out<<id+1;
    numeroParte= out.str();

    std::string parte            = extension;
    std::string nombreParteFinal = std::string(nombreFichero) + parte;
    std::string str              = nombreParteFinal           + numeroParte;
    
    //Pasar a const char*
    const char *nombreFinal = str.c_str();
    
    return nombreFinal; 
}

const char *nombreConcatenar(const char *nombreFichero, std::string extension){

    std::string str  = std::string(nombreFichero) + extension;
    
    //String to const char*
    const char *nombreFinal = str.c_str();
    
    //String to char
    //char nombreFinal = new char[str.size() + 1];
    //memcpy(nombreFinal, str.c_str(), str.size() + 1);

    return nombreFinal;

}

char *nombreConcatenarChar(const char *nombreFichero, std::string extension){

    std::string str  = std::string(nombreFichero) + extension;
    
    //String to const char*
    //const char *nombreFinal = str.c_str();
    
    //String to char
    char *nombreFinal = new char[str.size() + 1];
    memcpy(nombreFinal, str.c_str(), str.size() + 1);

    return nombreFinal;

}

void guardarBytesFaltantes(char *nombre, int falta){
    
    const char *nombre_temp = nombreConcatenar(nombre, ".tmp");
    FILE *temp              = fopen(nombre_temp, "w");
    fwrite(&falta, sizeof(int), 1, temp);

    fclose(temp);
}

/*char *nombreParteFalta(char *nombreArchivoOriginal, int numeroPartes){
    
    int i = 0;

    int numeroPartesFaltantes = 0;
    char *nombreParte         = NULL;
    char *nombreParteFaltante = NULL;

    while (i != numeroPartes){

        nombreParte   = nombreUnido(nombreArchivoOriginal, i+1, ".part");
        FILE *archivo = fopen(nombreParte, "r");

        if (archivo == NULL){
            numeroPartesFaltantes++;
            nombreParteFaltante = nombreParte;
        }else{
            fclose(archivo);
        }
        
        i++;
    }

    if( numeroPartesFaltantes == 1){
        return nombreParteFaltante;
    }else if( numeroPartesFaltantes > 1){
        return NULL;
    }else{
        return "ninguna"; // Estan todas las partes, no falta NINGUNA
    }
}

*/
