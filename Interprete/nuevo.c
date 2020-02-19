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
	execvp(argv[0],argv);

	
	return 0;
}
