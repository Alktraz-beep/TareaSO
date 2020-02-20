#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define PROMPT "<@#>"
#define MAX 100

void separarEnVector(char* cadena, char *array[]);
void mostrarVector(char *arreglo[100],int tam);
void inicializarVec(char *arreglo[MAX]);

int main(){

	//***crea un apuntador a char dinamico ***/
	char *array[MAX]; // vector primero donde se guarda el comando por indices	
	inicializarVec(array);
	char *comando,*salida="sal"; //en comando se guarda el comando y en salida la palabra para salir
	comando=(char*)malloc(1000*sizeof(char)+1);
	//***aqui se lee la cadena que ingresara como comando mientras no sea
	//*** la palabra "sal",despues imprime la palabra ingresada
	do{
		printf(PROMPT);
		scanf("%[^\n]",comando);
		getchar();
		printf("cadena orig: %s\n tamano:%ld \n",comando,strlen(comando));
		separarEnVector(comando,array); //llama a funcion para separar el comando
		int tam=sizeof(array)/sizeof(array[0]);
		//printf("vec 0:%s\n",array[0]);
		//printf("vec 1:%s\n",array[1]);
		//printf("vec 2:%s\n",array[2]);
		mostrarVector(array,tam);
	}while(strcmp(comando,salida)!=0);
	
	return 0;
}
void separarEnVector(char* cadena,char *array[MAX]){
	char *aux;//=(char*)0;  //cadena auxiliar donde se guardara la palabra para guardar en el vector
	int cont=0,cont2=0;  //contador para el indice del vector
	//free(aux);
	aux=(char*)malloc(1000*sizeof(char));  //*****mem dinamica del auxiliar

	//----------aqui se separa el comando por palabras en el vector--------
	while(cadena[cont]!=0){
		if(isspace(cadena[cont])==0){
			strncat(aux,&cadena[cont],1);
		}else{
			array[cont2]=aux;
			//printf("%d: %s\n",cont2,array[cont2]);
			cont2++;
			strcpy(aux,"");
		}
		cont++;
	}
	array[cont2]=aux;
//	printf("%d: %s\n",cont2,array[cont2]);
}
void mostrarVector(char *arreglo[100],int tam){
	for(int i=0;i<MAX;i++){
		printf("pos %d cad: %s\n",i,arreglo[i]);
	}
}
void inicializarVec(char *arreglo[MAX]){
	for(int i=0;i<MAX;i++){
		arreglo[i]=(char*)0;
	}
}
