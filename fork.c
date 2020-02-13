//Programa hecho por Josue Yafte Ramírez Viramontes y Pablo Pérez Durán
//Programa que crea dos procesos hijos a partir del padre 
//el padre  crea un vector de 10 enteros de valores entre 0-255
//hijo 1 busca la moda del vector y la envia al proceso padre
//hijo 2 busca el elemento mas grande y lo manda al proceso padre
//por ultimo el proceso padre imprime el vector con la moda y el numero mas grande

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MAX 10
//inicia la funcion que saca la moda de un arreglo
int moda(int array[]){
	//calcula la moda
	int contador=0,auxiliar[MAX], posicion=0,numero=0,contador2,mayor=0,posicionmayor=0,bandera=0;
	float suma=0;
	// se llena el auxiliar con 0
	for(contador=0;contador<MAX;contador++){
		auxiliar[contador]=0;
	}
	//Recorre el vector comprobando repeticiones
	//las almacena en el vector auxiliar segun la posicion del numero
	for(contador=0;contador<MAX;contador++){
		numero=array[contador];
		posicion=contador;
		for(contador2=contador;contador2<MAX;contador2++){
			if(array[contador2]==numero)
				auxiliar[posicion]++;
		}
	}
	/* Se establece cual es la posicion del vector que tiene el numero 
	mayor de repeticiones este corresponde a la moda 
	*/
	mayor=auxiliar[0];
	posicionmayor=0;
	for(contador=0;contador<MAX;contador++){
		if(auxiliar[contador]>mayor){
			posicionmayor=contador;
			mayor=auxiliar[contador];
		}
	}
	return array[posicionmayor];
} 
//funcion para hacer un intercambio en el arreglo
void cambiar(int *xp, int *yp){
	int temp=*xp;
	*xp=*yp;
	*yp=temp;
}
//funcion para sacar el numero mas grande de un arreglo
int mayorNumero(int array[]){
	int i,j;
	for(i=0;i<MAX-1;i++){
		for(j=0;j<MAX-1;j++){
			if(array[j]>array[j+1]){
				cambiar(&array[j],&array[j+1]);
			}
		}
	}
	return array[9];
}

void mostrarArreglo(int array[]){
	int i;
	for(i=0;i<MAX;i++){
		printf("%d-",array[i]);
	}
	printf("\n");
}


int main(){
	int pid;
	int numero=-1,estado1,pidWait1,numMayor;
	
	//--se crea arreglo de 10 numeros
	int arreglo[10]={255,0,7,130,5,130,8,7,130,130};

	pid=fork(); //se crea primer proceso 

	if(pid){ //si es diferente de 0 es padre
		int pid2;	
		pid2=fork();//segundo proceso en variable pid2
		if(pid2){ 
			//*******proceso del padre muestra el arreglo y espera a que le regresen valores sus hijos
			printf("Arreglo creado:\n");
			mostrarArreglo(arreglo);	


			printf("==padre esperando==");
			pidWait1=wait(&estado1);
			//printf("*****pid terminado:  %d",pidWait1);
			if(pidWait1==pid){ //significa que termino antes el hijo 1
				printf("***la moda es:  %d***\n\n",estado1>>8);
			}else if(pidWait1==pid2){ //terminó antes el hijo 2
				printf("***el mayor numero del arreglo es:  %d***\n",estado1>>8);
			}else{
				printf("error");
			}
			printf("==padre esperando==");


			pidWait1=wait(&estado1);
			if(pidWait1==pid){ //significa que termino antes el hijo 1
				printf("***la moda es:  %d***\n\n",estado1>>8);
			}else if(pidWait1==pid2){ //terminó antes el hijo 2
				printf("***el mayor numero del arreglo es:  %d***\n",estado1>>8);
			}else{
				printf("error");
			}
		}else{
			//********proceso del hijo 2 que regresa el numero mas grande al padre
			printf(" ejecutandose el proceso hijo 2  numero mayor %d ... \n",pid2);
			exit(mayorNumero(arreglo));
		}
	}else{ 
		//*****si es 0 es el hijo 1
		printf("ejecutandose proceso hijo 1 moda del arreglo %d...\n",pid);
		exit(moda(arreglo));
	}
	return 0;
}


