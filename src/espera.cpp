#include <sys/wait.h>

#include "espera.hpp"

void espera(){
	int status, wpid;
	while( (wpid= wait(&status)) > 0 );
}
