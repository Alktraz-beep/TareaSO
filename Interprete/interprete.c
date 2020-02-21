/*
Programa que realiza el shell de linux, ejecutando los comandos con execvp 
por medio de vector
Programa realizado por @Josue Yafte Ramírez Viramontes y @Juan Pablo Pérez Dublán
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/wait.h>
#define PROMPT "<@#>"
#define MAX 1000

void separarEnVector(char* cadena, char *array[]);
void mostrarVector(char *arreglo[],int tam);
void inicializarVec(char *arreglo[],int tam);
void vaciarVector(char *arreglo[]);//tal vez nunca se utilice
int vectorSize(char* cadena);
int hayPipe(char *cadena);
void separarCadena(char *vec1[], char *vec2[],char *comando);

int main(){

	/*                   declaracion de
				variables relacionadas a procesos
	*/
	int pid,estado;
	
	
	//********************crea un apuntador a char dinamico ***/

	char *comando,*salida="sal"; //en comando se guarda el comando y en salida la palabra para salir
	comando=(char*)malloc(MAX*sizeof(char)+1);

	int sal=0;//esta variable verifica si aun no ha salido es 0 y si ya puso "sal' es 1
	//aqui se lee la cadena que ingresara como comando mientras no sea
	//la palabra "sal",despues imprime la palabra ingresada

	do{
		printf(PROMPT);
		scanf("%[^\n]",comando);
		getchar();
		//printf("cadena orig: %s\n tamano:%ld \n",comando,strlen(comando));
		pid=fork();
		//Aqui se empieza el hijo que es el que ejecuta el codigo
		//printf("tamaño del vector: %d\n",tamVec);

		if(pid){
			wait(&estado);
			sal=estado>>8;
			printf("salida= %d\n",sal);
		}else{
			//--------aqui se verifica si el comando tiene pipe
			//--------si lo tiene separa en dos vectores sin el pipe
			if(hayPipe(comando)){
				//se crean dos cadenas para separar en dos vectores
				char *array1[10]; // vector primero donde se guarda el comando por indices	
				char *array2[10]; // vector primero donde se guarda el comando por indices	
				printf("tiene pipe\n");
				separarCadena(array1,array2,comando);
				exit(0);
			}else{
				int tamVec=vectorSize(comando)+1;//se le suma 1 para el null del final
				char *array[tamVec]; // vector primero donde se guarda el comando por indices	
				inicializarVec(array,tamVec);// se inicializa el vector para que no quede basura al siguiente comando
				separarEnVector(comando,array); //llama a funcion para separar el comando por espacio

				if(strcmp(array[0],salida)==0){
					exit((int)1);
				}
				execvp(array[0],array);
			}
			
			//mostrarVector(array,tamVec);
		}
		
	}while(sal==0);
	
	return 0;
}



/*
Funcion que permite sacar el tamaño que deberia tener el array de palabras del comando
*/

int vectorSize(char* cadena){
	char *aux;//=(char*)0;  //cadena auxiliar donde se guardara la palabra para guardar en el vector
	int cont=0,cont2=0;  //contador para el indice del vector
	aux=(char*)malloc(1000*sizeof(char));  //*****mem dinamica del auxiliar

	//----------aqui se separa el comando por palabras en el vector--------

	while(cadena[cont]!=0){
		if(isspace(cadena[cont])==0 && cadena[cont]!='\0'){
			strncat(aux,&cadena[cont],1);
		}else{
			cont2++;
			strcpy(aux,"");
		}
		cont++;
	}
	cont2++;
	return cont2;
}
/*
Funcion que permite separar el comando que se le da "cadena" en un vector array de cadenas
*/
void separarEnVector(char* cadena,char *array[]){
	char *aux;//=(char*)0;  //cadena auxiliar donde se guardara la palabra para guardar en el vector
	int cont=0,cont2=0;  //contador para el indice del vector
	//free(aux);
	aux=(char*)malloc(1000*sizeof(char));  //*****mem dinamica del auxiliar

	//----------aqui se separa el comando por palabras en el vector--------
	while(cadena[cont]!=0){

		if(isspace(cadena[cont])==0 && cadena[cont]!='\0'){
			strncat(aux,&cadena[cont],1);
		}else{
			array[cont2]=strdup(aux);
			cont2++;
			strcpy(aux,"");
		}
		cont++;
	}
	array[cont2]=strdup(aux);
}

/*
Funcion que permite mostrar un vectorde cadenas usado para comprobar si si se estan metiendo
los datos correctos 
*/
void mostrarVector(char *arreglo[],int tam){
	for(int i=0;i<tam;i++){
		printf("pos %d cad: %s\n",i,arreglo[i]);
	}
}
/*
funcion que inicializa el vector con null para vaciar la cadena
*/
void inicializarVec(char *arreglo[],int tam){
	for(int i=0;i<tam;i++){
		arreglo[i]=(char*)0;
	}
}
/*
funcion que permite vaciar el vector de cadenas
*/
void vaciarVector(char *arreglo[MAX]){
	for(int i=0;i<MAX;i++){
		free(arreglo[i]);
	}
}
/*
Funcion que permite buscar si tiene pipe "|" la funcion
*/
int hayPipe(char *cadena){
	int haypipe=0,i=0;//si no tiene pipe es 0 y si tiene es 1, i es un contador
	while(cadena[i]!=0){
		if(cadena[i]=='|')
			haypipe=1;
		i++;
	}
	return haypipe;
}
/*
Funcion que separa una cadena en dos 
*/
void separarCadena(char *vec1[], char *vec2[],char *comando){
	int i=0;
	//se hacen dos cadenas para despues separar en dos vectores cada cadena
	char* comando1;	
	char* comando2;	
	comando1=(char*)malloc(MAX*sizeof(char)+1);
	comando2=(char*)malloc(MAX*sizeof(char)+1);
	while(comando[i]!=0){
		if(comando[i]!='|'){
			strncat(comando1,&comando[i],1);
		}else{
			break;	
		}
		i++;
	}
	while(comando[i]!=0){
		i++;
		strncat(comando2,&comando[i],1);
	}
	//printf("cadena1: %s\n",comando1);
	//printf("cadena2: %s\n",comando2);
}
