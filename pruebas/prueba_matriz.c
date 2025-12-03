#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>

// Estructura
typedef struct{
    int dir_x;
    int dir_y;
    int dist;
} Coordenadas;

// Variables Globales
int **laberinto;
int fila_global, columna_global;
Coordenadas direccion[] = {{0,2}, {0,-2}, {2,0}, {-2,0}};
Coordenadas movimientos[] = {{0,1}, {0,-1}, {1,0}, {-1,0}}; // 0{W} - 1{S} - 2{D} - 3{A} 

//Prototipo
void intercambio_coordenadas(Coordenadas *, Coordenadas *);
void mezclar_coordenadas(Coordenadas *, size_t);
void generar_caminos(int, int);
void romper_paredes(int, int);
int **crear_laberinto(int, int);
void liberar_laberinto(int **, int );
void imprimir_matriz(int **, int, int, Coordenadas, Coordenadas, Coordenadas *);
int movimientos_validos(int, int);
bool es_valido(int , int , int , int , bool **);
void bfs(Coordenadas, Coordenadas, int, int);
int movimientos_validos(int, int);
void gameplay(Coordenadas *, Coordenadas, Coordenadas);

// Funcion Principal o MAIN
int main(){
    srand(time(NULL));

    int fila_usuario;
    int columna_usuario;
    int num;

    printf("Bienvenido al juego del Laberinto.\nIngrese Ancho y Alto para Definir el Laberinto\n");
    printf("Jugador : '😊' - Entrada : '🚪' - Salida : '🏁' - Pared: '⬜️' - Camino : '  '\n");

    do{
        printf("Menu:\n1. Para ingresar ancho y alto del Laberinto\n2. Para mostrar laberinto de tamaño fijo (10x10)\n3. Para jugar al Laberinto\n0. Para salir");
        printf("\nElija 1, 2 o 3 - 0 Para Salir: ");
        scanf("%d", &num);
        if(num == 1){
            printf("Ingrese Ancho : ");
            scanf("%d", &fila_usuario);
            printf("Ingrese Alto : ");
            scanf("%d", &columna_usuario);
        } else if(num == 2){
            fila_usuario = 10;
            columna_usuario = 10;
        } else if(num == 3){
            printf("Ingrese Ancho : ");
            scanf(" %d", &fila_usuario);
            printf("Ingrese Alto : ");
            scanf(" %d", &columna_usuario);
        } else if(num == 0){
            break;
        } else{
            printf("Numero incorrecto, ingrese de vuelta.\n");
        }
        
        if(num < 4){
            int **laberinto = crear_laberinto(fila_usuario, columna_usuario);

            Coordenadas entrada = {1, 1};
            Coordenadas salida = {fila_global - 2, columna_global - 2};


            if(num == 3){
                gameplay(&entrada, entrada, salida);
            } else{
                bfs(entrada, salida, fila_global, columna_global);

                imprimir_matriz(laberinto, fila_global, columna_global, entrada, salida, NULL);
            }

            liberar_laberinto(laberinto, columna_global);
        }

    } while (1);

    return 0;
}

void intercambio_coordenadas(Coordenadas *dir_A, Coordenadas *dir_B){
        Coordenadas temp = *dir_A;
        *dir_A = *dir_B;
        *dir_B = temp;
}

void mezclar_coordenadas(Coordenadas dir[], size_t tamanho){
    for(size_t i = 0; i < tamanho - 1; i++){
        size_t j = i + (rand() % (tamanho - i));
        intercambio_coordenadas(&dir[i], &dir[j]);
    }
}

void generar_caminos(int pos_y, int pos_x){
    laberinto[pos_y][pos_x] = 0;

    size_t tam_dir = sizeof(direccion) / sizeof(direccion[0]);

    mezclar_coordenadas(direccion, tam_dir);

    for(size_t i = 0; i < tam_dir; i++){
        int dirX = direccion[i].dir_x;
        int dirY = direccion[i].dir_y;
        int newX = pos_x + dirX;
        int newY = pos_y + dirY;
        
        if (newX >= 1 && newX < fila_global - 1 && newY >= 1 && newY < columna_global - 1){
            if(laberinto[newY][newX] == 1){
                laberinto[pos_y + dirY/2][pos_x + dirX/2] = 0;
                generar_caminos(newY, newX);
            }
        }
    }
}

