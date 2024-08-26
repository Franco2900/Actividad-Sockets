import socket   # Libreria para poder usar sockets
import os       # Libreria para usar funciones del sistema operativo

socketCliente = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Crea un socket

# Dirección IP y puerto del servidor
servidor_ip = '127.0.0.1'  # localhost
servidor_puerto = 3000

socketCliente.connect((servidor_ip, servidor_puerto)) # El cliente se conecta al servidor
print('Conectado al servidor') # Escribe un mensaje en la consola

while(True):

    opcion = 0

    while(True):

        print('Indique que desea generar'); 
        print('1) Nombre de usuario')
        print('2) Contraseña')
        print('Ingrese 0 para salir')

        while(True):
            opcion = input() # El usuario ingresa algo. Todo lo que ingrese se considera como un string
            
            if(opcion.isdigit() ):
                opcion = int(opcion) # Transformo el string a int
                break
            else:
                print('Ingrese solo un número')

        if(opcion == 1 or opcion == 2 or opcion == 0): break
        else:                                          print("Elija una opción valida")

    os.system('cls') # Limpia los mensajes de la consola



    if(opcion == 1):
        
        # Informo al servidor de la elección del usuario
        mensaje = '1\0'                          # Como el servidor esta en C, le agrego el \0 al final del mensaje porque los strings en C terminan con \0. Caso contrario, el servidor lee basura
        socketCliente.sendall(mensaje.encode() ) # Codifica un mensaje y lo envia al servidor
        
        while True:
            print('Indique cuantos caracteres quiere en su nombre de usuario (debe ser entre 5 y 15)')
            tamanio = 0

            while(True):
                tamanio = input() 
                
                if(tamanio.isdigit() ): break
                else:                   print('Ingrese solo un número')
            
            mensaje = tamanio + '\0'
            socketCliente.sendall(mensaje.encode() )

            respuesta = socketCliente.recv(1024).decode('utf-8').strip() # Recibe un mensaje del servidor. También lo decodifica de binario a string y le borra los espacios vacios
            respuesta = respuesta[:-1] # Elimino el último caracter del string. Los mensajes que vienen desde el servidor son strings C, es decir, siempre terminan con \0 al final

            if(respuesta == 'Longitud valida'):
                nombreGenerado = socketCliente.recv(1024).decode('utf-8')
                print('El nombre de usuario generado es: ' + nombreGenerado)
                break
            else:
                print(respuesta)
        


    if(opcion == 2):

        mensaje = '2\0'                          
        socketCliente.sendall(mensaje.encode() )

        while True:
            print('Indique cuantos caracteres quiere en su nombre de usuario (debe ser entre 8 y 50)')
            tamanio = 0

            while(True):
                tamanio = input() 
                
                if(tamanio.isdigit() ): break
                else:                   print('Ingrese solo un número')
            
            mensaje = tamanio + '\0'
            socketCliente.sendall(mensaje.encode() )

            respuesta = socketCliente.recv(1024).decode('utf-8').strip()
            respuesta = respuesta[:-1] 

            if(respuesta == 'Longitud valida'):
                codigoGenerado = socketCliente.recv(1024).decode('utf-8')
                print('El código generado es: ' + codigoGenerado)
                break
            else:
                print(respuesta)



    if(opcion == 0):

        mensaje = '0\0'                          
        socketCliente.sendall(mensaje.encode() )
        socketCliente.close() # Cierra el socket
        break


    print("Presiona Enter para continuar...")
    input()
    os.system('cls')