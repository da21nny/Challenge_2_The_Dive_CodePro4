#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>

#define JUGADOR "😊" // 😊 o J
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
int **crear_laberinto(int fila, int columna, int seguro);
void generar_caminos(int pos_y, int pos_x);
void mezclar_coordenadas(Coordenadas dir[], size_t tamanho);
void intercambio_coordenadas(Coordenadas *dir_A, Coordenadas *dir_B);
void romper_paredes(int fila, int columna);
void bfs(Coordenadas entrada, Coordenadas salida, int fila, int columna);
void imprimir_matriz(int **laberinto, int fila, int columna, Coordenadas entrada, Coordenadas salida, Coordenadas *jugador);
void liberar_laberinto(int **laberinto, int columna);
int movimientos_validos(int nuevoX, int nuevoY);
void gameplay(Coordenadas jugador, Coordenadas entrada, Coordenadas salida);
bool es_valido(int newFila, int newCol, int fila, int columna, bool **visitados);
void medir_rendimiento(clock_t inicio, clock_t post_creacion, clock_t fin);

// Funcion Principal o MAIN
int main(int argc, char *argv[]){
    srand(time(NULL));

    int numero;
    int seguro;

    printf("Bienvenido al juego del Laberinto.\nDetalles a tener en cuenta\n");
    printf("Jugador : '%s' - Entrada : '%s' - Salida : '%s' - Pared: '%s' - Camino Correcto : '%s' - Camino : '%s'.\n", JUGADOR, ENTRADA, SALIDA, PARED, CORRECTO, CAMINO);

    do{
        printf("\nMenu:\n1. Para ingresar fila y columna del Laberinto\n2. Para mostrar laberinto de tamaño fijo (10x10)\n3. Para jugar al Laberinto\n0. Para salir");
        printf("\nElija 1, 2 o 3 - 0 Para Salir: ");
        scanf("%d", &numero);
        if(numero == 1 || numero == 3){
            printf("Ingrese fila : ");
            scanf("%d", &fila);
            printf("Ingrese columna : ");
            scanf("%d", &columna);
        } else if(numero == 2){
            fila = 10;
            columna = 10;
        } else if(numero == 0){
            printf("\nSaliendo...\n");
            break;
        } else{
            printf("Numero incorrecto, ingrese de vuelta.\n");
            continue;
        }

        printf("\n1- Modo Seguro (Unico Camino)\n2- Modo aleatorio (Multiples Caminos)\n");
        printf("\nIngrese numero : ");
        scanf("%d", &seguro);
        
        if(fila % 2 == 0) fila += 1;
        if(columna % 2 == 0) columna += 1;

        clock_t inicio = clock();
        int **laberinto = crear_laberinto(fila, columna, seguro);
        clock_t post_creacion = clock();

        Coordenadas entrada = {1, 1};
        Coordenadas salida = {fila - 2, columna - 2};
        Coordenadas jugador = {1,1};

        if(numero == 3){
            gameplay(jugador, entrada, salida);
        } else{
            bfs(entrada, salida, fila, columna);
            clock_t fin = clock();
            imprimir_matriz(laberinto, fila, columna, entrada, salida, NULL);
            medir_rendimiento(inicio, post_creacion, fin);
        }

        liberar_laberinto(laberinto, columna);
    
    } while (1);

    return 0;
}

