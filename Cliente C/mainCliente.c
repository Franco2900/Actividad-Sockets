#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Permite usar booleanos
#include <string.h>

#include <locale.h> // Libreria para localizar el lenguaje de los caracteres
#include <winsock2.h> // Librería para la programar sockets en Windows

#include "Cliente.h"

int main()
{
    SOCKET socketCliente = conectarseAlServidor();

    while(true) // Envia y recibe mensajes hasta que se inserte un 0
    {
        int opcion = menu();

        // OPCIONES DEL MENU
        if(opcion == 1)
        {
            enviarMensaje("1", socketCliente);     // Le indico al servidor que el cliente eligió la opción 1
            generarNombreDeUsuario(socketCliente);
        }
        if(opcion == 2)
        {
            enviarMensaje("2", socketCliente);
            generarContrasenia(socketCliente);
        }
        if(opcion == 0)
        {
            enviarMensaje("0", socketCliente);
            printf("Ha finalizado su sesión \n");
            break;
        }

    }


    /* CERRAR EL SOCKET Y WINSOCK */
    closesocket(socketCliente);  // closesocket() cierra el socket
    WSACleanup();

    system("pause");
    return 0;
}


