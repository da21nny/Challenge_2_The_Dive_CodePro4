#define _POSIX_C_SOURCE 199309L //Para usar clock_gettime en C17

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

// Definicion de constantes para imprimir el laberinto
#define ENTRADA "🚪" // 🚪 o E
#define SALIDA "🏁" // 🏁 o S
#define PARED "⬜️" // ⬜️ o #
#define CORRECTO "🟩" // 🟩 o +
#define CAMINO "  "

// Estructura que contiene Eje X, Eje Y, distancia 
typedef struct{
    int dir_x;  //Direccion X (Columna)
    int dir_y;  //Direccion Y (Fila)
    int dist;   //Distancia
} Coordenadas; // Estructura Coordenadas

// Variables Globales
int **laberinto; //Matriz del laberinto (es un Puntero a un puntero)
int fila, columna; //Tamaño del laberinto
Coordenadas direccion[] = {{0,2}, {0,-2}, {2,0}, {-2,0}}; // 0{W} - 1{S} - 2{D} - 3{A}
Coordenadas movimientos[] = {{0,1}, {0,-1}, {1,0}, {-1,0}}; // 0{W} - 1{S} - 2{D} - 3{A} 

//Prototipo de funciones
int **crear_laberinto(int fila, int columna);
void generar_caminos(int pos_y, int pos_x);
void mezclar_coordenadas(Coordenadas dir[], size_t tamanho);
void intercambio_coordenadas(Coordenadas *dir_A, Coordenadas *dir_B);
void romper_paredes(int fila, int columna);
int bfs(Coordenadas entrada, Coordenadas salida, int fila, int columna);
bool es_valido(int newFila, int newCol, int fila, int columna, bool **visitado);
void imprimir_matriz(int **laberinto, int fila, int columna, Coordenadas entrada, Coordenadas salida);
void liberar_laberinto(int **laberinto, int columna);
void medir_rendimiento(double inicio, double post_creacion, double fin);
double obtener_tiempo();

// Funcion Principal o MAIN
int main(int argc, char *argv[]){
    srand(time(NULL)); // Inicializar semilla para numeros aleatorios
    if(argc == 1){ //Tamaño por defecto 10x10
        fila = 10;
        columna = 10;
    } else if (argc == 3){ //Tamaño ingresado por el usuario
        fila = atoi(argv[1]);
        columna = atoi(argv[2]);
        if (fila <= 4 || columna <= 4){
            printf("\nFila o Columna debe ser mayor a 4"); //Avisa al usuario que el tamaño del laberinto es muy pequeño
            return 1;
        }
        printf("Tamaño ingresado: %d x %d.\n", fila, columna); //Muestra el tamaño ingresado por el usuario
    } else{ //Validacion de argumentos
        printf("El programa requiere 2 argumentos\n"); 
        printf("Ejemplo ./programa 20 20\n");
        return 1;
    }
    // Asegurar que las dimensiones sean impares
    if(fila % 2 == 0) fila += 1;
    if(columna % 2 == 0) columna += 1;

    double inicio = obtener_tiempo(); // Medir tiempo de inicio
    int **laberinto = crear_laberinto(fila, columna); // Crear laberinto. Obs: Laberinto se maneja de esta manera (y,x) = (fila,columna)
    double post_creacion = obtener_tiempo(); // Medir tiempo despues de crear laberinto

    Coordenadas entrada = {1, 1}; // Entrada fija en (1,1)
    Coordenadas salida = {columna - 2, fila - 2}; // Salida fija en (columna-2, fila-2) 

    int distancia = bfs(entrada, salida, fila, columna); // Ejecutar BFS para encontrar el camino
    double fin = obtener_tiempo(); // Medir tiempo final

    imprimir_matriz(laberinto, fila, columna, entrada, salida); // Imprimir el laberinto con el camino encontrado

    if(distancia > 0){ // Si se encontro un camino
        printf("\n¡Camino encontrado! Distancia: %d pasos\n", distancia);
    } else { // Si no se encontro un camino
        printf("\nNo se encontró camino (esto no debería pasar)\n"); 
    }

    printf("\nObs: Se imprime un Laberinto Impar (fila + 1) x (columna + 1) para que tenga paredes exteriores\n");

    medir_rendimiento(inicio, post_creacion, fin); // Medir y mostrar rendimiento
        
    liberar_laberinto(laberinto, fila); // Liberar memoria del laberinto
    
    return 0;
}

