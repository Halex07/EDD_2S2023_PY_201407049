#include <iostream>
#include <string>
#include "menu.h" 


using namespace std;

int main() {
    // Definir usuario y contraseña predefinidos
    string usuarioCorrecto = "PM-201407049";
    string contrasenaCorrecta = "contrasena";

    // Variables para almacenar el usuario y la contraseña ingresados
    string usuarioIngresado, contrasenaIngresada;

    // Solicitar usuario y contraseña
    cout << "**********  EDD PROJECTUP  **********" << endl;
    cout << "User: ";
    cin >> usuarioIngresado;
    cout << "Password: ";
    cin >> contrasenaIngresada;

    // Verificar la autenticación
    if (usuarioIngresado == usuarioCorrecto && contrasenaIngresada == contrasenaCorrecta) {
        cout << "**********  EDD PROJECTUP  **********" << endl;
        cout << "******  Bienvenido PM-201407049  *****" << endl;

        // Menú con 4 opciones
        int opcion;
        do {
            cout << "Menu:" << endl;
            cout << "1. Cargar Empleados" << endl;
            cout << "2. Crear Proyecto" << endl;
            cout << "3. Crear Tareas" << endl;
            cout << "4. Asignar Tareas" << endl;
            cout << "5. Asignar Proyecto" << endl;
            cout << "6. Generar Grafica" << endl;
            cout << "7. Salir" << endl;
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
                case 1:
                    cout << "Seleccionaste Cargar Empleados" << endl;
                    mostrarMenuUsuarios();
                    break;
                case 2:
                    cout << "Seleccionaste Crear Proyecto" << endl;
                    crearProyecto();
                    break;
                case 3:
                    cout << "Seleccionaste Crear Tareas" << endl;
                    break;
                case 4:
                    cout << "Seleccionaste Asignar Tareas" << endl;
                    return 0;
                    break;
                case 5:
                    cout << "Seleccionaste Asignar Proyectos" << endl;
                    asignarProyecto();
                    break;
                case 6:
                    cout << "Seleccionaste Generar Grafica" << endl;
                     // Generar el archivo DOT después de salir del menú

                    generarGrafo();

                    // Convertir el archivo DOT en una imagen usando Graphviz
                    system("dot -Tpng usuarios.dot -o usuarios.png");
                    break;
                case 7:
                    cout << "Saliendo del programa." << endl;
                    break;
                default:
                    cout << "Opcion invalida. Por favor, selecciona una opcion valida." << endl;
            }
        } while (opcion != 7); // Repetir el menú hasta que se seleccione la opción de salir
    } else {
        cout << "Usuario o password incorrectos. Cierre el programa e intente nuevamente." << endl;
    }

    return 0;
}