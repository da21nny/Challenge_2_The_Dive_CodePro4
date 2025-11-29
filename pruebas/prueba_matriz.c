#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Estructura
typedef struct{
    int dir_x;
    int dir_y;
} Coordenadas;

// Variables Globales
int **laberinto;
int ancho_global, alto_global;
Coordenadas direccion[] = {{0,2}, {0,-2}, {2,0}, {-2,0}};
Coordenadas movimientos[] = {{0,1}, {0,-1}, {1,0}, {-1,0}};

//Prototipo
void intercambio_coordenadas(Coordenadas *, Coordenadas *);
void mezclar_coordenadas(Coordenadas *, size_t);
void generar_caminos(int, int);
void romper_paredes(int, int);
int **crear_laberinto(int, int);
void liberar_laberinto(int **, int );
void imprimir_matriz(int **, int, int, Coordenadas, Coordenadas);
void movimientos_validos(int **, int []);

// Funcion Principal o MAIN
int main(){
    srand(time(NULL));

    int ancho_usuario;
    int alto_usuario;
    int num;

    printf("Bienvenido al juego del Laberinto.\nIngrese Ancho y Alto para Definir el Laberinto\n");
    printf("Entrada : '🚪' - Salida : '🏁' - Pared: '⬜️' - Camino : '  '\n");

    printf("Menu:\n1. Para ingresar ancho y alto del Laberinto\n2. Para mostrar laberinto de tamaño fijo (10x10)\n");
    do{
        printf("Elija 1 o 2 : ");
        scanf("%d", &num);
        if(num == 1){
            printf("Ingrese Ancho : ");
            scanf("%d", &ancho_usuario);
            printf("Ingrese Alto : ");
            scanf("%d", &alto_usuario);
        } else if(num == 2){
            ancho_usuario = 10;
            alto_usuario = 10;
        } else{
            printf("Numero incorrecto, ingrese de vuelta.\n");
        }
    } while (num > 3);    

    int **laberinto = crear_laberinto(ancho_usuario, alto_usuario);

    Coordenadas entrada = {1, 1};
    Coordenadas salida = {ancho_global - 2, alto_global - 2};

    imprimir_matriz(laberinto, ancho_global, alto_global, entrada, salida);

    liberar_laberinto(laberinto, alto_global);

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
        
        if (newX >= 1 && newX < ancho_global - 1 && newY >= 1 && newY < alto_global - 1){
            if(laberinto[newY][newX] == 1){
                laberinto[pos_y + dirY/2][pos_x + dirX/2] = 0;
                generar_caminos(newY, newX);
            }
        }
    }
}

void romper_paredes(int ancho, int alto){
    int cantidad = (ancho * alto) / 15;
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
    
    ancho_global = ancho;
    alto_global = alto;

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

void imprimir_matriz(int **laberinto, int ancho, int alto, Coordenadas entrada, Coordenadas salida){
    printf("\nLaberinto %d x %d\n", ancho, alto);
    for(int i = 0; i < alto; i++){
        for (int j = 0; j < ancho; j++){            
            //printf("%s", laberinto[i][j] == 1 ? "⬜️" : "  ");
            if (i == entrada.dir_y && j == entrada.dir_x){
                printf("🚪");
            }
            else if (i == salida.dir_y && j == salida.dir_x){
                printf("🏁");
            }
            else{
                printf("%s", laberinto[i][j] == 1 ? "⬜️" : "  ");
            }
        }
        printf("\n");
    }
    printf("\nObs: Se imprime un Laberinto de medida (Ancho + 1) x (Alto + 1) para que tenga paredes exteriores\n");
}

void movimientos_validos(int **laberinto, int posicion[]){
    Coordenadas validos[] = {};

    size_t tam_mov = sizeof(movimientos) / sizeof(movimientos[0]);
    size_t tam_lab = sizeof(laberinto) / sizeof(laberinto[0]);

    for(size_t i = 0; i < tam_mov; i++){
        int dirX = movimientos[i].dir_x;
        int dirY = movimientos[i].dir_y;
        int newX = posicion[0] + dirX;
        int newY = posicion[1] + dirY;

        if(newX >= 0 && newX < tam_lab && newY >= 0 && newY < tam_lab){
            if(laberinto[newX][newY] == 0){
                //validos->dir_x = newX;
                //validos->dir_y = newY;
                validos[i].dir_x = newX;
                validos[i].dir_y = newY;
            }
        }
    }
}
