
//Programa hecho por Josué Yafte Ramírez Viramontes y Juan Pablo Pérez Dublán
/*objetivo: el objetivo de este programa es poner en práctica nuestros conocimientos tanto en planificación de 
memoria como en planificación de procesos, por esto mismo tomaremos en cuenta los tres proceso básicos de la 
planificación que es seleccionar el proceso al que se le asignaría el procesador, admitir y organizar los procesos que 
compiten activamente y suspende de forma temporal y reanuda procesos. Por otro lado, pondremos en práctica la teoría del proceso.*/
/*En nuestro caso para correr el programa entramos    a la ruta contenedora y se ejecuta: ./planifiador archivo.txt*/

#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
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
/*------------------------------fuciones y declaracion de cola-------------*/
typedef struct nodo{
	int dato;
	struct nodo* siguiente;
} nodo;

nodo* primero = NULL;
nodo* ultimo = NULL;

void insertarNodo(int elemento);
void mostrarCola();
bool colaVacia();
void eliminarNodo(int elemento);
int cabezaCola();
bool buscarNodo(int nodoBuscado);
/*-----------------------------funciones del algoritmo------------------------*/
int indexOf(int proceso,int TP[numProc][3]);
void calcularReal(int indice,int proceso,int MI[marcosPagina][4],int TD[numProc][maxPaginas][2],int TP[numProc][3],int estado[numProc],int nodosEliminados[numProc],int paginas[numProc]);

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
		int paginas[numProc];
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
		for(int i=0;i<numProc;i++){
			paginas[i]=tamPags[i];
		}
		bubbleSort(tamPags,numProc);
		maxPaginas=tamPags[numProc-1]+1;
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
			printf("\n*********tabla de direcciones P%d*******\n",i);
			imprimirArray3D(TD,i);
		}
		//----------------------------------------despues de esto se crea cola------------------------------
		for(int i=0;i<numProc;i++){
			insertarNodo(TP[i][0]);
		}
		printf("*********cola de procesos*******\n");
		mostrarCola();
		//---------------------------------------se crea tabla memoria inicial------------------------------
		int MI[marcosPagina][4];
		iniciarMemoria(MI,TD,TP);
		printf("********Memoria Inicial*********\n");
		imprimirMemoria(MI);
		//-------------------------------------ya obtenido todo en tablas se empieza algoritmo-------
		int estado[numProc];
		for(int i=0;i<numProc;i++){
			estado[i]=0;
		}
		int nodosEliminados[numProc];
		for(int i=0;i<numProc;i++){
			nodosEliminados[i]=0;
		}
		//imprimirVec(nodosEliminados);
		do{
			//sacamos el primer proceso de la cola que es primero->dato
			printf("\nEntra proceso %d a ejecucion \n",cabezaCola());//indexOf(cabezaCola(),TP))
			//se saca la direccion virtual y real correspondiente
			calcularReal(indexOf(cabezaCola(),TP),cabezaCola(),MI,TD,TP,estado,nodosEliminados,paginas);
			imprimirMemoria(MI);
			printf("\n||||||||||||cola de procesos||||||||||\n");
			mostrarCola();
		}while(colaVacia()==false);
		
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
//-------------------------------Cola vacia,insertar nodo,mostrar cola,eliminar elemento,cabeza de la cola--------------------------------

void insertarNodo(int elemento){
	nodo* nuevo = (nodo*) malloc(sizeof(nodo));
	nuevo->dato=elemento;
	if(primero == NULL){
		primero = nuevo;
		primero->siguiente = NULL;
		ultimo = nuevo;
	}else{
		ultimo->siguiente = nuevo;
		nuevo->siguiente = NULL;
		ultimo = nuevo;
	}
}

void mostrarCola(){
	nodo* actual = (nodo*) malloc(sizeof(nodo));
	actual = primero;
	if(primero != NULL){
		
		while(actual != NULL){
			printf("\n %d", actual->dato);
			actual = actual->siguiente;
		}
	}else{
		printf("\n la cola esta vacia\n");
	}
	printf("\n");
}

bool colaVacia(){
	bool vacia;
	nodo* actual = (nodo*) malloc(sizeof(nodo));
	actual = primero;
	if(primero != NULL){
		vacia=false;//la cola no esta vacia
	}else{
		vacia=true;//la cola esta vacia
	}
	return vacia;
}
int cabezaCola(){
	return primero->dato;
}

