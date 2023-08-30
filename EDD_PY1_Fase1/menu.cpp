#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <string>
#include <map>

using namespace std;

struct Usuario {
    string nombre;
    string contrasena;
    string puesto;
    string id;
    Usuario* siguiente;

    Usuario(string _nombre, string _contrasena, string _puesto, string _id) {
        nombre = _nombre;
        contrasena = _contrasena;
        puesto = _puesto;
        id = _id;
        siguiente = nullptr;
    }
};

struct ListaUsuarios {
    Usuario* cabeza;

    ListaUsuarios() {
        cabeza = nullptr;
    }

    void agregarUsuario(Usuario* nuevoUsuario) {
        if (!cabeza) {
            cabeza = nuevoUsuario;
        } else {
            Usuario* actual = cabeza;
            while (actual->siguiente) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoUsuario;
        }
    }
};

struct Proyecto {
    string id;
    string nombre;
    char prioridad;  // A, B, C
    
    Proyecto(string _nombre, char _prioridad) {
        static int idCounter = 100;
        id = "PY-" + to_string(idCounter++);
        nombre = _nombre;
        prioridad = _prioridad;
    }
    
    // Sobrecarga del operador '<' para la cola de prioridad
    bool operator<(const Proyecto& otro) const {
        return prioridad < otro.prioridad;
    }
};

struct Asignacion {
    string idProyecto;
    string idUsuario;
    
    Asignacion(string _idProyecto, string _idUsuario) : idProyecto(_idProyecto), idUsuario(_idUsuario) {}
};

ListaUsuarios usuariosLista;
map<string, Usuario*> usuariosMapa;
int contadorIDs[] = {0, 0, 0}; // Para cada puesto: Developer Frontend, Developer Backend, Quality
priority_queue<Proyecto> colaDePrioridad;
vector<Asignacion> asignaciones;

string generarNuevoID(string puesto) {
    int indice = 0;
    if (puesto == "Developer Frontend") {
        indice = 0;
    } else if (puesto == "Developer Backend") {
        indice = 1;
    } else if (puesto == "Quality") {
        indice = 2;
    }
    contadorIDs[indice]++;
    int contador = contadorIDs[indice];

    if (puesto == "Developer Frontend") {
        return "FDEV-" + to_string(contador);
    } else if (puesto == "Developer Backend") {
        return "BDEV-" + to_string(contador);
    } else if (puesto == "Quality") {
        return "QA-" + to_string(contador);
    } else {
        return "";
    }
}

void generarGrafo() {
    FILE *archivoDOT;
    archivoDOT = fopen("usuarios.dot", "w");
    
    if (!archivoDOT) {
        std::cerr << "Error al abrir el archivo DOT." << std::endl;
        return;
    }

    fprintf(archivoDOT, "digraph Usuarios {\n");
    
    // Imprimir los nodos de los usuarios
    Usuario *actual = usuariosLista.cabeza;
    while (actual) {
        fprintf(archivoDOT, "  \"%s\" [label=\"%s\\n%s\"];\n", actual->id.c_str(), actual->nombre.c_str(), actual->puesto.c_str());
        actual = actual->siguiente;
    }
    
    // Crear subgrafos para las filas
    fprintf(archivoDOT, "  subgraph fila1 {\n");
    fprintf(archivoDOT, "    label=\"Fila 1\";\n");
    actual = usuariosLista.cabeza;
    while (actual) {
        fprintf(archivoDOT, "    \"%s\";\n", actual->id.c_str());
        actual = actual->siguiente;
    }
    fprintf(archivoDOT, "  }\n");
    
    // Conectar los nodos dentro de cada fila
    actual = usuariosLista.cabeza;
    while (actual && actual->siguiente) {
        fprintf(archivoDOT, "  \"%s\" -> \"%s\";\n", actual->id.c_str(), actual->siguiente->id.c_str());
        actual = actual->siguiente;
    }
    
    fprintf(archivoDOT, "}\n");
    fclose(archivoDOT);
    
    // Generar la imagen del grafo
    std::string command = "dot -Tpng usuarios.dot -o usuarios.png";
    system(command.c_str());
}

