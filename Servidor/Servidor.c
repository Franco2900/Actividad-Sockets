#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <winsock2.h>
#include <locale.h>
#include <time.h>

#include "Servidor.h"


/************************************* FUNCIONES DEL SOCKET *****************************************************************/
SOCKET escucharConexiones()
{
    setlocale(LC_ALL, ""); // Establezco que el programa maneje caracteres especiales
    srand(time(NULL) );    // srand() establece la semilla para el generador de números aleatorios rand(). time() devuelve el tiempo actual en segundos desde el 1 de enero de 1970. Al usar time(NULL) el generador rand() se inicializa con una semilla diferente

    /* INICIALIZAR WINSOCK */
    WSADATA wsaData;

    if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0 )
    {
        printf("Error al inicializar Winsock \n");
        return 1;
    }


    /* CREAR EL SOCKET */
    SOCKET socketServidor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socketServidor == INVALID_SOCKET)
    {
        printf("Error al crear el socket.\n");
        WSACleanup();
        return 1;
    }


    /* CONFIGURAR LA DIRECCIÓN Y PUERTO */
    SOCKADDR_IN servidorAddress;

    servidorAddress.sin_family = AF_INET;
    servidorAddress.sin_port = htons(3000);
    servidorAddress.sin_addr.s_addr = INADDR_ANY;


    /* ASOCIAR EL SOCKET CON LA DIRECCIÓN Y EL PUERTO */
    if (bind(socketServidor, (SOCKADDR *)&servidorAddress, sizeof(servidorAddress)) == SOCKET_ERROR) // bind() asocia una direccion con un socket
    {
        printf("Error al asociar el socket con la dirección \n");
        closesocket(socketServidor);
        WSACleanup();
        return 1;
    }


    /* ESCUCHAR POR CONEXIONES ENTRANTES */
    if (listen(socketServidor, 1) == SOCKET_ERROR) // listen() pone al socket servidor en modo de escucha. Sus parámetros son el socket y el número máximo de clientes que escucha a la vez
    {
        printf("Error al escuchar en el socket.\n");
        closesocket(socketServidor);
        WSACleanup();
        return 1;
    }
    else
    {
        printf("Escuchando por conexiones con nuevos clientes \n");
    }

    return socketServidor;
}

SOCKET aceptarSocket(SOCKET socketServidor)
{
    /* ACEPTAR UNA CONEXION */
    SOCKADDR_IN clienteAddress;
    int clienteAddrLen = sizeof(clienteAddress);
    SOCKET socketCliente = accept(socketServidor, (SOCKADDR *)&clienteAddress, &clienteAddrLen);

    if (socketCliente == INVALID_SOCKET)
    {
        printf("Error al aceptar la conexión \n");
        closesocket(socketServidor);
        WSACleanup();
        return 1;
    }
    else printf("Cliente aceptado \n");


    return socketCliente;
}


void enviarMensaje(char *mensaje, SOCKET socketCliente)
{
    send(socketCliente, mensaje, strlen(mensaje)+1, 0); // Tiene el +1 por el caracter nulo del final del string \0
}


/************************************************** PUNTO 1.A ***********************************************************/

char* generarNombreDeUsuario(int tamanio)
{
    int tamanioNombreUsuario = tamanio;

    char *nombreUsuario = NULL; // Puntero char vacio
    nombreUsuario = (char *)malloc((tamanioNombreUsuario+1) * sizeof(char)); // malloc() le asigna memoria suficiente al puntero para almacenar la cantidad de caracteres indicados. Tiene el +1 para añadirle después el '\0' de cierre de string

    char consonantes[] = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ"; // Caracteres entre los que se puede elegir
    char vocales[] = "aeiouAEIOU";

    int longitudConsonantes = sizeof(consonantes) - 1;
    int longitudVocales = sizeof(vocales) - 1;

    int empezarConConsonantes = rand() % 2; // rand() genera un número aleatorio. Obtengo un número al azar entre 0 y 1

    if(empezarConConsonantes == 1)
    {
        for(int i = 0; i < tamanioNombreUsuario; i++)
        {
            char caracterAleatorio = consonantes[rand() % longitudConsonantes]; // Para asegurarme que el número al azar este en la longitud de los posibles caracteres a elegir, agarró su resto
            nombreUsuario[i] = caracterAleatorio;

            i++;

            caracterAleatorio = vocales[rand() % longitudVocales];
            nombreUsuario[i] = caracterAleatorio;
        }
    }
    if(empezarConConsonantes == 0)
    {
        for(int i = 0; i < tamanioNombreUsuario; i++)
        {
            char caracterAleatorio = vocales[rand() % longitudVocales];
            nombreUsuario[i] = caracterAleatorio;

            i++;

            caracterAleatorio = consonantes[rand() % longitudConsonantes];
            nombreUsuario[i] = caracterAleatorio;
        }
    }

    nombreUsuario[tamanioNombreUsuario] = '\0'; // Agrego el carácter de cierre al final del string

    printf("Nombre de usuario: %s \n", nombreUsuario);
    return nombreUsuario;
}


void atenderGeneracionNombreDeUsuario(SOCKET socketCliente)
{
    printf("Se eligio la opción: Generar nombre de usuario \n");

    while(true)
    {
        char buffer[1024];
        recv(socketCliente, buffer, sizeof(buffer), 0);
        printf("Longitud recibida: %s \n", buffer);

        int tamanio = (int) strtol(buffer, NULL, 10);

        if(tamanio < 5 || tamanio > 15)
        {
            printf("Longitud invalida. Se espera una longitud entre 5 y 15. Informando al cliente \n");
            enviarMensaje("Longitud invalida", socketCliente);
        }
        else
        {
            printf("Longitud valida \n");
            enviarMensaje("Longitud valida", socketCliente);

            char* nombreDeUsuario = generarNombreDeUsuario(tamanio);
            enviarMensaje(nombreDeUsuario, socketCliente);
            break;
        }
    }

}

/************************************************** PUNTO 1.B ***********************************************************/

char* generarContrasenia(int tamanio)
{
    int tamanioContrasenia = tamanio; // Obtengo un número al azar entre 0 y 42, y luego le sumo 8

    char *contrasenia = NULL;
    contrasenia = (char *)malloc((tamanioContrasenia+1) * sizeof(char));

    char caracteresAlfanumericos[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int longitudCaracteresAlfanumericos = sizeof(caracteresAlfanumericos) - 1;

    for(int i = 0; i < tamanioContrasenia; i++)
    {
        char caracterAleatorio = caracteresAlfanumericos[rand() % longitudCaracteresAlfanumericos];
        contrasenia[i] = caracterAleatorio;
    }

    printf("Contraseña: %s \n", contrasenia);
    return contrasenia;
}


void atenderGeneracionContrasenia(SOCKET socketCliente)
{
    printf("Se eligio la opción: Generar contraseña \n");

    while(true)
    {
        char buffer[1024];
        recv(socketCliente, buffer, sizeof(buffer), 0);
        printf("Longitud recibida: %s \n", buffer);

        int tamanio = (int) strtol(buffer, NULL, 10);

        if(tamanio < 8 || tamanio > 50)
        {
            printf("Longitud invalida. Se espera una longitud de entre 8 y 50. Informando al cliente \n");
            enviarMensaje("Longitud invalida", socketCliente);
        }
        else
        {
            printf("Longitud valida \n");
            enviarMensaje("Longitud valida", socketCliente);

            char* contrasenia = generarContrasenia(tamanio);
            enviarMensaje(contrasenia, socketCliente);
            break;
        }
    }

}
