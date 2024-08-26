#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Permite usar booleanos
#include <string.h>

#include <locale.h>   // Libreria para localizar el lenguaje de los caracteres
#include <winsock2.h> // Librería para la programar sockets en Windows

#include "Cliente.h"

/************************************* FUNCIONES DEL SOCKET ******************************************************/

SOCKET conectarseAlServidor()
{
    setlocale(LC_ALL, ""); // Establezco que el programa maneje caracteres especiales


    /* INICIALIZAR WINSOCK */
    WSADATA wsaData; // Creo una estructura WSADATA para implementar Winsock(Windows Sockets)

    // WSAStartup() inicia el uso de winsock. MAKEWORD(2, 2) indica que se usa la versión 2.2 de Winsock
    if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0 )
    {
        printf("Error al inicializar Winsock \n");
        return 1;
    }


    /* CREAR EL SOCKET */
    SOCKET socketCliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // La función socket() crea un socket. Sus parámetros son dominio, tipo de socket y protocolo

    //  Los dominios pueden ser:                    Los tipos pueden ser:                       Los protocolos pueden ser:
    //    AF_INET para IPv4                             SOCK_STREAM para una conexión TCP           IPPROTO_TCP para el protocolo TCP
    //    AF_INET6 para IPv6                            SOCK_DGRAM para una conexión UDP
    //    AF_UNIX para comunicación local

    if (socketCliente == INVALID_SOCKET)
    {
        printf("Error al crear el socket \n");
        WSACleanup(); // WSACleanup() finaliza el uso de Winsock
        return 1;
    }


    /* CONFIGURAR LA DIRECCIÓN Y PUERTO */
    SOCKADDR_IN servidorAddress; // SOCKADDR_IN es una estructura para manejar las direcciones del servidor socket

    servidorAddress.sin_family = AF_INET;          // sin_family es la familia de direcciones
    servidorAddress.sin_port = htons(3000);        // sin_port es el puerto. La función htons (host to network short) convierte el número de puerto del orden de bytes del host al orden de bytes de la red
    servidorAddress.sin_addr.s_addr =  inet_addr("127.0.0.1");  // sin_addr.s_addr es la dirección IP del servidor. En INADDR_ANY significa que aceptará conexiones en cualquiera de las interfaces de red disponibles en la máquina.


    /* CONECTARSE CON EL SERVIDOR */
    bool conexionExitosa = false;
    while(!conexionExitosa) // Se queda en un loop infinito hasta que se conecte
    {
        if (connect(socketCliente, (struct servidorAddress*)&servidorAddress, sizeof(servidorAddress)) == SOCKET_ERROR)
        {
            //system("cls"); // Limpia la pantalla. Prefiero que no lo haga para que así se vea que de verdad esta intentando conectarse de vuelta
            printf("Error al conectar con el servidor. Intentando de vuelta \n");
        }
        else
        {
            printf("Conectado con el servidor \n");
            conexionExitosa = true;
        }
    }

    return socketCliente;
}


void enviarMensaje(char *mensaje, SOCKET socketCliente)
{
    send(socketCliente, mensaje, strlen(mensaje)+1, 0); // send() envia un mensaje. Sus parámetros son el socket a través del cual se enviarán los datos, el buffer y el tamaño en bytes de los datos que se desean enviar
}


/************************************* MENU ******************************************************/

int menu()
{
    int opcion;
    boolean opcionValida = false;

    while(!opcionValida)
    {
        printf("Indique que desea generar \n");
        printf("1) Nombre de usuario \n");
        printf("2) Contraseña \n");
        printf("Ingrese 0 para salir \n");

        opcion = 0;

        while(true) // Me aseguro que el cliente solo ponga un int
        {
            if(scanf("%d", &opcion) )
            {
                fflush(stdin);
                break;
            }
            else
            {
                fflush(stdin);
                printf("Ingrese solo un número \n");
            }
        }

        if(opcion == 1 || opcion == 2 || opcion == 0) opcionValida = true;
        else                                          printf("Elija una opción valida \n");
    }

    system("cls");
    return opcion;
}


/************************************************** PUNTO 1.A ***********************************************************/

void generarNombreDeUsuario(SOCKET socketCliente)
{
    char buffer[1024];

    while(true)
    {
        printf("Indique cuantos caracteres quiere en su nombre de usuario (debe ser entre 5 y 15) \n");
        int tamanio = ingresarTamanio();

        char mensaje[sizeof(tamanio)];
        sprintf(mensaje, "%d", tamanio);    // Convierto el int a string
        enviarMensaje(mensaje, socketCliente);

        recv(socketCliente, buffer, sizeof(buffer), 0);

        if(strcmp(buffer, "Longitud valida") == 0)      // Si el servidor no me da el visto bueno, se repite todo
        {
            memset(buffer, '\0', strlen(buffer) );

            recv(socketCliente, buffer, sizeof(buffer), 0);
            printf("El nombre de usuario generado es: %s \n", buffer);
            memset(buffer, '\0', strlen(buffer) );

            system("pause");
            system("cls");
            break;
        }
        else
        {
            printf("%s\n", buffer);
            memset(buffer, '\0', strlen(buffer) );
        }
    }

}

/************************************************** PUNTO 1.B ***********************************************************/
void generarContrasenia(SOCKET socketCliente)
{
    char buffer[1024];

    while(true)
    {
        printf("Indique cuantos caracteres quiere en código (debe ser entre 8 y 50) \n");
        int tamanio = ingresarTamanio();

        char mensaje[sizeof(tamanio)];
        sprintf(mensaje, "%d", tamanio);    // Convierto el int a string
        enviarMensaje(mensaje, socketCliente);

        recv(socketCliente, buffer, sizeof(buffer), 0);

        if(strcmp(buffer, "Longitud valida") == 0)      // Si el servidor no me da el visto bueno, se repite todo
        {
            memset(buffer, '\0', strlen(buffer) );

            recv(socketCliente, buffer, sizeof(buffer), 0);
            printf("El código generado es: %s \n", buffer);
            memset(buffer, '\0', strlen(buffer) );

            system("pause");
            system("cls");
            break;
        }
        else
        {
            printf("%s\n", buffer);
            memset(buffer, '\0', strlen(buffer) );
        }
    }

}


/****************************************** UTIL *****************************************************/
int ingresarTamanio()
{
    int tamanio;

    while(true) // Me aseguro que el cliente solo ponga un int
    {
        if(scanf("%d", &tamanio) )
        {
            fflush(stdin);
            break;
        }
        else
        {
            fflush(stdin);
            printf("Ingrese solo un número \n");
        }
    }

    return tamanio;
}
