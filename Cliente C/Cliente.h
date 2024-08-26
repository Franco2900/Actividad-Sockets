#ifndef CLIENTE_H_INCLUDED
#define CLIENTE_H_INCLUDED

#include "Cliente.c"

// Funciones del socket
SOCKET conectarseAlServidor();
void enviarMensaje(char *mensaje, SOCKET socketCliente);

void generarNombreDeUsuario(SOCKET socketCliente); // Punto 1.A
void generarContrasenia(SOCKET socketCliente);     // Punto 1.B

// Util
int menu();
int ingresarTamanio();

#endif // CLIENTE_H_INCLUDED
