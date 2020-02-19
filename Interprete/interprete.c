#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define PROMPT "<@#>"

void separarEnVector(char* cadena);
int main(){

	//***crea un apuntador a char dinamico ***/
	char *comando,*salida="sal";
	comando=(char*)malloc(1000*sizeof(char)+1);

	//***aqui se lee la cadena que ingresara como comando mientras no sea
	//*** la palabra "sal",despues imprime la palabra ingresada
	do{
		printf(PROMPT);
		scanf("%[^\n]",comando);
		getchar();
		printf("cadena orig: %s\n",comando);
	}while(strcmp(comando,salida)!=0);
	
	return 0;
}
void separarEnVector(char* cadena){
	printf("1");
	char *array[100];	
	char *aux=(char*)0;
	aux=(char*)malloc(1000*sizeof(char));
	for(int i=0;i<1000;i++){
		if(cadena[i]!='\0'){
			strncat(aux,&cadena[i],1);
		}else{
			printf("sep %d:%s",i,aux);
		}
	}
}
