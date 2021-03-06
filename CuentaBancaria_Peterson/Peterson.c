/*
	Programa Hecho por Josue Yafte Ramírez Viramontes y Juan Pablo Pérez Dublán
	Este programa realiza las transacciones de retiro y deposito con dos hilos
	uno hace los retiros y otro los depositos con su region crítica 
	termina cada proceso cuando el retiro o deposito es 0
*/
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
int proc_favorecido, proc1deseaEntrar, proc2deseaEntrar;
int saldo=1000;//saldo inicial
//--------otras tareas
void otrasTareas1(){
	//sleep(2);
	printf("Hilo1 haciendo otras tareas...\n");
}
void otrasTareas2(){
	//sleep(2);
	printf("Hilo2 haciendo otras tareas...\n");
}
//------------proceso 1
void *deposito_HiloSecundario1(){
	int deposito;
	while(1){
		proc1deseaEntrar=1;
		proc_favorecido=2;
		while(proc2deseaEntrar && proc_favorecido==2);
		//aqui va la operacion de la region critica
		printf("ingrese monto a depositar:\n");
		fflush(stdout);
		scanf("%d",&deposito);//aqui va el deposito
		saldo=saldo+deposito;
		printf("el saldo actualizado es: %d", saldo);
		fflush(stdout);
		//**********
		printf("hilo1 saliendo region critica...\n");
		fflush(stdout);
		proc1deseaEntrar=0;
		if(deposito==0) //el hilo acaba si el deposito es 0, pero antes cede el paso al otro hilo
			pthread_exit(NULL);
		otrasTareas1();
		fflush(stdout);
	}
}


//------------proceso 2
void *retiro_HiloSecundario2(){
	int retiro;
	while(1){
		proc2deseaEntrar=1;
		proc_favorecido=1;
		while(proc1deseaEntrar && proc_favorecido==1);
		//aqui va la operacion region critica
		printf("ingrese monto a retirar:\n");
		fflush(stdout);
		scanf("%d",&retiro);
		if(saldo<retiro){
			printf("no se puede realizar retiro\n");
		}else{
		saldo=saldo-retiro;
		printf("el saldo actual es:%d\n",saldo);
		fflush(stdout);
		}
		//***********
		printf("hilo2 saliendo region critica...\n");
		fflush(stdout);
		proc2deseaEntrar=0;
		//el hilo acaba si el deposito es 0, pero antes cede el paso al otro hilo
		if(retiro==0)
			pthread_exit(NULL);
		otrasTareas2();
		fflush(stdout);
	}
}
int main(){
	// aqui va el algoritmo de crear los hios en 0 
	proc1deseaEntrar=0;
	proc2deseaEntrar=0;
	int error;
	pthread_t hilo1,hilo2;
	//-------creacion de hilos
	error=pthread_create(&hilo1,NULL,deposito_HiloSecundario1,NULL);
	error=pthread_create(&hilo2,NULL,retiro_HiloSecundario2,NULL);
	//----Uniones de hilos
	error=pthread_join(hilo1,NULL);
	error=pthread_join(hilo2,NULL);

	return 0;	
}
/*
	CONCLUSIONES
	Ramírez Viramontes Josue Yafte:
	Este programa me permitio  entender como es que la concurrencia entre procesos ya que
	este algoritmo es eficiente permite que no haya errores al depositar y retirar
	ya que en la vida real seria algo que no serviria mas con un sistema
	que al fallar podria afectar al usuario, sin embargo funciona con exito y sin problemas al hacer las operaciones
	sobre el saldo.

	Pérez Dublán Juan Pablo:
	Para finalizar esta tarea, me gustaría destacar el hecho de que se me hizo muy interesante
	agregarle los hilos POSIX al algoritmo de peterson ya que por un lado habíamos analizado el algoritmo y por el otro ya habíamos 
	puesto en practica el uso de hilos, entonces esto mismo nos hizo tener un analisis mas concreto.
	Por otro lado aunque es un ejercicio un tanto simple, nos da una idea de como poner en practica en la vida o 
	en proyectos más completos.

*/
