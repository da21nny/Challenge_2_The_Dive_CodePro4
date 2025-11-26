#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define ANCHO 11
//#define ALTO 11

int **laberinto;
int ancho_global, alto_global;

//Prototipo
void imprimirMatriz(int **, int, int);
int **crearLaberinto(int, int);

typedef struct{
    int dir_x;
    int dir_y;
} Coordenadas;

void mezclarCoordenadas(Coordenadas *dir, int tamanho){
    for(int i = tamanho - 1; i > 0; i--){
        int j = rand() % (i + 1);
        Coordenadas temp = dir[i];
        dir[i] = dir[j];
        dir[j] = temp;
    }
}

void generarCaminos(int pos_y, int pos_x){
    laberinto[pos_y][pos_x] = 0;

    Coordenadas direccion[] = {{0,2}, {0,-2}, {2,0}, {-2,0}};

    mezclarCoordenadas(direccion, 4);

    for(int i = 0; i < 4; i++){
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
    for(int i = 0; i < alto; i++){
        laberinto[i] = (int *)malloc(ancho * sizeof(int));
        for(int j = 0; j < ancho; j++){
            laberinto[i][j] = 1;
        }
    }
    int inicio_x = 1, inicio_y = 1;
    generarCaminos(inicio_y, inicio_x);

    return laberinto;
}

void liberar_laberinto(int **laberinto, int alto) {
    for (int i = 0; i < alto; i++) {
        free(laberinto[i]);
    }
    free(laberinto);
}

int main(){
    int **laberinto = crearLaberinto(10, 10);

    imprimirMatriz(laberinto, ancho_global, alto_global);

    liberar_laberinto(laberinto, alto_global);

    return 0;
}

void imprimirMatriz(int **laberinto, int ancho, int alto){
    for(int i = 0; i < alto; i++){
    //char dibujo[200] = "";
        for (int j = 0; j < ancho; j++){
            
            printf("%s", laberinto[i][j] == 1 ? "⬜️" : "  ");
            /*if (laberinto[i][j] == 1){
                strncat(dibujo, "⬜️", 7); 
            }
            else{
                strncat(dibujo, "  ", 3);
            }*/
        }
        printf("\n");
        //printf("%s\n", dibujo);
    }
}
