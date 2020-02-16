#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(){
	//***crea un apuntador a char dinamico ***/
	char *comando,*salida="sal";
	comando=calloc(1024,sizeof(char));
	//***aqui se lee la cadena que ingresara como comando mientras no sea
	//*** la palabra "sal",despues imprime la palabra ingresada
	do{
		scanf("%s",comando);
		getchar();
		printf("%s\n",comando);
	}while(strcmp(comando,salida)!=0);
	
	return 0;
}