void eliminarNodo(int elemento){
	nodo* actual = (nodo*) malloc(sizeof(nodo));
	actual = primero;
	nodo* anterior = (nodo*) malloc(sizeof(nodo));
	anterior = NULL;
	int nodoBuscado = elemento, encontrado = 0;
	if(primero != NULL){
		
		while(actual != NULL && encontrado != 1){
			
			if(actual->dato == nodoBuscado){		
				if(actual == primero){
					primero = primero->siguiente;
				}else if(actual == ultimo){
					anterior->siguiente = NULL;
					ultimo = anterior;
				}else{
					anterior->siguiente = actual->siguiente;
				}
				encontrado = 1;
			}
			anterior = actual;	
			actual = actual->siguiente;
		}
		if(encontrado==0){
			printf("\n Nodo no Encontrado\n\n");
		}else{
			free(anterior);
		}
	}else{
		printf("\n cola vacia\n\n");
	}
}
bool buscarNodo(int nodoBuscado){
	nodo* actual = (nodo*) malloc(sizeof(nodo));
	actual = primero;
	bool esta=false;
	int encontrado = 0;
	if(primero != NULL){
		
		while(actual != NULL && encontrado != 1){
			
			if(actual->dato == nodoBuscado){
				esta=true;
				encontrado = 1;
			}
						
			actual = actual->siguiente;
		}
		if(encontrado==0){
			esta=false;
		}
	}else{
		printf("\n La cola esta vacia\n\n");
	}
}
//-------------------------------------sacar indice de proceso dependiendo su ubicacion en tabla de procesos------------------------
int indexOf(int proceso,int TP[numProc][3]){
	int i;	
	for(i=0;i<numProc;i++)
		if(TP[i][0]==proceso)
			break;// for que cuenta hasta encontrar el proceso y regresa ese indice
	return i;
}
//--------------------------------------CALCULA direccion real-----------------------------------------------------------------
void actualizarPagina(int MI[marcosPagina][4],int pagina,int proceso){
	for(int i=0;i<marcosPagina;i++){
		if(MI[i][1]==proceso && MI[i][2]==pagina){
			MI[i][3]++;
		}
	}
}
void encolar(int elemento){
	eliminarNodo(elemento);
	insertarNodo(elemento);
}
//para esto primero busca en la Memoria inicial y de existir busca la pagi
bool existeEnMemoria(int proceso, int pagina,int MI[marcosPagina][4]){
	bool existe=false;
	for(int i=0;i<marcosPagina;i++)
		if(MI[i][1]==proceso && MI[i][2]==pagina)
			existe=true;
	return existe;
}
void intercambiar(int MI[marcosPagina][4],int proceso,int pagina,int frecuencia){
		int vectorTemp[marcosPagina];
		for(int i=0;i<marcosPagina;i++){
			vectorTemp[i]=MI[i][3];//saca todo en un vectr
		}
		bubbleSort(vectorTemp,marcosPagina);//se ordena
		int j;
		for(j=0;j<marcosPagina;j++)
			if(MI[j][3]==vectorTemp[0]){//si es el mas pequeño realizar intercambio
				MI[j][1]=proceso;
				MI[j][2]=pagina;
				MI[j][3]=frecuencia;
				break;
			}
}
bool nodoEliminado(int proc,int nodosEliminados[numProc]){
	bool eliminado=false;
	for(int i=0;i<numProc;i++)
		if(nodosEliminados[i]==proc)
			eliminado=true;
	return eliminado;
}
void calcularReal(int indice,int proceso,int MI[marcosPagina][4],int TD[numProc][maxPaginas][2],int TP[numProc][3], int pivote[numProc], int nodosEliminados[numProc],int paginas[numProc]){
	//hacemos busqueda por MI
	int pagina=0;//pagina a la que se hara el calculo
	int contador=0;//contador que verifica si se cumplieron los  quantums
	bool aux=true;
	for(int i=0;i<marcosPagina;i++){
		if(MI[i][1]==proceso && contador<quantum){
			pagina=pivote[indice];
			//pagina=MI[i][2];
			//ahora se hace barrido en la tabla de direcciones para sacar las direciones donde la pagina cumpla esa condicion
			for(int j=0;j<maxPaginas;j++){
				if(TD[indice][j+pivote[indice]][0]==pagina && TD[indice][j+pivote[indice]][0]!=-1 && contador<quantum){
					if((TD[indice][j+pivote[indice]][1]<l && TD[indice][j+pivote[indice]][1]>=0) && pagina<TP[indice][2] && existeEnMemoria(proceso,pagina,MI)){//significa que esta bien no hay desbordamiento
						contador++;
						printf("direccion virtual: (%d,%d) direccion real:%d \n",pagina,TD[indice][j+pivote[indice]][1],(pagina*l+TD[indice][j+pivote[indice]][1]));
						printf(existeEnMemoria(proceso,pagina,MI)? "true\n":"false\n");
						printf("operacion %d mp:%d\n",contador,i);
						actualizarPagina(MI,pagina,proceso);
					}else if((TD[indice][j+pivote[indice]][1]>=l || TD[indice][j+pivote[indice]][1]<0)){
						contador++;
						printf("desbordamiento de pagina en (%d,%d)\n",pagina,TD[indice][j+pivote[indice]][1]);//DESBORDAMIENTO
						encolar(proceso);
					}else if(pagina>=TP[indice][2]){
						printf("inexistencia de pagina\n");//INEXISTENCIA
						encolar(proceso);
					}else if(existeEnMemoria(proceso,pagina,MI)==false && (TD[indice][j+pivote[indice]][0]!=-1 || TD[indice][j+pivote[indice]][1])){//FALLO DE PAGINA
						printf("FALLO DE PAGINA \n");
						contador++;
						intercambiar(MI,proceso,pagina,0);
						printf("direccion virtual: (%d,%d) direccion real:%d\n",pagina,TD[indice][j+pivote[indice]][1],(pagina*l+TD[indice][j+pivote[indice]][1]));
						actualizarPagina(MI,pagina,proceso);
					}
				}else if(TD[indice][j+pivote[indice]][0]!=pagina && contador<quantum && TD[indice][j+pivote[indice]][0]!=-1 && contador<quantum){
					pagina=TD[indice][j+pivote[indice]][0];
					//printf("*%d %d %d*\n",indice,j+pivote[indice],TD[indice][j+pivote[indice]][1]);
					//imprimirArray3D(TD,indice);
					if((TD[indice][j+pivote[indice]][1]<l && TD[indice][j+pivote[indice]][1]>=0) && pagina<TP[indice][2] && existeEnMemoria(proceso,pagina,MI)){//significa que esta bien no hay desbordamiento
						contador++;
						printf("direccion virtual: (%d,%d) direccion real:%d 2\n",pagina,TD[indice][j+pivote[indice]][1],(pagina*l+TD[indice][j+pivote[indice]][1]));
						printf(existeEnMemoria(proceso,pagina,MI)? "true\n":"false\n");
						printf("operacion %d mp:%d\n",contador,i);
						actualizarPagina(MI,pagina,proceso);
					}else if((TD[indice][j+pivote[indice]][1]>=l || TD[indice][j+pivote[indice]][1]<0)){
						contador++;
						printf("desbordamiento de pagina en (%d,%d)\n",pagina,TD[indice][j+pivote[indice]][1]);//DESBORDAMIENTO
						encolar(proceso);
					}else if(pagina>=TP[indice][2]){
						printf("inexistencia de pagina\n");//INEXISTENCIA
						encolar(proceso);
					}else if(existeEnMemoria(proceso,pagina,MI)==false && (TD[indice][j+pivote[indice]][0]!=-1 || TD[indice][j+pivote[indice]][1])){//FALLO DE PAGINA
						printf("FALLO DE PAGINA \n");
						contador++;
						intercambiar(MI,proceso,pagina,0);
						printf("direccion virtual: (%d,%d) direccion real:%d\n",pagina,TD[indice][j+pivote[indice]][1],(pagina*l+TD[indice][j+pivote[indice]][1]));
						actualizarPagina(MI,pagina,proceso);
					}
				}else if((TD[indice][j+pivote[indice]][0]==-1 || TD[indice][j+pivote[indice]][1]==-1) && contador<quantum){
					printf("proceso termino:%d\n",proceso);
					aux=false;
					eliminarNodo(proceso);
					break;
				}
			}
			if(aux==false){
				break;
			}
			if(contador==quantum && pivote[indice]<paginas[indice]){//&& (TD[indice][j+pivote[indice]][0]!=-1)){
					//reinicia  a la cola el proceso lo manda a la cola	
					printf("su quantum expiró pivote:%d\n",pivote[indice]);
					encolar(proceso);
					pivote[indice]=pivote[indice]+contador;
					break;
			}
		}
	}
}



