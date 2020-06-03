#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#define MAXCHAR 1000
/*-----------------------------variables globales----*/
int numProc=0;
int noProceso=0;//auxiliar para segunda vuelta
int pagPorProc=0;
int maxPaginas=0;//contiente el maximo de paginas por procesos
int l=20;//tamaño de pagina de los marcos
int quantum=4;
int marcosPagina=5;
/*------------------------------funciones para el llenado de tablas-------*/
void separarCadena(char* cadena);
void sacarPidTam(char* cadena,int TP[][3], int tamPags[]);
void imprimirArray(int array[][3]);
void imprimirVec(int array[numProc]);
void bubbleSort(int arr[], int n);
void sacarPaginaDesplazamiento(char* cadena,int TD[numProc][maxPaginas][2]);
void imprimirArray3D(int array[numProc][maxPaginas][2],int proc);
void basura(int array[numProc][maxPaginas][2]);
void eliminar(int cola[numProc],int elemento);
void iniciarMemoria(int MI[marcosPagina][4],int TD[numProc][maxPaginas][2],int TP [numProc][3]);
void imprimirMemoria(int array[][4]);
/*-----------------------------funciones para algoritmo-------------------*/
bool colavacia(int cola[numProc]);
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
		//imprimirVec(tamPags);
		bubbleSort(tamPags,numProc);
		maxPaginas=tamPags[numProc-1];
		//printf("%d",maxPaginas);
		imprimirVec(tamPags);
		//--------------------se declara arreglo tridimensional e inicializa en -1
		int TD[numProc][maxPaginas][2];
		basura(TD);
		//-------------------------------se hace leida para tablas de paginas--------------------------------
		pagPorProc=0;
		noProceso=0;
		fp=fopen(argv[1],"r");//se abre para lectura
		if(fp==NULL){
			printf("no se pudo abrir: %s\n",argv[1]);//si no encuentra el archivo
			return 1;
		}else{
			while(fgets(str,MAXCHAR,fp)!=NULL){
					//funcion para meter las paginas
					sacarPaginaDesplazamiento(str,TD);
				}
			fclose(fp);//se cierra archivo
		}
		printf("*********tabla de procesos*******\n");
		imprimirArray(TP);
		//----------------imprime tablas de direcciones------
		for(int i=0;i<numProc;i++){
			printf("*********tabla de direcciones P%d*******\n",i);
			imprimirArray3D(TD,i);
		}
		//----------------------------------------despues de esto se crea cola------------------------------
		int cola[numProc];
		for(int i=0;i<numProc;i++){
			cola[i]=TP[numProc-i-1][0];
		}
		printf("*********cola de procesos*******\n");
		imprimirVec(cola);
		//---------------------------------------se crea tabla memoria inicial------------------------------
		int MI[marcosPagina][4];
		iniciarMemoria(MI,TD,TP);
		printf("********Memoria Inicial*********\n");
		imprimirMemoria(MI);
		//-------------------------------------ya obtenido todo en tablas se empieza algoritmo-------
		do{
		}while(colavacia(cola));
		
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
					tamPags[noProceso-1]=pagPorProc;//printf("%d\n",pagPorProc);//**aqui se agrega a vector
				}
			}
			bandera++;
			token=strtok(NULL,limite);
		}
}
//---------------funcion para imprimir vector----------------------------------------------------------------------------------------------
void imprimirVec(int array[numProc]){
		for(int i=0;i<numProc;i++){
				if(array[i]!=-1)
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
//-----------------FUNCION PARA LLENAR TABLAS DE PAGINAS---------------------------------------------------------------------------------
void sacarPaginaDesplazamiento(char* cadena,int TD[numProc][maxPaginas][2]){
	char limite[]=" ";
	char* token=strtok(cadena,limite);
	int bandera=0;//es una bandera que empezara cada cadena dividida en 3, 0 es el primero, 1 es el segundo y 2 es el ultimo dato
	bool pregunta;
	int aux=0;
	//si esta bandera es 0 y el token es 0 entonces es master
	if(token!=NULL)
		while(token!=NULL){
			if(strcmp(cadena,"0")==0 && bandera==0){//si es master
				pregunta=true;
				printf("-------%d\n",noProceso);
				pagPorProc=0;
			}else if(strcmp(cadena,"1") && bandera==0)//si es 1 es una pagina
				pregunta=false;

			if(bandera==2 && pregunta==true){//significa que encontro un tamaño
					noProceso++;
			}
			if((bandera==1 && pregunta==false) || (bandera==2 && pregunta==false)){//significa que es pagina o desplaz
				if(bandera==1){
					printf("%i  ",atoi(token));//paginas
					aux=atoi(token);
					TD[noProceso-1][pagPorProc][0]=aux;
				}
				if(bandera==2){
					printf("%i-->%d\n",atoi(token),pagPorProc);//desplazamientos
					aux=atoi(token);
					TD[noProceso-1][pagPorProc][1]=aux;
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
//---------------funcion para imprimir memoria ----------------------------------------------------------------------------------------------
void imprimirMemoria(int array[][4]){
		for(int i=0;i<marcosPagina;i++){
			for(int j=0;j<4;j++){
				printf(" %d ",array[i][j]);
			}
			printf("\n");
		}
}
//-------------funciion para imprimir arreglo 3d-------------------------------------------------------------------------------------------
void imprimirArray3D(int array[numProc][maxPaginas][2],int proc){
		for(int i=0;i<maxPaginas;i++){
			for(int j=0;j<2;j++){
				if(array[proc][i][j]!=-1)
					printf(" %d ",array[proc][i][j]);
			}
			printf("\n");
		}
}
//----------funcion que inicializa arreglo 3d en -1--------------------------------------------------------------------------------------
void basura(int array[numProc][maxPaginas][2]){
	for(int i=0;i<numProc;i++){
		for(int j=0;j<maxPaginas;j++){
			for(int k=0;k<2;k++){
				array[i][j][k]=-1;
			}
		}
	}
}
//---------funcion que elimina elemento de cola------------------------------------------------------------------------------------------
void eliminar(int cola[numProc],int elemento){
	int i;
	for(i=0;i<numProc;i++)
		if(cola[i]==elemento){
			cola[i]=-1;
			break;
		}
}
//-------funcion para inicializar memoria inicial------------------------------------------------------------------------------------------
int sacarSiguientePagina(int TD[numProc][maxPaginas][2],int proceso,int pag){
	int pagina=0;
	for(int i=0;i<maxPaginas;i++){
		if(TD[proceso][i][0]!=pag){
			pagina=TD[proceso][i][0];
			break;
		}
	}
	return pagina;
}
void iniciarMemoria(int MI[marcosPagina][4],int TD[numProc][maxPaginas][2],int TP [numProc][3]){
		for(int i=0;i<marcosPagina;i++){
			MI[i][0]=i;//el marco de pagina
			if(i<numProc){
				MI[i][1]=TP[i][0];//asigna el pid
				MI[i][2]=TD[i][0][0];
			}else if(i>=numProc){//si sobre pasa el marco a la cantidad de procesos
				MI[i][1]=TP[i-numProc][0];//asigna el pid
				MI[i][2]=sacarSiguientePagina(TD,i-numProc,0);
			}
			MI[i][3]=0;//la frecuencia inicial es 0
		}
}
//---------------------------------------Cola vacia?-----------------------------------------------------------------------------------
bool colavacia(int cola[numProc]){
	int cont=0;
	bool preg;
	for(int i=0;i<numProc;i++)
		if(cola[i]==-1)
			cont++;
	if(cont==numProc)
		preg=false;//si esta vacia regresa false
	else
		preg=true;//si esta aun con elemento es true
	return preg;
}









