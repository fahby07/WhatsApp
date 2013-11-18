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





void Cliente( char * nome, char * ip,char * puerto){

	int status;
	int Mysocket;
	char buffer[1500];
	struct sockaddr_in direccion;


	//Crear socket
	Mysocket = socket(AF_INET,SOCK_STREAM,0);
	//Verificar la creacion del socket
	if(Mysocket>=0){
		printf("\nSe creo el socket");}
	else{
		perror("\nNo se concreto el socket");
		printf("\n");
		exit(1);}


	//Configuracion del socket
	direccion.sin_family=AF_INET;
	inet_pton(AF_INET,ip,&(direccion.sin_addr.s_addr));
        direccion.sin_port= htons(atoi(puerto));


	//Conexion y verificacion
	status = connect(Mysocket, (struct sockaddr *) &direccion, sizeof(direccion));
        if(status <0){ 
                printf("\nNo hay conexion" );
		close(Mysocket);
		printf("\n");
                exit(1);}
	else{
		printf("\nHay conexion");}


	//Conexion para envio de msj y verificacion
	status=send(Mysocket,nome,strlen(nome)+1,0);
	if(status<0){
		printf("\nNo se puede enviar mensajes en este momento");
		close(Mysocket);
		printf("\n");
		exit(1);}
	else{
		//Pedir el msj al usuario
		puts("\033[01;34mEscriba su mensaje: \n");
		bzero(buffer,1500); // Clean buffer
		fgets(buffer,1499,stdin);
		scanf("%1499[^\n]",buffer);

		status = send(Mysocket,buffer,strlen(buffer)+1,0);
		//Verificacion
		if(status <0){ 
		        printf("\033[01;37m\nMensaje no enviado"); 
		        close(Mysocket);
			printf("\n");
		        exit(1);}
		else{
		        printf("\033[01;37m\nMensaje enviado");}}


	//Cerrar socket
	int off;
	off=close(Mysocket);
	if(off==0){
		printf("\nSe cerro el socket correctamente");}

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
			Cliente(nombre,Ip,Puerto);
			break;}}

	printf("\033[01;37m\nNo se encontro el contacto");
	fflush(stdin);}


void Servidor(char * puerto){

	int Mysocket;
	struct sockaddr_in Myaddress;
	struct sockaddr_in Newaddress;
	int status;
	int datoRec;
	int address;
	char msj[1500];
	int enlace;


	//Creacion del socket
	Mysocket = socket(AF_INET,SOCK_STREAM,0);
	if(Mysocket<0){
		printf("\nNo se pudo crear el socket");
		printf("\n");
		exit(1);}
	printf("Se creo el socket exitosamente");
	

	//Configuracion del socket
	Myaddress.sin_family=AF_INET;
	Myaddress.sin_addr.s_addr=htonl(INADDR_ANY);
	Myaddress.sin_port = htons(atoi(puerto));


	//Asociacion con el puerto
	enlace = bind(Mysocket, (struct sockaddr *) &Myaddress, sizeof(Myaddress));
	if(enlace<0){
		printf("\nNo se vinculo el socket con el puerto");
		printf("\n");
		close(Mysocket);
		exit(1);}
	printf("\nVinculo con el puerto exitoso");


	//Ciclo de conexion
	while(1){
		listen(Mysocket, 5);
		address=sizeof(Newaddress);

		status=accept(Mysocket, (struct sockaddr *)&Newaddress, &address);
                if(status<0){
                        printf("\nConexion denegada");
			printf("\n");
			close(Mysocket);
			exit(1);}
		printf("\nConexion aceptada");

		//Recepcion de datos
		memset(msj,0x0,1500);
		
		datoRec = recv(status,msj,1500,0);
		if(datoRec<0){
			printf("\nNo se puede leer el mensaje");
			printf("\n");
			close(Mysocket);
			exit(1);}
		printf("\n Mensaje nuevo de: %s",msj);

		datoRec = recv(status,msj,1500,0);
		if(datoRec<0){
			printf("\nError en la recepcion del mensaje");
			printf("\n");
			close(Mysocket);
			exit(1);}
		printf("\n%s",msj);}
		
	//Cerrar socket
	int off;
	off=close(Mysocket);
	if(off==0){
		printf("\nSe cerro el socket correctamente");}

}



void RecibirMensajes(){

	char * puerto;
	char * ip;

	FILE *archivo;
	archivo=fopen("Config.txt","r");
	char linea[200];
	fgets(linea,200,archivo);

	char * Token;
	Token = strtok (linea,";");
	puerto =Token;
	Token = strtok (NULL, " ");
	ip = Token;

	Servidor(puerto);
	printf("\033[01;37m\n Fin de Mensajes nuevos");}



void MenuPrincipal(){
	printf("\n-------Bienvenido a WhatsApp-------");
	printf("\n");
	printf("\n----Agregar un nuevo contacto[1]---");
	printf("\n----Enviar un nuevo mensaje[2]-----");
	printf("\n----Salir[Cualquier tecla]---------");
	printf("\n");
	
	fflush(stdin);
	scanf("%d",&opcion);

	if(opcion==1){
		AgregarContacto();}
	else if (opcion==2){
		EnviarMensaje();}
	else 
		exit(1);}



//Funcion main
void main(){
	while(1){
		MenuPrincipal();
	}
	}
