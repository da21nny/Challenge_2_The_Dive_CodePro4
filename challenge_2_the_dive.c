#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

#define ENTRADA "🚪" // 🚪 o E
#define SALIDA "🏁" // 🏁 o S
#define PARED "⬜️" // ⬜️ o #
#define CORRECTO "🟩" // 🟩 o +
#define CAMINO "  "

// Estructura que contiene Eje X, Eje Y, distancia 
typedef struct{
    int dir_x;
    int dir_y;
    int dist;
} Coordenadas;

// Variables Globales
int **laberinto;
int fila, columna;
Coordenadas direccion[] = {{0,2}, {0,-2}, {2,0}, {-2,0}};
Coordenadas movimientos[] = {{0,1}, {0,-1}, {1,0}, {-1,0}}; // 0{W} - 1{S} - 2{D} - 3{A} 

//Prototipo de funciones
int **crear_laberinto(int fila, int columna);
void generar_caminos(int pos_y, int pos_x);
void mezclar_coordenadas(Coordenadas dir[], size_t tamanho);
void intercambio_coordenadas(Coordenadas *dir_A, Coordenadas *dir_B);
void romper_paredes(int fila, int columna);
void bfs(Coordenadas entrada, Coordenadas salida, int fila, int columna);
bool es_valido(int newFila, int newCol, int fila, int columna, bool **visitado);
void imprimir_matriz(int **laberinto, int fila, int columna, Coordenadas entrada, Coordenadas salida);
void liberar_laberinto(int **laberinto, int columna);
void medir_rendimiento(clock_t inicio, clock_t post_creacion, clock_t fin);

// Funcion Principal o MAIN
int main(int argc, char *argv[]){
    srand(time(NULL));

    int numero;

    if(argc == 1){
        fila = 10;
        columna = 10;
    } else if (argc == 3){
        fila = atoi(argv[1]);
        columna = atoi(argv[2]);
        if (fila <= 4 || columna <= 4){
            printf("\nFila o Columna debe ser mayor a 4");
            return 1;
        }
        printf("Tamaño ingresado: %d x %d.", fila, columna);
    } else{
        printf("El programa requiere 2 argumentos\n");
        printf("Ejemplo ./programa 20 20\n");
        return 1;
    }
        
    if(fila % 2 == 0) fila += 1;
    if(columna % 2 == 0) columna += 1;

    clock_t inicio = clock();
    int **laberinto = crear_laberinto(fila, columna);
    clock_t post_creacion = clock();

    Coordenadas entrada = {1, 1};
    Coordenadas salida = {fila - 2, columna - 2};

    bfs(entrada, salida, fila, columna);
    clock_t fin = clock();

    imprimir_matriz(laberinto, fila, columna, entrada, salida);
    medir_rendimiento(inicio, post_creacion, fin);
        
    liberar_laberinto(laberinto, columna);
    
    return 0;
}

// Funcion que crea un laberinto llenos de 1 (Paredes) de tamaño MxN
int **crear_laberinto(int fila, int columna){
    laberinto = (int **)malloc(fila * sizeof(int *));
    for(size_t i = 0; i < fila; i++){
        laberinto[i] = (int *)malloc(columna * sizeof(int));
        for(size_t j = 0; j < columna; j++){
            laberinto[i][j] = 1;
        }
    }
    int inicio_x = 1, inicio_y = 1;
    generar_caminos(inicio_y, inicio_x);

    return laberinto;
}

//Funcion que genera de forma aleatoria los caminos (0) en el laberinto
void generar_caminos(int pos_y, int pos_x){
    laberinto[pos_y][pos_x] = 0;

    size_t tam_dir = sizeof(direccion) / sizeof(direccion[0]);

    mezclar_coordenadas(direccion, tam_dir);

    for(size_t i = 0; i < tam_dir; i++){
        int dirX = direccion[i].dir_x;
        int dirY = direccion[i].dir_y;
        int newX = pos_x + dirX;
        int newY = pos_y + dirY;
        
        if (newX >= 1 && newX < columna && newY >= 1 && newY < fila){
            if(laberinto[newY][newX] == 1){
                laberinto[pos_y + dirY/2][pos_x + dirX/2] = 0;
                generar_caminos(newY, newX);
            }
        }
    }
}

//Funcion que mezcla las coordenadas para producir caminos aleatorios.
void mezclar_coordenadas(Coordenadas dir[], size_t tamanho){
    for(size_t i = 0; i < tamanho - 1; i++){
        size_t j = i + (rand() % (tamanho - i));
        intercambio_coordenadas(&dir[i], &dir[j]);
    }
}

//Funcion auxiliar para intercambiar las coordenadas.
void intercambio_coordenadas(Coordenadas *dir_A, Coordenadas *dir_B){
        Coordenadas temp = *dir_A;
        *dir_A = *dir_B;
        *dir_B = temp;
}

//Funcion para liberar laberinto en memoria.
void liberar_laberinto(int **laberinto, int columna){
    for (size_t i = 0; i < columna; i++) {
        free(laberinto[i]);
    }
    free(laberinto);
}

