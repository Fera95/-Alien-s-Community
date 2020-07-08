#ifndef GENERADOR_ALIENS
#define GENERADOR_ALIENS

#include "lista.h"


// Se lee de un archivo de configuración de puente
configpuente_t get_config_puente(int puenteID);

// Se lee de un archivo de configuracion de espera 
configespera_t get_config_espera(int esperaID);

// se crea un numero pseudo aleatorio siguiendo una distro
int randNum (int mean, int stdDev, int distro);

// se crea un alien y se aumente al numero de contadores
void createAlien(int* AlienCounter, alien_t* newAlien, int ListaId);

// Se imprimer los valores del alien
void checkAlien(alien_t* testAlien);

// Se printea la struct de la config de puente
void checkConfigPuente(configpuente_t file);

// Se printea la struct de la config de espera
void checkConfigEspera(configespera_t file);


#endif // PACKAGE_GENERATOR