// Funcion que crea un laberinto llenos de 1 (Paredes) de tamaño MxN
int **crear_laberinto(int fila, int columna){
    laberinto = (int **)malloc(fila * sizeof(int *)); //Asignar memoria para filas
    for(size_t y = 0; y < fila; y++){
        laberinto[y] = (int *)malloc(columna * sizeof(int)); //Asignar memoria para columnas
        for(size_t x = 0; x < columna; x++){
            laberinto[y][x] = 1; //Inicializar todas las posiciones con 1 (Pared)
        }
    }
    int inicio_y = 1, inicio_x = 1; //Posicion inicial para generar caminos
    generar_caminos(inicio_y, inicio_x); //Generar caminos en el laberinto

    return laberinto;
}

//Funcion que genera de forma aleatoria los caminos (0) en el laberinto
void generar_caminos(int pos_y, int pos_x){
    laberinto[pos_y][pos_x] = 0; //Marcar la posicion actual como camino (0)

    size_t tam_dir = sizeof(direccion) / sizeof(direccion[0]); //Calcular el tamaño del arreglo de direcciones

    mezclar_coordenadas(direccion, tam_dir); //Mezclar las direcciones para obtener caminos aleatorios

    for(size_t i = 0; i < tam_dir; i++){
        int dirX = direccion[i].dir_x; //Obtener el desplazamiento en X
        int dirY = direccion[i].dir_y; //Obtener el desplazamiento en Y
        int newX = pos_x + dirX; //Calcular nueva posición en X
        int newY = pos_y + dirY; //Calcular nueva posición en Y

        if (newY >= 1 && newY < fila && newX >= 1 && newX < columna && laberinto[newY][newX] == 1){ //Verificar si la nueva posición es válida
            laberinto[pos_y + dirY/2][pos_x + dirX/2] = 0; //Romper la pared entre la posición actual y la nueva posición
            generar_caminos(newY, newX); //Llamada recursiva para continuar generando caminos desde la nueva posición
        }
    }
}

//Funcion que mezcla las coordenadas para producir caminos aleatorios.
void mezclar_coordenadas(Coordenadas dir[], size_t tamanho){
    for(size_t i = 0; i < tamanho - 1; i++){
        size_t j = i + (rand() % (tamanho - i)); //Indice aleatorio entre i y tamanho-1
        intercambio_coordenadas(&dir[i], &dir[j]); //Intercambiar las coordenadas en las posiciones i y j
    }
}

//Funcion auxiliar para intercambiar las coordenadas.
void intercambio_coordenadas(Coordenadas *dir_A, Coordenadas *dir_B){
        Coordenadas temp = *dir_A; //Almacenar temporalmente dir_A
        *dir_A = *dir_B; //Asignar dir_B a dir_A
        *dir_B = temp; //Asignar el valor temporal a dir_B
}

//Funcion para liberar laberinto en memoria.
void liberar_laberinto(int **laberinto, int fila){
    for (size_t y = 0; y < fila; y++) {
        free(laberinto[y]); // Liberar cada fila
    }
    free(laberinto); // Liberar el puntero a puntero
}

//Funcion que imprime el laberinto en consola.
void imprimir_matriz(int **laberinto, int fila, int columna, Coordenadas entrada, Coordenadas salida){
    printf("\nLaberinto %d x %d\n", fila, columna); 
    printf("\n");
    for(int y = 0; y < fila; y++){
        for (int x = 0; x < columna; x++){
            if (y == entrada.dir_y && x == entrada.dir_x) printf(ENTRADA); // Imprimir entrada
            else if (y == salida.dir_y && x == salida.dir_x) printf(SALIDA); // Imprimir salida
            else if(laberinto[y][x] == 2) printf(CORRECTO); // Imprimir camino correcto
            else printf("%s", laberinto[y][x] == 1 ? PARED : CAMINO); // Imprimir pared o camino
        }
        printf("\n");
    }
}

