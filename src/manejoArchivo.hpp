#ifndef _H_MANEJOARCHIVO_
#define _H_MANEJOARCHIVO_

#include <iostream>

size_t largoFicheroConst(const char* filename);
size_t largoFichero(char *filename);
size_t largoPartes(size_t tamanioArchivo, size_t numeroPartes);
size_t cuantoFalta(size_t tamanioArchivo, size_t tamanioPartes, size_t numeroPartes);
int manejoErrores(char *filename, int numeroPartes);
const char *nombreUnido(const char *nombreFichero, int id, std::string extension);
char *nombreUnidoChar(const char *nombreFichero, int id, std::string extension);
const char *nombreUnidoId(const char*nombreFichero, int id);
const char *nombreConcatenar(const char *nombreFichero, std::string extension);
void guardarBytesFaltantes(char *nombre, int falta);
char *nombreConcatenarChar(const char *nombreFichero, std::string extension);


#endif