void mostrarMenuUsuarios() {
    int opcion;
    string nombre, contrasena, puesto, nombreArchivo, linea; // Declarar todas las variables aquí

    do {
        cout << "Menu:" << endl;
        cout << "1. Cargar Manual" << endl;
        cout << "2. Carga Masiva" << endl;
        cout << "3. Salir" << endl;
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                cout << "Seleccionaste Carga Manual." << endl;

                Usuario* nuevoUsuario;
                cout << "Ingrese el nombre de usuario: ";
                getline(cin, nombre);

                cout << "Ingrese la password: ";
                getline(cin, contrasena);

                cout << "Elija un puesto: " << endl;
                cout << "1. Developer Frontend" << endl;
                cout << "2. Developer Backend" << endl;
                cout << "3. Quality" << endl; 
                int seleccionPuesto;
                cin >> seleccionPuesto;
                cin.ignore();

                switch (seleccionPuesto) {
                    case 1:
                        nuevoUsuario = new Usuario(nombre, contrasena, "Developer Frontend", generarNuevoID("Developer Frontend"));
                        usuariosLista.agregarUsuario(nuevoUsuario);
                        cout << "Usuario registrado exitosamente. ID: " << nuevoUsuario->id << endl;
                        break;
                    case 2:
                        nuevoUsuario = new Usuario(nombre, contrasena, "Developer Backend", generarNuevoID("Developer Backend"));
                        usuariosLista.agregarUsuario(nuevoUsuario);
                        cout << "Usuario registrado exitosamente. ID: " << nuevoUsuario->id << endl;
                        break;
                    case 3:
                        nuevoUsuario = new Usuario(nombre, contrasena, "Quality", generarNuevoID("Quality"));
                        usuariosLista.agregarUsuario(nuevoUsuario);
                        cout << "Usuario registrado exitosamente. ID: " << nuevoUsuario->id << endl;
                        break;
                    default:
                        cout << "Seleccion de puesto invalida." << endl;
                        continue;
                }
                break;
            }
            case 2: {
                cout << "Seleccionaste Carga Masiva desde CSV." << endl;
                cout << "Ingrese el nombre del archivo CSV: ";
                cin >> nombreArchivo;
                cin.ignore();

                ifstream archivo(nombreArchivo);
                if (!archivo.is_open()) {
                    cout << "No se pudo abrir el archivo." << endl;
                    break;
                }

                while (getline(archivo, linea)) {
                    stringstream ss(linea);
                    string idStr, nombre, contrasena, puesto;
                    getline(ss, idStr, ',');
                    getline(ss, nombre, ',');
                    getline(ss, contrasena, ',');
                    getline(ss, puesto, ',');

                    Usuario* nuevoUsuario = new Usuario(nombre, contrasena, puesto, generarNuevoID(puesto));
                    usuariosLista.agregarUsuario(nuevoUsuario);
                }

                cout << "Carga masiva completada." << endl;
                archivo.close();
                break;
            }
            case 3:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opcion invalida. Por favor, selecciona una opcion valida." << endl;
        }
    } while (opcion != 3);
}



void crearProyecto() {
    cout << "Ingrese el nombre del proyecto: ";
    string nombre;
    cin.ignore();
    getline(cin, nombre);
    cout << "Seleccione la prioridad (A, B, C): ";
    char prioridad;
    cin >> prioridad;
    prioridad = toupper(prioridad); // Convertir a mayúscula
    if (prioridad == 'A' || prioridad == 'B' || prioridad == 'C') {
        Proyecto nuevoProyecto(nombre, prioridad);
        colaDePrioridad.push(nuevoProyecto);
        cout << "Proyecto creado exitosamente. ID: " << nuevoProyecto.id << endl;
    } else {
        cout << "Prioridad invalida." << endl;
    }
}

void asignarProyecto() {
    cout << "Ingrese el ID del usuario al que desea asignar un proyecto: ";
    string idUsuario;
    cin >> idUsuario;

    if (usuariosMapa.find(idUsuario) == idUsuario) { 
        cout << "Usuario no encontrado." << endl;
        return;
    }

    if (colaDePrioridad.empty()) {
        cout << "No hay proyectos disponibles para asignar." << endl;
        return;
    }

    Proyecto proyecto = colaDePrioridad.top();
    colaDePrioridad.pop();

    Asignacion asignacion(proyecto.id, idUsuario);
    asignaciones.push_back(asignacion);

    cout << "Proyecto asignado exitosamente. ID de proyecto: " << proyecto.id << endl;
}