//Funcion BFS ...
int bfs(Coordenadas entrada, Coordenadas salida, int fila, int columna){
    Coordenadas *cola = (Coordenadas *)malloc((fila * columna) * sizeof(Coordenadas)); // Cola dinamica para BFS
    int frente = 0; // Indice del frente de la cola
    int atras = 0; // Indice del final de la cola

    bool **visitado = (bool **)malloc(fila * sizeof(bool *)); // Matriz dinamica para marcar nodos visitados
    Coordenadas **caminos = (Coordenadas **)malloc(fila * sizeof(Coordenadas *)); // Matriz dinamica para almacenar caminos

    for(size_t y = 0; y < (size_t)fila; y++){ 
        visitado[y] = (bool *)calloc(columna, sizeof(bool)); // Inicializar matriz visitado con false
        caminos[y] = (Coordenadas *)malloc(columna * sizeof(Coordenadas)); // Inicializar matriz caminos
        for(size_t x = 0; x < (size_t)columna; x++){
            caminos[y][x].dir_x = -1; // Inicializar caminos con -1
            caminos[y][x].dir_y = -1; // Inicializar caminos con -1
        }
    }

    visitado[entrada.dir_y][entrada.dir_x] = true; // Marcar la entrada como visitada
    entrada.dist = 0; // Distancia inicial es 0
    cola[atras++] = entrada; // Encolar la entrada

    int distancia_final = -1; // Distancia final del camino
    bool camino_encontrado = false; // Bandera para indicar si se encontro el camino

    while (frente < atras){ // Mientras la cola no este vacia
        Coordenadas actual = cola[frente++]; // Desencolar el frente de la cola

        if(actual.dir_x == salida.dir_x && actual.dir_y == salida.dir_y){ // Si se llego a la salida
            distancia_final = actual.dist; // Guardar la distancia final
            camino_encontrado = true; // Marcar que se encontro el camino
            break; // Salir del bucle
        }

        size_t tam_mov = sizeof(movimientos) / sizeof(movimientos[0]); // Calcular el tamaño del arreglo de movimientos

        for(size_t i = 0; i < tam_mov; i++){
            int newFila = actual.dir_y + movimientos[i].dir_y; // Calcular nueva fila
            int newCol = actual.dir_x + movimientos[i].dir_x; // Calcular nueva columna

            if(es_valido(newFila, newCol, fila, columna, visitado)){ // Validar nueva posicion
                visitado[newFila][newCol] = true; // Marcar como visitado

                caminos[newFila][newCol].dir_x = actual.dir_x; // Guardar el camino
                caminos[newFila][newCol].dir_y = actual.dir_y; // Guardar el camino

                Coordenadas vecino; // Crear nuevo nodo vecino
                vecino.dir_x = newCol; // Asignar nueva columna
                vecino.dir_y = newFila; // Asignar nueva fila
                vecino.dist = actual.dist + 1; // Incrementar distancia
                cola[atras++] = vecino; // Encolar el vecino
            }
        }
    }

    if(camino_encontrado){ // Si se encontro el camino
        Coordenadas temp = salida; // Empezar desde la salida
        while(!(temp.dir_x == entrada.dir_x && temp.dir_y == entrada.dir_y)){ // Mientras no se llegue a la entrada
            laberinto[temp.dir_y][temp.dir_x] = 2; // Marcar el camino en el laberinto
            temp = caminos[temp.dir_y][temp.dir_x]; // Retroceder al nodo anterior
        }
    }

    for(size_t y = 0; y < (size_t)fila; y++){ // Liberar memoria
        free(visitado[y]); // Liberar cada fila de visitado
        free(caminos[y]); // Liberar cada fila de caminos
    }
    free(visitado); // Liberar puntero a puntero visitado
    free(caminos); // Liberar puntero a puntero caminos
    free(cola); // Liberar cola dinamica

    return distancia_final; // Retornar la distancia final
}

//Funcion auxiliar que valida parametros que necesita la funcion BFS 
bool es_valido(int newFila, int newCol, int fila, int columna, bool **visitado){
    return (newFila >= 0 && newFila < fila && // Verificar limites de fila
        newCol >= 0 && newCol < columna && // Verificar limites de columna
        laberinto[newFila][newCol] != 1 && // Verificar que no sea una pared
        !visitado[newFila][newCol]); // Verificar que no haya sido visitado
}

//Funcion que mide y muestra el rendimiento del programa
void medir_rendimiento(double inicio, double post_creacion, double fin){
    double tiempo_creacion = post_creacion - inicio; // Tiempo de creacion del laberinto    
    double tiempo_bfs = fin - post_creacion; // Tiempo de busqueda BFS
    double tiempo_total = fin - inicio; // Tiempo total
    printf("\nMetrica de Rendimiento.");
    printf("\nTamaño: %d x %d", fila, columna);
    printf("\nTiempo de Creacion del laberinto : %.3f milisegundos.", tiempo_creacion * 1000); // Convertir a milisegundos
    printf("\nTiempo de Busqueda BFS : %.3f milisegundos.", tiempo_bfs * 1000); // Convertir a milisegundos
    printf("\nTiempo de generacion + resolucion; %.3f milisegundos.\n", tiempo_total * 1000);  // Convertir a milisegundos
}

//Funcion que obtiene el tiempo actual en segundos con alta precision
double obtener_tiempo(){
    struct timespec ts; // Estructura para almacenar el tiempo
    clock_gettime(CLOCK_MONOTONIC, &ts); // Obtener el tiempo monotono
    return ts.tv_sec + ts.tv_nsec / 1e9; // Convertir a segundos y retornar
}