//Funcion que imprime el laberinto en consola.
void imprimir_matriz(int **laberinto, int fila, int columna, Coordenadas entrada, Coordenadas salida){
    printf("\nLaberinto %d x %d\n", fila, columna);
    printf("\n");
    for(int i = 0; i < columna; i++){
        for (int j = 0; j < fila; j++){
            if (i == entrada.dir_y && j == entrada.dir_x){
                printf(ENTRADA);
            }
            else if (i == salida.dir_y && j == salida.dir_x){
                printf(SALIDA);
            }
            else if(laberinto[i][j] == 2){
                printf(CORRECTO);
            }
            else{
                printf("%s", laberinto[i][j] == 1 ? PARED : CAMINO);
            }
        }
        printf("\n");
    }

    printf("\nObs: Se imprime un Laberinto de medida (fila + 1) x (columna + 1) para que tenga paredes exteriores\n");

}

//Funcion BFS ...
void bfs(Coordenadas entrada, Coordenadas salida, int fila, int columna){
    Coordenadas *cola = (Coordenadas *)malloc((fila * columna) * sizeof(Coordenadas));
    int frente = 0;
    int atras = 0;

    bool **visitado = (bool **)malloc(columna * sizeof(bool *));
    Coordenadas **caminos = (Coordenadas **)malloc(columna * sizeof(Coordenadas *));

    for(size_t k = 0; k < (size_t)columna; k++){
        visitado[k] = (bool *)calloc(fila, sizeof(bool));
        caminos[k] = (Coordenadas *)malloc(fila * sizeof(Coordenadas));
        for(size_t j = 0; j < (size_t)fila; j++){
            caminos[k][j].dir_x = -1;
            caminos[k][j].dir_y = -1;
        }
    }

    visitado[entrada.dir_y][entrada.dir_x] = true;
    entrada.dist = 0;
    cola[atras++] = entrada;

    int distancia_final = -1;
    bool camino_encontrado = false;

    while (frente < atras){
        Coordenadas actual = cola[frente++];

        if(actual.dir_x == salida.dir_x && actual.dir_y == salida.dir_y){
            distancia_final = actual.dist;
            camino_encontrado = true;
            break;
        }

        size_t tam_mov = sizeof(movimientos) / sizeof(movimientos[0]);

        for(size_t i = 0; i < tam_mov; i++){
            int newFila = actual.dir_y + movimientos[i].dir_y;
            int newCol = actual.dir_x + movimientos[i].dir_x; 

            if(es_valido(newFila, newCol, fila, columna, visitado)){
                visitado[newFila][newCol] = true;

                caminos[newFila][newCol].dir_x = actual.dir_x;
                caminos[newFila][newCol].dir_y = actual.dir_y;

                Coordenadas vecino;
                vecino.dir_x = newCol;
                vecino.dir_y = newFila;
                vecino.dist = actual.dist + 1;
                cola[atras++] = vecino;
            }
        }
    }

    if(camino_encontrado){
        Coordenadas temp = salida;
        while(!(temp.dir_x == entrada.dir_x && temp.dir_y == entrada.dir_y)){
            temp = caminos[temp.dir_y][temp.dir_x];
        }

        temp = salida;
        while(!(temp.dir_x == entrada.dir_x && temp.dir_y == entrada.dir_y)){
            laberinto[temp.dir_y][temp.dir_x] = 2;
            temp = caminos[temp.dir_y][temp.dir_x];
        }
        laberinto[entrada.dir_y][entrada.dir_x] = 2;
    }
    for(size_t i = 0; i < (size_t)columna; i++){
        free(visitado[i]);
        free(caminos[i]);
    }
    free(visitado);
    free(caminos);
    free(cola);

    printf("\nEl camino mas corto tiene una longitud de: %d pasos\n", distancia_final);
}

//Funcion auxiliar que valida parametros que necesita la funcion BFS 
bool es_valido(int newFila, int newCol, int fila, int columna, bool **visitado){
    if(newFila < 0 || newFila >= fila || newCol < 0 || newCol >= columna){
        return false;
    }
    if(laberinto[newFila][newCol] == 1){
        return false;
    }
    if(visitado[newFila][newCol]){
        return false;
    }
    return true;
}

void medir_rendimiento(clock_t inicio, clock_t post_creacion, clock_t fin){
    double tiempo_creacion = ((double)(post_creacion - inicio)) / CLOCKS_PER_SEC;
    double tiempo_bfs = ((double)(fin - post_creacion)) / CLOCKS_PER_SEC;
    double tiempo_total = ((double)(fin - inicio))/ CLOCKS_PER_SEC;
    printf("\nMetrica de Rendimiento.");
    printf("\nTamaño: %d x %d", fila, columna);
    printf("\nTiempo de Creacion del laberinto : %.4f segundos", tiempo_creacion);
    printf("\nTiempo de Busqueda BFS : %.4f segundos", tiempo_bfs);
    printf("\nTiempo de generacion + resolucion; %.4f segundos.\n", tiempo_total);
}
