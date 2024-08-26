#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <winsock2.h>
#include <locale.h>
#include <time.h>

#include "Servidor.h"

int main()
{
    /*
    SOCKET socketServidor = escucharConexiones();
    SOCKET socketCliente  = aceptarSocket(socketServidor);

    while(true)
    {
        printf("*********************************************** \n");
        printf("Esperando a que el cliente ingrese una opción \n");

        char buffer[1024];
        recv(socketCliente, buffer, sizeof(buffer), 0);
        printf("Opción ingresada: %s \n", buffer);

        if(strcmp(buffer, "1") == 0) // Si el cliente ingreso en la opción 1
        {
            memset(buffer, '\0', strlen(buffer) ); // Limpio el buffer reemplazando todos sus caracteres con '\0'

            while(true)
            {
                recv(socketCliente, buffer, sizeof(buffer), 0);
                int tamanio = (int) strtol(buffer, NULL, 10);
                memset(buffer, '\0', strlen(buffer) );

                if(tamanio < 5 || tamanio > 15)
                {
                    printf("Longitud invalida. Informando al cliente \n");
                    enviarMensaje("Longitud invalida", socketCliente);
                }
                else
                {
                    printf("Longitud valida \n");
                    enviarMensaje("Longitud valida", socketCliente);
                    enviarMensaje(generarNombreDeUsuario(tamanio), socketCliente);
                    break;
                }
            }

        }
        if(strcmp(buffer, "2") == 0)
        {
            memset(buffer, '\0', strlen(buffer) ); // Limpio el buffer reemplazando todos sus caracteres con '\0'

            while(true)
            {
                recv(socketCliente, buffer, sizeof(buffer), 0);
                int tamanio = (int) strtol(buffer, NULL, 10);
                memset(buffer, '\0', strlen(buffer) );

                if(tamanio < 8 || tamanio > 50)
                {
                    printf("Longitud invalida. Informando al cliente \n");
                    enviarMensaje("Longitud invalida", socketCliente);
                }
                else
                {
                    printf("Longitud valida \n");
                    enviarMensaje("Longitud valida", socketCliente);
                    enviarMensaje(generarContrasenia(tamanio), socketCliente);
                    break;
                }
            }

        }
        if(strcmp(buffer, "0") == 0)
        {
            memset(buffer, '\0', strlen(buffer) );
            break;
        }

    }*/

    // Lo mismo que lo comentado pero más legible (dejo lo comentado para debugear)

    while(true)
    {
        SOCKET socketServidor = escucharConexiones();
        SOCKET socketCliente  = aceptarSocket(socketServidor);

        while(true)
        {
            printf("*********************************************** \n");
            printf("Esperando a que el cliente ingrese una opción valida \n");

            char opcionDelCliente[1024];
            recv(socketCliente, opcionDelCliente, sizeof(opcionDelCliente), 0);
            printf("Opción ingresada: %s \n", opcionDelCliente);

            if(strcmp(opcionDelCliente, "1") == 0) atenderGeneracionNombreDeUsuario(socketCliente);
            if(strcmp(opcionDelCliente, "2") == 0) atenderGeneracionContrasenia(socketCliente);
            if(strcmp(opcionDelCliente, "0") == 0) break;
        }

        /* CERRAR EL SOCKET Y WINSOCK */
        closesocket(socketCliente);
        closesocket(socketServidor);
        WSACleanup();

        system("cls");
    }

    system("pause");
    return 0;
}
