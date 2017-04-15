#ifndef _H_MANEJOERRORENTRADA_
#define _H_MANEJOERRORENTRADA_

int manejoErrorEntrada(bool op_cut, bool op_join, char* nombreArchivo);
int manejoNombreFichero(char* nombreArchivo);
int contenidoFichero(char *nombreArchivo);
int comprobarFichero(char* nombreArchivo);
int manejoErrorCut(bool op_cut, bool op_parte, int numeroPartes);

#endif
