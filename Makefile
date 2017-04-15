# Macros

CC     = g++
CFLAGS = -o

DIR_OUTPUT        = ./build
DIR_SRC           = ./src

NOMBRE_EJECUTABLE_MAIN = main
NOMBRE_EJECUTABLE_CUT  = cut
NOMBRE_EJECUTABLE_JOIN = join

all: release

# Compilación principal del ejecutable
release:
	@echo 'Compilando target: $@'
	@echo "Creando el directorio ./build"
	mkdir -p $(DIR_OUTPUT)
	@echo ' '
	@echo "####################################"
	@echo "Compilando programa principal main"
	$(CC) $(DIR_SRC)/main.cpp $(DIR_SRC)/manejoArchivo.cpp $(DIR_SRC)/manejoError.cpp $(DIR_SRC)/manejoCut.cpp $(DIR_SRC)/manejoJoin.cpp $(DIR_SRC)/pipe.cpp $(DIR_SRC)/espera.cpp  $(CFLAGS) $(DIR_OUTPUT)/$(NOMBRE_EJECUTABLE_MAIN)
	@echo "####################################"
	@echo "Programa main compilado perfectamente"
	@echo ' '
	@echo "####################################"
	@echo "Compilando programa cut"
	$(CC) $(DIR_SRC)/cut.cpp $(DIR_SRC)/manejoArchivo.cpp $(CFLAGS) $(DIR_OUTPUT)/$(NOMBRE_EJECUTABLE_CUT)
	@echo "####################################"
	@echo "Programa cut compilado perfectamente"
	@echo ' '
	@echo "####################################"
	@echo "Compilando programa join"
	$(CC) $(DIR_SRC)/join.cpp $(DIR_SRC)/manejoArchivo.cpp $(CFLAGS) $(DIR_OUTPUT)/$(NOMBRE_EJECUTABLE_JOIN)
	@echo ' '
	@echo "####################################"
	@echo "Programa join compilado perfectamente"
	@echo ' '
	@echo $@ Compilado en el directorio $(DIR_OUTPUT)
	@echo "Ingrese al directorio '"$(DIR_OUTPUT)"' haciendo: cd "$(DIR_OUTPUT)
	@echo "Ejecute el programa haciendo: ./"$(NOMBRE_EJECUTABLE_MAIN)" y escribiendo la opcion del programa adecuado"
	@echo "Ya sea --cut nombreprograma -p numeropartes o --join nombreprograma -p numeropartes"
	@echo ' '
	@echo ' '
	@echo "Si tiene problemas, en el fichero README."


clean:
	rm -rf $(DIR_OUTPUT)/*
	-@echo ' ' 