// Funcion que crea un laberinto llenos de 1 (Paredes) de tamaño MxN
int **crear_laberinto(int fila, int columna, int seguro){
    laberinto = (int **)malloc(fila * sizeof(int *));
    for(size_t i = 0; i < fila; i++){
        laberinto[i] = (int *)malloc(columna * sizeof(int));
        for(size_t j = 0; j < columna; j++){
            laberinto[i][j] = 1;
        }
    }
    int inicio_x = 1, inicio_y = 1;
    generar_caminos(inicio_y, inicio_x);

    if(seguro == 2){
        romper_paredes(fila, columna);
    }

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
        
        if (newX >= 1 && newX < fila && newY >= 1 && newY < columna){
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

//Funcion opcional para generar mas caminos alternativos en el laberinto
void romper_paredes(int fila, int columna){
    int cantidad = (fila * columna) / 20;
    for(size_t i = 0; i < cantidad; i++){
        int posX = (rand() % (fila - 2)) + 1;
        int posY = (rand() % (columna - 2)) + 1;
        if(laberinto[posY][posX] == 1){
            int libres = 0;
            size_t tam_mov = sizeof(movimientos) / sizeof(movimientos[0]);
            for(size_t k = 0; k < tam_mov; k++){
                int dirX = movimientos[k].dir_x;
                int dirY = movimientos[k].dir_y;
                int newX = posX + dirX;
                int newY = posY + dirY;
                if(laberinto[newY][newX] == 0){
                    libres++;
                }
            }
            if(libres >= 1 && libres <= 2){
                laberinto[posY][posX] = 0;
            }
        }
    }
}

//Funcion para liberar laberinto en memoria.
void liberar_laberinto(int **laberinto, int columna){
    for (size_t i = 0; i < columna; i++) {
        free(laberinto[i]);
    }
    free(laberinto);
}

//Funcion que imprime el laberinto en consola.
void imprimir_matriz(int **laberinto, int fila, int columna, Coordenadas entrada, Coordenadas salida, Coordenadas *jugador){
    printf("\nLaberinto %d x %d\n", fila, columna);
    if(jugador != NULL){
        printf("Modo Juego!");
    }
    printf("\n");
    for(int i = 0; i < columna; i++){
        for (int j = 0; j < fila; j++){
            if (jugador !=NULL && i == jugador->dir_y && j == jugador->dir_x){
                printf(JUGADOR);
            }else if (i == entrada.dir_y && j == entrada.dir_x){
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

    if(jugador == NULL){
    printf("\nObs: Se imprime un Laberinto de medida (fila + 1) x (columna + 1) para que tenga paredes exteriores\n");
    }
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
    if(newFila < 0 || newFila >= columna || newCol < 0 || newCol >= fila){
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

//Opcional. Juego que permite al usuario manejarse dentro del laberinto.
void gameplay(Coordenadas jugador, Coordenadas entrada, Coordenadas salida){
    int juego_activo = 1;

    printf("\nModo Juego\n");
    printf("Usa W(arriba) - S(abajo) - A(izquierda) - D(derecha)\n");
    printf("Para salir presiona Q\n");

    while(getchar() != '\n');

    while (juego_activo){

        imprimir_matriz(laberinto, fila, columna, entrada, salida, &jugador);

        if(jugador.dir_x == salida.dir_x && jugador.dir_y == salida.dir_y){
            printf("\nFelicidades, llegaste a la salida\n");
            break;
        }   

        printf("Movimiento (W/S/D/A) o Q para salir : ");
        char tecla = tolower(getch());

        if(tecla == 'q'){
            printf("\nSaliendo del Juego\n");
            break;
        }

        int indice_movimiento = -1;
        switch (tecla){
            case 'w': indice_movimiento = 1; break;
            case 's': indice_movimiento = 0; break;
            case 'd': indice_movimiento = 2; break;
            case 'a': indice_movimiento = 3; break;
            default:
                printf("Tecla incorrecta\nEnter para continuar\n");
                continue;
        }

        int nuevoX = jugador.dir_x + movimientos[indice_movimiento].dir_x;
        int nuevoY = jugador.dir_y + movimientos[indice_movimiento].dir_y;
        

        if(movimientos_validos(nuevoX, nuevoY)){
            jugador.dir_x = nuevoX;
            jugador.dir_y = nuevoY;
        }else{
            printf("\nMovimiento invalido, no puede atravesar paredes\n");
            continue;
        }
    }
}

//Funcion que valida los movimientos que el jugador hace dentro del laberinto.
int movimientos_validos(int nuevoX, int nuevoY){
    if(nuevoX < 0 || nuevoX >= fila || nuevoY < 0 || nuevoY >= columna){ 
        return 0;
    }

    if (laberinto[nuevoY][nuevoX] == 1){
        return 0;
    }
    
    return 1;    
}
