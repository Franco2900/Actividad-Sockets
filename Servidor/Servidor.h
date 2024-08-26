#ifndef SERVIDOR_H_INCLUDED
#define SERVIDOR_H_INCLUDED

#include "Servidor.c"

// Funciones del socket
SOCKET escucharConexiones();
SOCKET aceptarSocket(SOCKET socketServidor);
void enviarMensaje(char *mensaje, SOCKET socketCliente);

// Punto 1.A
char* generarNombreDeUsuario(int tamanio);
void atenderGeneracionNombreDeUsuario(SOCKET socketCliente);

// Punto 1.B
char* generarContrasenia(int tamanio);
void atenderGeneracionContrasenia(SOCKET socketCliente);

#endif // SERVIDOR_H_INCLUDED
