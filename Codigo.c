#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

	//Estado del socket
        int status;
	//Creacion del socket
        int Mysocket;
	//Almacena mensaje
        char buffer[1500];
        struct sockaddr_in direccion;


        //Crear socket
        Mysocket = socket(AF_INET,SOCK_STREAM,0);
        //Verificar la creacion del socket
        if(Mysocket<0){
                perror("\033[01;37m\nNo se concreto el socket");
                exit(1);
}


        //Configuracion del socket
        direccion.sin_family=AF_INET;
        inet_pton(AF_INET,ip,&(direccion.sin_addr.s_addr));
        direccion.sin_port= htons(atoi(puerto));

        //Conexion y verificacion
        status = connect(Mysocket, (struct sockaddr *) &direccion, sizeof(direccion));
       
        if(status<0){
                printf("\033[01;37m\nNo se puede enviar mensajes en este momento");
                close(Mysocket);
                exit(1);
}
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
                        exit(1);
}
                else{
                        printf("\033[01;37m\nMensaje enviado");}}


        //Cerrar socket
	close(Mysocket);
        
}


void EnviarMensaje(){

        //Variables 
        char * Ip;
        char * Puerto;
        int volverMenu;
        volverMenu = 0;

        //Se solicita al usuario el nombre del contacto que desea enviar el msj
        char Destino[50];
        char * p;
        p= Destino;
        char * nombre;
        printf("\033[01;37m\nEnviar mensaje a: ");
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
                        volverMenu=1;
                        break;}}
        
        if(volverMenu==0)
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
                printf("\033[01;37m\nNo se pudo crear el socket");
                exit(1);
}
        

        //Configuracion del socket
        Myaddress.sin_family=AF_INET;
        Myaddress.sin_addr.s_addr=htonl(INADDR_ANY);
        Myaddress.sin_port = htons(atoi(puerto));


        //Asociacion con el puerto
        enlace = bind(Mysocket, (struct sockaddr *) &Myaddress, sizeof(Myaddress));
        if(enlace<0){
                printf("\033[01;37m\nNo se vinculo el socket con el puerto");
                close(Mysocket);
                exit(1);
}
    


        //Ciclo de conexion
        while(1){
                listen(Mysocket, 5);
                address=sizeof(Newaddress);

                status=accept(Mysocket, (struct sockaddr *)&Newaddress, &address);
                if(status<0){
                        close(Mysocket);
                        exit(1);}

                //Recepcion de datos
                memset(msj,0x0,1500);

                datoRec = recv(status,msj,1500,0);
                if(datoRec<0){
                        printf("\033[01;37m\nError en la recepcion del mensaje");
                        close(Mysocket);
                        exit(1);}
                printf("\033[01;33m\nNuevo mensaje: %s",msj);}
                
        //Cerrar socket
	close(Mysocket);
        
}



void RecibirMensajes(){

        // Variables
	char * puerto;
        char * ip;

	//Lee archivo de configuracion de puerto e ip propia       
	FILE *archivo;
        archivo=fopen("Config.txt","r");
        char linea[200];
        fgets(linea,200,archivo);

        //Almacena la informacion
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
        printf("\n----Salir[3]-----------------------");
        printf("\n");
        
        fflush(stdin);
        scanf("%d",&opcion);

        if(opcion==1){
                AgregarContacto();}
        else if (opcion==2){
                EnviarMensaje();}
        else 
                exit(1);
        MenuPrincipal();}

void *escuchar(void *servidor){;
        while(1)
                RecibirMensajes();}

void *llamada(void *menu){;
        while(1)
                MenuPrincipal();}


//Funcion main
void main(){

        int process;
        process = fork();

        if(process>=0){
                if(process==0){
                        pthread_t escucha;
                        char * servidor = "Servidor";
                        pthread_create(&escucha,NULL,escuchar,(void*)servidor);
                        pthread_join(escucha,NULL);}
                else{    
                        pthread_t envia;
                        char * cliente = "Cliente";
                        pthread_create(&envia,NULL,llamada,(void*)cliente);
                        pthread_join(envia,NULL);}}
        else{
                printf("\nFallo en el sistema");}
        }
