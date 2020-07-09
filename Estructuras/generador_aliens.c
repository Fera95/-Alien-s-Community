#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h> 
#include <string.h> 
#include "constants.h"
#include "lista.h"
#include "generador_aliens.h"


/** Lee las variables de un puente de un archivo .conf 
 * 
 * const char* conf_path: path al archivo
 * retorna: una estructura  configpuente_t con la informacion del archivo
 */
configpuente_t get_config_puente(const char* conf_path){
    configpuente_t conf;
    FILE* file = fopen(conf_path, "r");
    if(file==NULL){
        printf ("Path to config not avaliable. \n");
        exit(EXIT_FAILURE); 
    }
    char line[256];
    char prev[256];
    while (fgets(line, sizeof(line), file)) {
        char* current = strtok (line, "=:");
        //prev={0};
        while (current) {
            if(!strcmp(prev,"puenteID")){
                conf.bandID = atoi(current);
            }
            else if(!strcmp(prev,"puenteTotalAliens")){
                conf.bandStrength = atoi(current);
            }
            else if(!strcmp(prev,"puenteTotalAliens_turno")){
                conf.bandLength = atoi(current);
            }            
			else if(!strcmp(prev,"puentePesoPermitido")){
                conf.bandDistro = atoi(current);
            }
			else if(!strcmp(prev,"puenteAlgoritmo")){
                conf.bandMean = atoi(current);
            }
			else if(!strcmp(prev,"bandStdDev")){
                conf.bandStdDev = atoi(current);
            }
			else if(!strcmp(prev,"packageRadsP")){
                conf.packageRadsP = atoi(current);
            }
			else if(!strcmp(prev,"packageUrgeP")){
                conf.packageUrgeP = atoi(current);
            }
			else if(!strcmp(prev,"packageLeftP")){
                conf.packageLeftP = atoi(current);
            }
			else if(!strcmp(prev,"bandQuantum")){
                conf.bandQuantum = atoi(current);
            }
			else if(!strcmp(prev,"bandParameter")){
                conf.bandParameter = atoi(current);
            }
			else if(!strcmp(prev,"bandAlgorithm")){
                conf.bandAlgorithm = atoi(current);
            }
			else if(!strcmp(prev,"bandScheduler")){
                conf.bandScheduler = atoi(current);
            }
            strcpy(prev, current);
            current = strtok (NULL, "=:");
        }
    }
    fclose(file);
    return conf;
} 

