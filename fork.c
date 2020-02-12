//Programa hecho por Josue Yafte Ramírez Viramontes y Pablo Pérez Durán
//Programa que crea dos procesos hijos a partir del padre 
//el padre  crea un vector de 10 enteros de valores entre 0-255
//hijo 1 busca la moda del vector y la envia al proceso padre
//hijo 2 busca el elemento mas grande y lo manda al proceso padre
//por ultimo el proceso padre imprime el vector con la moda y el numero mas grande

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//inicia la funcion de la moda de un arreglo
void moda(){
} 
//funcion para sacar el numero mas grande de un arreglo
void mayorNumero(){
}

int main(){
	int pid;
	int numero=-1;
	
	//--se crea arreglo de 10 numeros
	int arreglo[10]={60,201,7,1,108,7,50,7,9,7};
		

	pid=fork(); //se crea primer proceso 

	if(pid){ //si es diferente de 0 es padre
		int pid2;	
		pid2=fork();//segundo proceso en variable pid2
		if(pid2!=0){ 
			//*******proceso del padre
		}else{
			//********proceso del hijo 2
			printf(" ejecutandose el proceso hijo 2  numero mayor: \n");
		}
	}else{ 
		//*****si es 0 es el hijo 1
		printf("ejecutandose proceso hijo 1 moda del arreglo:\n");
	}
	return 0;
}


