#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define PROMPT "<@#>"

char* separarEnVector(char* cadena);
int main(){
	char* argv[10];
	argv[0]="ls";
	argv[1]="-l";
	argv[2]=(char*)0;
	//execvp(argv[0],argv);
	char a='\0';
	char *hola="hola";
	char hola1[]="hola";
	int cont=0;
	//strncat(hola1,&a,1);
	while(hola[cont]!=0){
		cont++;
	}
	printf("%d",cont);
	for(int i=0;i<strlen(hola1)+1;i++){
		if(hola1[i]=='\0')
			printf("1");
	}

	
	return 0;
}
