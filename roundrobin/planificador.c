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
void sacarPidTam(char* cadena,int TP[][3], int tamPags[]);
void imprimirArray(int array[][3]);
void imprimirVec(int array[numProc]);
void bubbleSort(int arr[], int n);
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
		int tamPags[numProc];
		//-----------------------------se hace leida para llenar tabla nuevamente inicializa la TP(tablade procesos)

		fp=fopen(argv[1],"r");//se abre para lectura
		if(fp==NULL){
			printf("no se pudo abrir: %s\n",argv[1]);//si no encuentra el archivo
			return 1;
		}else{
			while(fgets(str,MAXCHAR,fp)!=NULL){
					sacarPidTam(str,TP,tamPags);
				}
			tamPags[noProceso-1]=pagPorProc;//printf("%d\n",pagPorProc);//**aqui se agrega a vector
			fclose(fp);//se cierra archivo
		}
		printf("-----------\n");
		imprimirArray(TP);
		//imprimirVec(tamPags);
		bubbleSort(tamPags,numProc);
		//imprimirVec(tamPags);
		//--------------------se declara arreglo tridimensional
		int TD[numProc][tamPags[numProc-1]][2];
		//-------------------------------se hace tablas de paginas
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
			}
			bandera++;
			token=strtok(NULL,limite);
		}
}
/*-------------------------------------------------funcion que saca el pid de los procesos, y su tamaño y calcula el num de pag---------*/
void sacarPidTam(char* cadena,int TP[][3],int tamPags[]){
	char limite[]=" ";
	char* token=strtok(cadena,limite);
	int bandera=0;//es una bandera que empezara cada cadena dividida en 3, 0 es el primero, 1 es el segundo y 2 es el ultimo dato
	bool pregunta;
	//si esta bandera es 0 y el token es 0 entonces es master
	if(token!=NULL)
		while(token!=NULL){
			if(strcmp(cadena,"0")==0 && bandera==0){//si es master
				pregunta=true;
				printf("-------%d\n",noProceso);
				if(pagPorProc!=0)
					tamPags[noProceso-1]=pagPorProc;//printf("%d\n",pagPorProc);//**aqui se agrega a vector
				pagPorProc=0;
			}else if(strcmp(cadena,"1") && bandera==0)//si es 1 es una pagina
				pregunta=false;
				//busca pid y tamaño	
			if((bandera==1 && pregunta==true) || (bandera==2 && pregunta==true)){//significa que encontro un pid o tamaño
				if(bandera==1){//fue pid
					TP[noProceso][0]=atoi(token);
					//printf("%s ",token);
				}
				if(bandera==2){//fue tamaño
					TP[noProceso][1]=atoi(token);
					//printf("%s ",token);
					if(atoi(token)%l==0){
						TP[noProceso][2]=(int)(atoi(token)/l);
						//printf("%d \n",(int)(atoi(token)/l));
					}else{
						TP[noProceso][2]=(int)(atoi(token)/l)+1;
						//printf("%d \n",(int)(atoi(token)/l)+1);
					}
					noProceso++;
				}
			}
			if((bandera==1 && pregunta==false) || (bandera==2 && pregunta==false)){//significa que es pagina o desplaz
				if(bandera==1)
					printf("%s  ",token);
				if(bandera==2){
					printf("%s\n",token);
					pagPorProc++;
				}
			}
			bandera++;
			token=strtok(NULL,limite);
		}
}
//---------------funcion para imprimir arreglo----------------------------------------------------------------------------------------------
void imprimirArray(int array[][3]){
		for(int i=0;i<numProc;i++){
			for(int j=0;j<3;j++){
				printf(" %d ",array[i][j]);
			}
			printf("\n");
		}
}
//--------------funcion imprimir vector-----------------------------------------------------------------------------------------------------
void imprimirVec(int array[numProc]){
		for(int i=0;i<numProc;i++){
				printf(" %d ",array[i]);
		}
		printf("\n");
}
//------------------BUBLESORT------------------------------------------------------------------------------------------------------------
void swap(int *xp, int *yp) 
{ 
    int temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
void bubbleSort(int arr[], int n) 
{ 
   int i, j; 
   for (i = 0; i < n-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
} 















