/*
	Programa Hecho por Josue Yafte Ramírez Viramontes y Juan Pablo Pérez Dublán
*/
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
int proc_favorecido, proc1deseaEntrar, proc2deseaEntrar;
//--------otras tareas
void otrasTareas1(){
	sleep(2);
	printf("Hilo1 haciendo otras tareas...\n");
}
void otrasTareas2(){
	sleep(2);
	printf("Hilo2 haciendo otras tareas...\n");
}
//------------proceso 1
void *deposito_HiloSecundario1(){
	while(1){
		proc1deseaEntrar=1;
		proc_favorecido=2;
		while(proc2deseaEntrar && proc_favorecido==2);
		//aqui va la operacion de la region critica
		printf("hilo1 en region critica...\n");
		fflush(stdout);
		proc1deseaEntrar=0;
		otrasTareas1();
		fflush(stdout);
	}
}


//------------proceso 2
void *retiro_HiloSecundario2(){
	while(1){
	}
}
int main(){
	// aqui va el algoritmo de crear los hios en 0 en paralelo
	proc1deseaEntrar=0;
	proc2deseaEntrar=0;
	int error;
	pthread_t hilo1,hilo2;
	//-------creacion de hilos
	error=pthread_create(&hilo1,NULL,deposito_HiloSecundario1,NULL);
	error=pthread_create(&hilo2,NULL,retiro_HiloSecundario2,NULL);
	//----Uniones de hilos
	error=pthread_join(hilo1,NULL);
	error=pthread_join(hilo2,NULL);

	return 0;	
}
