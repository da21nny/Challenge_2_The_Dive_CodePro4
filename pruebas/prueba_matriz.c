#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Variables Globales
int **laberinto;
int ancho_global, alto_global;

// Estructura
typedef struct{
    int dir_x;
    int dir_y;
} Coordenadas;

//Prototipo
void intercambioCoordenadas(Coordenadas *, Coordenadas *);
void mezclarCoordenadas(Coordenadas *, size_t);
void generarCaminos(int, int);
int **crearLaberinto(int, int);
void liberar_laberinto(int **, int );
void imprimirMatriz(int **, int, int, Coordenadas, Coordenadas);
void movimientosValidos(int **, int []);

// Funcion Principal o MAIN
int main(){
    srand(time(NULL));
    int ancho_usuario, alto_usuario;

    printf("Bienvenido al juego del Laberinto.\nIngrese Ancho y Alto para Definir el Laberinto\n");
    printf("Entrada : '🚪' - Salida : '🏁' - Pared: '⬜️' - Camino : '  '\n");
    printf("Ingrese Ancho : ");
    scanf("%d", &ancho_usuario);
    printf("Ingrese Alto : ");
    scanf("%d", &alto_usuario);

    int **laberinto = crearLaberinto(ancho_usuario, alto_usuario);

    Coordenadas entrada = {1, 1};
    Coordenadas salida = {ancho_global - 2, alto_global - 2};

    imprimirMatriz(laberinto, ancho_global, alto_global, entrada, salida);

    liberar_laberinto(laberinto, alto_global);

    return 0;
}

void intercambioCoordenadas(Coordenadas *dir_A, Coordenadas *dir_B){
        Coordenadas temp = *dir_A;
        *dir_A = *dir_B;
        *dir_B = temp;
}

void mezclarCoordenadas(Coordenadas dir[], size_t tamanho){
    for(size_t i = 0; i < tamanho - 1; i++){
        size_t j = i + (rand() % (tamanho - i));
        intercambioCoordenadas(&dir[i], &dir[j]);
    }
}

void generarCaminos(int pos_y, int pos_x){
    laberinto[pos_y][pos_x] = 0;

    Coordenadas direccion[] = {{0,2}, {0,-2}, {2,0}, {-2,0}};

    size_t tam = sizeof(direccion) / sizeof(direccion[0]);

    mezclarCoordenadas(direccion, tam);

    for(size_t i = 0; i < tam; i++){
        int dirX = direccion[i].dir_x;
        int dirY = direccion[i].dir_y;
        int newX = pos_x + dirX;
        int newY = pos_y + dirY;
        
        if (newX >= 1 && newX < ancho_global - 1 && newY >= 1 && newY < alto_global - 1){
            if(laberinto[newY][newX] == 1){
                laberinto[pos_y + dirY/2][pos_x + dirX/2] = 0;
                generarCaminos(newY, newX);
            }
        }
    }
}

int **crearLaberinto(int ancho, int alto){
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
    generarCaminos(inicio_y, inicio_x);

    return laberinto;
}

void liberar_laberinto(int **laberinto, int alto){
    for (size_t i = 0; i < alto; i++) {
        free(laberinto[i]);
    }
    free(laberinto);
}

void imprimirMatriz(int **laberinto, int ancho, int alto, Coordenadas entrada, Coordenadas salida){
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
}

void movimientosValidos(int **laberinto, int posicion[]){
    Coordenadas movimientos[] = {{0,1}, {0,-1}, {1,0}, {-1,0}};
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
