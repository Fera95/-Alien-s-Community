#include <stdio.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>




#define NTHREADS 9
void *thread_function(void *);


//Banderas para eventos
int  crear1 = 0;
int  crear2 =0;
int crear3 =0;

//argumentos para 2do nivel(funciones creadoreas)
struct args {
   pthread_t idB;
   int numero;
};
//argumentos para 3er nivel(funcion que printea)
struct args2 {
    char* creador;
    int numero;
};

//funcion que printea
void *thread_function(void *input)
{  
   int a = ((struct args2*)input)->numero;
  
   printf("Mis datos de alien son los siguientes: \n");
   printf("Numero (indice): %d\n", a);
   printf("Creador: %s\n", ((struct args2*)input)->creador);
   free(input);
}

//funciones que crean hilos que tienen funciones que printean.
void *crearhilo1(void *input){

while(1){
 if(crear1){
 pthread_t x = ((struct args*)input)->idB ;
 int num = ((struct args*)input)->numero;

 int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }

        *arg = num;

 struct args2 *datos2_1 = (struct args2 *)malloc(sizeof(struct args2));
 datos2_1->creador =  "Creador1";
 datos2_1->numero = *arg;
printf("Se dio evento1 creando alien tipo1 \n");
  free(input);
 pthread_create( &x, NULL, thread_function, (void *)datos2_1);
 crear1 =0;

 }
 else{
    printf(".\n");
 }
 
 sleep(3);
}
}

void *crearhilo2(void *input){

while(1){
 if(crear2){
 pthread_t x = ((struct args*)input)->idB ;
 int num = ((struct args*)input)->numero;

 int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }

        *arg = num;

 struct args2 *datos2_2 = (struct args2 *)malloc(sizeof(struct args2));
 datos2_2->creador =  "Creador2";
 datos2_2->numero = num;
printf("Se dio evento2 creando alien tipo2 \n");
  free(input);
 pthread_create( &x, NULL, thread_function, (void *)datos2_2);
  crear2 =0;
 
 }
 else{
    printf(".\n");
 }

  sleep(3);
}
}

void *crearhilo3(void *input){

while(1){
 if(crear3){
 pthread_t x = ((struct args*)input)->idB ;
 int num = ((struct args*)input)->numero;

 int *arg = malloc(sizeof(*arg));
        if ( arg == NULL ) {
            fprintf(stderr, "Couldn't allocate memory for thread arg.\n");
            exit(EXIT_FAILURE);
        }

        *arg = num;

 struct args2 *datos2_3 = (struct args2 *)malloc(sizeof(struct args2));
 datos2_3->creador =  "Creador3";
 datos2_3->numero = num;
 printf("Se dio evento3 creando alien tipo3 \n");
 free(input);
 pthread_create( &x, NULL, thread_function, (void *)datos2_3);
 crear3 =0;

 }
 else{
    printf(".\n");
 }
 
  sleep(3);
}
}



//main de la progra corriendop

int main()
{
   while(1){

  
   int i, j;

   pthread_t thread_id[NTHREADS];
   pthread_t thread_id2[NTHREADS];

   
   for(i=0; i < NTHREADS; i++)
   {

       struct args *datos = (struct args *)malloc(sizeof(struct args));
   
   
         
      
      //3 hilos de creacion 
      
     pthread_create( &thread_id[0], NULL, crearhilo1, (void *)datos);
     pthread_create( &thread_id[1], NULL, crearhilo2, (void *)datos);   
     pthread_create( &thread_id[2], NULL, crearhilo3, (void *)datos); 

     
      //condiciones (ya sea al azar o teclas);
         printf("Voy por la iteracion %d \n",i);
         
         if(i<=2){
         datos->idB =  thread_id2[i];
         int numeral = i;
         datos->numero = numeral;
         crear1 =1 ;
         
            
         }
         if(i>2 && i<=5){
             datos->idB =  thread_id2[i];
             int numeral = i;
            datos->numero = numeral;
             crear2 =1 ;
             
         }

         if(i>5){
            datos->idB =  thread_id2[i];
             int numeral = i;
            datos->numero = numeral;
            crear3 =1 ;
              
         }
       
       sleep(5);
   }
    }

}

