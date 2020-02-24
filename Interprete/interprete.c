/*
Programa que realiza el shell de linux, ejecutando los comandos con execvp 
por medio de vector si no tiene pipe, si tiene pipe se hacen dos vectores y se crea hijo nieto
para salir del ejecutar se utiliza el comando "sal" tantas veces como sea posible
Programa realizado por @Josue Yafte Ramírez Viramontes y @Juan Pablo Pérez Dublán
realizado el 21/02/2020
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/wait.h>
#define PROMPT "<Josue@Pablo#>"
#define MAX 1000

void separarEnVector(char* cadena, char *array[],char *delimitador);
void mostrarVector(char *arreglo[],int tam);
void inicializarVec(char *arreglo[],int tam);
int vectorSize(char* cadena);
int hayPipe(char *cadena);
void quitarEspacios(char *cadena);
char* separarCadena(char *comando);

int main(){

	/*                   declaracion de
				variables relacionadas a procesos
	*/
	int pid,estado;
	

	//********************crea un apuntador a char dinamico ***/

	char *comando,*salida="salir"; //en comando se guarda el comando y en salida la palabra para salir
	comando=(char*)malloc(MAX*sizeof(char)+1);

	int sal=0;//esta variable verifica si aun no ha salido es 0 y si ya puso "salir' es 2
	//aqui se lee la cadena que ingresara como comando mientras no sea
	//la palabra "salir",despues imprime la palabra ingresada

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
		}else{
			//--------aqui se verifica si el comando tiene pipe
			//--------si lo tiene separa en dos vectores sin el pipe
			if(hayPipe(comando)==1){
				int pidNieto,pipefd[2];
			//*******************ESTA PARTE DEL CODIGO CREA DOS VECTORES CON LOS DOS CODIGOS**********
				char *comando2=(char*)malloc(MAX*sizeof(char)+1);
				//se crean dos cadenas para separar en dos vectores
				strcpy(comando2,separarCadena(comando));
				int tamVec1=vectorSize(comando)+1;
				int tamVec2=vectorSize(comando2)+1;
				char *array1[tamVec1]; // <--------vector primero donde se guarda el comando por indices	
				inicializarVec(array1,tamVec1);// se inicializa el vector para que no quede basura al siguiente comando
				char *array2[tamVec2]; // <--------vector segundo donde se guarda el comando por indices	
				inicializarVec(array2,tamVec2);// se inicializa el vector para que no quede basura al siguiente comando
				separarEnVector(comando,array1," ");
				separarEnVector(comando2,array2," ");
			//***************************************************************************************
				pipe(pipefd);//nos ayuda a generar comunicacion entre las padre e hijo
				pidNieto=fork();
				if(pidNieto){
					close(0);
					close(pipefd[1]);
					dup(pipefd[0]);
					if(execvp(array2[0],array2)==-1){
						printf("Comando invalido!\n");//lanza -1 si el argumeno no se reconoce
						fflush(stdout);
						exit(0);
					}
				}else{
					close(1);
					close(pipefd[0]);
					dup(pipefd[1]);
					if(execvp(array1[0],array1)==-1){ //lanza -1 si el argumento no se reconoce
						printf("Comando invalido!\n");//mensaje que lanza
						fflush(stdout);
						exit(0);
					}
				}
				exit(8);
			}else if(hayPipe(comando)==0){
				quitarEspacios(comando);
				int tamVec=vectorSize(comando)+1;//se le suma 1 para el null del final
				char *array[tamVec]; // vector primero donde se guarda el comando por indices	
				inicializarVec(array,tamVec);// se inicializa el vector para que no quede basura al siguiente comando
				separarEnVector(comando,array," "); //llama a funcion para separar el comando por espacio

				if(strcmp(array[0],salida)==0){
					exit(2);
				}
				if(execvp(array[0],array)==-1){//lanza -1 si no se reconoce
					printf("comando invalido\n");
					fflush(stdout);
					exit(0);
				}

			}else{
				printf("Solo se permite un pipe\n");
			}
			
		}
		
	}while(sal!=2);
	
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

void separarEnVector(char* cadena,char *array[],char *delimitador){
	char *token=strtok(cadena,delimitador);
	int cont=0;
	while(token!=NULL){
		array[cont]=strdup(token);
		token=strtok(NULL,delimitador);
		cont++;
	}
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
Funcion que permite buscar si tiene pipe "|" la funcion
*/
int hayPipe(char *cadena){
	int haypipe=0,i=0;//si no tiene pipe es 0 y si tiene es 1, i es un contador
	while(cadena[i]!=0){
		if(cadena[i]=='|')
			haypipe++;
		i++;
	}
	return haypipe;
}
/*
Funcion que quita los primeros espacios de la funcion
*/
void quitarEspacios(char *cadena){
	int i=0;
	char *aux2;
	aux2=(char*)malloc(MAX*sizeof(char));  //*****mem dinamica del auxiliar
	if(isspace(cadena[0])==0){
	}else{
		while(isspace(cadena[i])!=0){
			i++;
		}
		for(int j=i;cadena[j]!='\0';j++){
			strncat(aux2,&cadena[j],1);
		}
		strcpy(cadena,aux2);
	}
}
/*
Funcion que separa una cadena en dos 
*/
char* separarCadena(char *comando){
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
	strcpy(comando,comando1);
	return comando2;
}
/*
funcion que quita espacios del vector
*/

//****************conclusiones*******************
/*Josue Yafte Ramírez Viramontes:
	En este programa realizado pude darme cuenta de como es que el shell de linux hace para ejecutar los comandos que
	normalmente utilizamos en la terminal para programar y tambien para hacer tareas basicas,
	aunque esta forma fue por medio de fork() y de las pipes que vimos en clase fue muy interesante y bastante
	arduo hacer el programa ya que fueron bastantes lineas de comandos y funciones tan solo
	para que pueda hacer lo que el Shell de modo que interprete las llamadas al sistema, son aspectos interesantes
	en la materia donde aprendi como es la importancia de el sistema operativo y de su interprete de comandos
*/
/*Juan Pablo Pérez Dublán
	Para finalizar esta practica cabe destacar que aunque a mi parecer fue un programa  complicado de realizar siento que
	fue muy útil para ciertos ascpetos, uno de ellos fue el de poner en práctica nuestros conocimientos sobre las
	llamadas al sistema, y el como interactucan entre ellas,pues tuvimos que usar todas las llamadas al sistema que hemos
	visto durante el curso  por otro lado el realizar el shell me sirivo para darmecuenta de como es que linux ejecuta
	los comandos en la terminal.
*/

