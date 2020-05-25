#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#define MAXCHAR 1000
/*-----------------------------variables globales----*/
int numProc=0;
int pagPorProc=0;
/*------------------------------funciones-------*/
void separarCadena(char* cadena);
/*-------------------------------------------------------funcion main-----------------------------------------------------------------*/
int main(int argc,char *argv[]){
	//en la variable argv[1] se encuentra el nombre del archivo.txt	
	char str[MAXCHAR];//aqui se almacenan las palabras
	if(argc!=2)
		printf("necesita un argumento que es el nombre del archivo con extension .txt\n");
	else{
	
		FILE *fp;//el archivo
		//--primer lectura saca numero de procesos------------------
		fp=fopen(argv[1],"r");//se abre para lectura
		if(fp==NULL){
			printf("no se pudo abrir: %s\n",argv[1]);//si no encuentra el archivo
			return 1;
		}else{
			while(fgets(str,MAXCHAR,fp)!=NULL){
					//printf("%s",str);//aqui se lee cada linea en caso de que el archivo si se pueda leer
					separarCadena(str);
				}
			fclose(fp);//se cierra archivo
		}
		//--------se se crea tabla de procesos
		int TP[numProc][3];
		//-------se hace leida para llenar tabla nuevamente
		fp=fopen(argv[1],"r");//se abre para lectura
		if(fp==NULL){
			printf("no se pudo abrir: %s\n",argv[1]);//si no encuentra el archivo
			return 1;
		}else{
			while(fgets(str,MAXCHAR,fp)!=NULL){
					//llenar tabla(str);
				}
			fclose(fp);//se cierra archivo
		}
	}

	return 0;
}

/*------------------------------------------------funcion  que saca cuantos procesos hay----------------------------------------------*/


/*------------------------------------------------funcion  que hace tabla de procesos----------------------------------------------*/
/*------------------------------------------------funcion  que calcula paginas----------------------------------------------*/
/*------------------------------------------------funcion  que guarda proceso en tabla----------------------------------------------*/
/*------------------------------------------------funcion  que divide cadena y quita espacios----------------------------------------------*/
void separarCadena(char* cadena){
	char limite[]=" ";
	char* token=strtok(cadena,limite);
	int bandera=0;//es una bandera que empezara cada cadena dividida en 3, 0 es el primero, 1 es el segundo y 2 es el ultimo dato
	bool pregunta;
	
	//si esta bandera es 0 y el token es 0 entonces es master
	if(token!=NULL)
		while(token!=NULL){
			//printf("%s\n",token);
			if(strcmp(cadena,"0")==0 && bandera==0){//si es master
				pregunta=true;
				numProc++;
				//printf("esto es una proceso\n");
			}/*else if(strcmp(cadena,"1")==0 && bandera==0){
				pagPorProc++;
				//printf("esto es una pagina\n");
			}*/


				/*if(strcmp(cadena,"1")==0 && bandera==0){//es pagina
				pregunta=false;
				numPag++;
			}
			if(bandera==1 && pregunta)//de aqui salen los pid 
				printf("pid: %s\n",token);
			if(bandera==2 && pregunta)//de aqui salen los tamaños
				printf("tamano: %s\n",token);*/
			bandera++;
			token=strtok(NULL,limite);
		}
}


