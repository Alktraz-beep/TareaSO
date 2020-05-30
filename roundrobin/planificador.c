#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#define MAXCHAR 1000
/*-----------------------------variables globales----*/
int numProc=0;
int noProceso=0;//auxiliar para segunda vuelta
int pagPorProc=0;
int l=20;//tamaño de pagina de los marcos
/*------------------------------funciones-------*/
void separarCadena(char* cadena);
void sacarPidTam(char* cadena,int TP[][3]);
/*-------------------------------------------------------funcion main-----------------------------------------------------------------*/
int main(int argc,char *argv[]){
	//en la variable argv[1] se encuentra el nombre del archivo.txt	
	char str[MAXCHAR];//aqui se almacenan las palabras
	if(argc!=2)
		printf("necesita un argumento que es el nombre del archivo con extension .txt\n");
	else{
	
		FILE *fp;//el archivo
		//--primer lectura saca numero de procesos-----------------------------------------------------
		fp=fopen(argv[1],"r");//se abre para lectura
		if(fp==NULL){
			printf("no se pudo abrir: :(%s\n",argv[1]);//si no encuentra el archivo
			return 1;
		}else{
			while(fgets(str,MAXCHAR,fp)!=NULL){
					//printf("%s",str);//aqui se lee cada linea en caso de que el archivo si se pueda leer
					separarCadena(str);
				}
			fclose(fp);//se cierra archivo
		}

		//--------se se crea tabla de procesos
		printf("numero de procesos:%d\n",numProc);
		int TP[numProc][3];
		//-------se hace leida para llenar tabla nuevamente

		fp=fopen(argv[1],"r");//se abre para lectura
		if(fp==NULL){
			printf("no se pudo abrir: %s\n",argv[1]);//si no encuentra el archivo
			return 1;
		}else{
			while(fgets(str,MAXCHAR,fp)!=NULL){
					sacarPidTam(str,TP);
				}
			fclose(fp);//se cierra archivo
		}
	}

	return 0;
}

/*------------------------------------------------funcion  que saca cuantos procesos hay----------------------------------------------*/
void separarCadena(char* cadena){
	char limite[]=" ";
	char* token=strtok(cadena,limite);
	int bandera=0;//es una bandera que empezara cada cadena dividida en 3, 0 es el primero, 1 es el segundo y 2 es el ultimo dato
	bool pregunta;
	
	//si esta bandera es 0 y el token es 0 entonces es master
	if(token!=NULL)
		while(token!=NULL){
			if(strcmp(cadena,"0")==0 && bandera==0){//si es master
				pregunta=true;
				numProc++;
				//printf("esto es una proceso\n");
			}
			bandera++;
			token=strtok(NULL,limite);
		}
}
/*-------------------------------------------------funcion que saca el pid de los procesos, y su tamaño y calcula el num de pag---------*/
void sacarPidTam(char* cadena,int TP[][3]){
	char limite[]=" ";
	char* token=strtok(cadena,limite);
	int bandera=0;//es una bandera que empezara cada cadena dividida en 3, 0 es el primero, 1 es el segundo y 2 es el ultimo dato
	bool pregunta;
	//si esta bandera es 0 y el token es 0 entonces es master
	if(token!=NULL)
		while(token!=NULL){
			if(strcmp(cadena,"0")==0 && bandera==0){//si es master
				pregunta=true;
			}else if(strcmp(cadena,"1") && bandera==0)//si es 1 es una pagina
				pregunta=false;
			
			if((bandera==1 && pregunta==true) || (bandera==2 && pregunta==true)){//significa que encontro un pid o tamaño
				if(bandera==1)//fue pid
					TP[noProceso][0]=atoi(token);
					//printf("%s ",token);
				if(bandera==2){//fue tamaño
					TP[noProceso][1]=atoi(token);
					if(atoi(token)%l==0)
						TP[noProceso][2]=(int)(atoi(token)/l);
					else
						TP[noProceso][2]=(int)(atoi(token)/l)+1;
					//printf("%s ",token);
					//printf("%i \n",(int)(atoi(token)/l)+1);
					noProceso++;
				}
			}
			bandera++;
			token=strtok(NULL,limite);
		}
}

