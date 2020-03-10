/*
	Programa Hecho por Josue Yafte Ramírez Viramontes y Juan Pablo Pérez Dublán
*/
#include<stdio.h>
#include<pthread.h>
int proc_favorecido, proc1deseaEntrar, proc2deseaEntrar;
//------------proceso 1
void *deposito_HiloSecundario1(){
	while(1){
	}
}


//------------proceso 2
void *deposito_HiloSecundario2(){
	while(1){
	}
}
int main(){
	// aqui va el algoritmo de crear los hios en 0 en paralelo
	proc1deseaEntrar=0;
	proc2deseaEntrar=0;
	int error;
	pthread_t hilo1,hilo2;
	return 0;	
}
