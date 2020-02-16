#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(){
	char *comando,*salida="sal";
	comando=calloc(1024,sizeof(char));
	
	do{
		scanf("%s",comando);
		getchar();
		printf("%s\n",comando);
	}while(strcmp(comando,salida)!=0);
	
	return 0;
}
