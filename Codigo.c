#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int opcion;


//Esta funcion agrega al archivo Contactos.txt nuevos contactos
void AgregarContacto(){

	//Declaracion de variables
	char IP[16];
	char Puerto[5];
	char Nombre[40];
	
	//Se solicita la IP del contacto
	printf("\n Ingrese la IP del contacto: ");
	scanf("%s",IP);

	//Se solicita el puerto del contacto
	printf(" Ingrese el número de puerto: ");
	scanf("%s", Puerto);

	//Se solicita el nombre(alias) del contacto
	printf(" Ingrese el nombre del contacto: ");
	scanf("%s", Nombre);
	
	//Una vez recolectado la informacion se guarda en el archivo
	FILE* fichero;
        
        fichero = fopen("Contactos.txt","a");
        if(fichero!= NULL){
		//Se ingresa la informacion al archivo
                fprintf(fichero,"%s; %s; %s\n",IP,Puerto,Nombre);
		//Se guarda y cierra el archivo
                fclose(fichero);
        }}





void Cliente( char * ip,char * puerto){

	int status;

	//Creacion del socket
	int Mysocket;
	Mysocket = socket(AF_INET,SOCK_STREAM,0);//Parametros(Protocolo,direccion,puerto)
	if(Mysocket>=0){
		printf("\nSe creo el socket");}

	struct sockaddr_in dir;
	dir.sin_family=AF_INET;
	inet_pton(AF_INET,ip,&(dir.sin_addr.s_addr));
        dir.sin_port= htons(atoi(puerto));

	//Conexion
	status = connect(Mysocket, (struct sockaddr *) &dir, sizeof(dir));
        if(status <0){ 
                printf("\nNo hay conexion\n" );
                exit(1);}
	else{

		char buffer[1500];
		puts("\033[01;34mEscriba su mensaje: \n");
		bzero(buffer,1500); // Clean buffer
		fgets(buffer,1499,stdin);
		scanf("%1499[^\n]",buffer);

		status = send(Mysocket,buffer,strlen(buffer)+1,0);

		if(status <0){ 
		        printf("\033[01;37m\nMensaje no enviado"); 
		        close(Mysocket);
		        exit(1);}
		else{
		        printf("\033[01;37m\nMensaje enviado");}}


	//Cerrar socket
	int off;
	off=close(Mysocket);
	if(off==0){
		printf("\nSe cerro el socket correctamente");}



	//printf("\nEnviando mensaje %s",buffer);
	printf("\nIP: %s",ip);
	printf("\nPuerto: %s", puerto);
	fflush( stdin );
}






void EnviarMensaje(){

	//Variables 
	char * Ip;
	char * Puerto;


	//Se solicita al usuario el nombre del contacto que desea enviar el msj
	char Destino[50];
	char * p;
	p= Destino;
	char * nombre;
	printf("Enviar mensaja a: ");
	scanf("%s",Destino);

	//Se accede al archivo de Contactos para extraer la IP y el puerto
	FILE *archivo;

	//Se abre el archivo
	archivo=fopen("Contactos.txt","r");
	char caract[200];

	while(!feof(archivo)){
		//Se obtiene linea del archivo
		fgets(caract,200,archivo);
		//Entra el if si el nombre del Contacto se encuentra en la linea
		if (strstr(caract,Destino)!=NULL){
			//Se separa por tokens la informacion
			char * Token;
			Token = strtok (caract,";");
			//Se guarda en una variable la ip
			Ip=Token;
		  	Token = strtok (NULL, " ");
			//Se guarda en una variable el puerto
			Puerto = Token;
			Token = strtok (NULL, " ");
			//Se guarda en una variable el nombre
			nombre = Token;
			Cliente(Ip,Puerto);
			break;}}

	printf("\033[01;37m\nNo se encontro el contacto");
	fflush(stdin);}


void Servidor(char * puerto){

	int tam;
	int Mysocket;
	Mysocket = socket(AF_INET,SOCK_STREAM,0);


	//Creacion del socket
	if(Mysocket>=0){
		printf("\nSe creo el socket");}

	//Asociacion del socket a un puerto
	struct sockaddr_in dir;
	dir.sin_family=AF_INET;
	//dir.sin_port=htons(puerto);
	dir.sin_addr.s_addr=htonl(INADDR_ANY);

	int Vpuerto;
	Vpuerto = bind(Mysocket, (struct sockaddr *) &dir, sizeof(dir));
	if(Vpuerto==0){
		printf("\nSe vinculo el socket con el puerto");}

	//Escuchar
	int conexion;
	while(1){
		listen(Mysocket, 5);
		tam=sizeof(struct sockaddr_in);
		conexion=accept(Mysocket, (struct sockaddr *)&dir, &tam);
                if(conexion >=0){
                        printf("Conexion exitosa");}
		else{
			printf("No se escuha");
			exit(1);}}

	//Cerrar socket
	int off;
	off=close(Mysocket);
	if(off==0){
		printf("\nSe cerro el socket correctamente");}

}



void RecibirMensajes(){

	char * msj;
	char * deqien;

	FILE *archivo;

	//Se abre el archivo
	archivo=fopen("Mensajes.txt","r");
	char caract[100];

	while(!feof(archivo)){
		fgets(caract,100,archivo);
		char * Token;
		Token = strtok (caract,";");
		//Se guarda en una variable la ip
		msj =Token;
		Token = strtok (NULL, " ");
		deqien= Token;
		printf("\033[01;35m\nestoy recibiendo msj: %s, de %s",msj,deqien);}

	Servidor(deqien);
	printf("\033[01;37m\n Fin de Mensajes nuevos");}





void MenuPrincipal(){
	printf("\n-------Bienvenido a WhatsApp-------");
	printf("\n");
	printf("\n----Agregar un nuevo contacto[1]---");
	printf("\n----Enviar un nuevo mensaje[2]-----");
	printf("\n----Leer nuevos mensajes[3]--------");
	printf("\n----Salir[4]-----------------------");
	printf("\n");
	
	fflush(stdin);
	scanf("%d",&opcion);

	if(opcion==1){
		AgregarContacto();}
	else if (opcion==2){
		EnviarMensaje();}
	else if (opcion==3){
		RecibirMensajes();}
	else
		exit(1);}





//Funcion main
void main(){
	while(1){
		MenuPrincipal();}
	printf("\n");}