void romper_paredes(int ancho, int alto){
    int cantidad = (ancho * alto) / 20;
    for(size_t i = 0; i < cantidad; i++){
        int posX = (rand() % (ancho - 2)) + 1;
        int posY = (rand() % (alto - 2)) + 1;
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

int **crear_laberinto(int ancho, int alto){
    if(ancho % 2 == 0) ancho += 1;
    if(alto % 2 == 0) alto += 1;
    
    fila_global = ancho;
    columna_global = alto;

    laberinto = (int **)malloc(alto * sizeof(int *));
    for(size_t i = 0; i < alto; i++){
        laberinto[i] = (int *)malloc(ancho * sizeof(int));
        for(size_t j = 0; j < ancho; j++){
            laberinto[i][j] = 1;
        }
    }
    int inicio_x = 1, inicio_y = 1;
    generar_caminos(inicio_y, inicio_x);

    romper_paredes(ancho, alto);

    return laberinto;
}

void liberar_laberinto(int **laberinto, int alto){
    for (size_t i = 0; i < alto; i++) {
        free(laberinto[i]);
    }
    free(laberinto);
}

void imprimir_matriz(int **laberinto, int ancho, int alto, Coordenadas entrada, Coordenadas salida, Coordenadas *jugador){
    printf("\nLaberinto %d x %d\n", ancho, alto);
    if(jugador != NULL){
        printf("Modo Juego!");
    }
    printf("\n");

    for(int i = 0; i < alto; i++){
        for (int j = 0; j < ancho; j++){
            if (jugador !=NULL && i == jugador->dir_y && j == jugador->dir_x){
                printf("😊");
            }else if (i == entrada.dir_y && j == entrada.dir_x){
                printf("🚪");
            }
            else if (i == salida.dir_y && j == salida.dir_x){
                printf("🏁");
            }
            else if(laberinto[i][j] == 2){
                printf("🟩");
            }
            else{
                printf("%s", laberinto[i][j] == 1 ? "⬜️" : "  ");
            }
        }
        printf("\n");
    }
    /*if(jugador != NULL){
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }*/

    if(jugador == NULL){
    printf("\nObs: Se imprime un Laberinto de medida (Ancho + 1) x (Alto + 1) para que tenga paredes exteriores\n");
    }
}

bool es_valido(int fila, int col, int fila_global, int columna_global, bool **visitado){
    if(fila < 0 || fila >= columna_global || col < 0 || col >= fila_global){
        return false;
    }
    if(laberinto[fila][col] == 1){
        return false;
    }
    if(visitado[fila][col]){
        return false;
    }
    return true;
}

void bfs(Coordenadas entrada, Coordenadas salida, int fila_global, int columna_global){
    Coordenadas cola[fila_global * columna_global];
    int frente = 0;
    int atras = 0;

    bool **visitado = (bool **)malloc(columna_global * sizeof(bool *));
    Coordenadas **caminos = (Coordenadas **)malloc(columna_global * sizeof(Coordenadas *));

    for(size_t k = 0; k < (size_t)columna_global; k++){
        visitado[k] = (bool *)calloc(fila_global, sizeof(bool));
        caminos[k] = (Coordenadas *)malloc(fila_global * sizeof(Coordenadas));
        for(size_t j = 0; j < (size_t)fila_global; j++){
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

            if(es_valido(newFila, newCol, fila_global, columna_global, visitado)){
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
    for(size_t i = 0; i < (size_t)columna_global; i++){
        free(visitado[i]);
        free(caminos[i]);
    }
    free(visitado);
    free(caminos);

    printf("\nEl camino mas corto tiene una longitud de: %d pasos\n", distancia_final);
}

//Opcional
void gameplay(Coordenadas *jugador, Coordenadas entrada, Coordenadas salida){
    char tecla;
    int juego_activo = 1;

    printf("\nModo Juego\n");
    printf("Usa W(arriba) - S(abajo) - A(izquierda) - D(derecha)\n");
    printf("Para salir presiona Q\n");

    while(getchar() != '\n');

    while (juego_activo){

        imprimir_matriz(laberinto, fila_global, columna_global, entrada, salida, jugador);

        if(jugador->dir_x == salida.dir_x && jugador->dir_y == salida.dir_y){
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
                while(getchar() != '\n');
                getchar();
        }

        int nuevoX = jugador->dir_x + movimientos[indice_movimiento].dir_x;
        int nuevoY = jugador->dir_y + movimientos[indice_movimiento].dir_y;
        

        if(movimientos_validos(nuevoX, nuevoY)){
            jugador->dir_x = nuevoX;
            jugador->dir_y = nuevoY;
        }else{
            printf("\nMovimiento invalido, no puede atravesar paredes\nEnter para Continuar\n");
            while(getchar() != '\n');
            getchar();
        }
    }
}

int movimientos_validos(int nuevoX, int nuevoY){
    if(nuevoX < 0 || nuevoX >= fila_global || nuevoY < 0 || nuevoY >= columna_global){
        return 0;
    }

    if (laberinto[nuevoY][nuevoX] == 1){
        return 0;
    }
    
    return 1;    
}
