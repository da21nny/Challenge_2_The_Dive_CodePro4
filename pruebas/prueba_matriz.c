#include <stdio.h>
#include <string.h>

#define ANCHO 11
#define ALTO 11

//Prototipo
void imprimirMatriz(int[ANCHO][ALTO]);
void crearLaberinto(int[ANCHO][ALTO]);
void generarCaminos(int[ANCHO][ALTO], int, int);


void crearLaberinto(int laberinto_ficticio[ANCHO][ALTO]){
    int i, j;
    /*if((ancho %= 2) == 0){
        ancho += 1;
    }
    if((alto %= 2) == 0){
        alto += 1;
    }*/
    for(i = 0; i < ANCHO; i++){
        for(j = 0; j < ALTO; j++){
            laberinto_ficticio [i][j] = 1;
        }
    }
}

void intercambio(int *valor_A, int *valor_B){
    int temp = *valor_A;
    *valor_A = *valor_B;
    *valor_B = temp;
}

void generarCaminos(int laberinto_camino[ANCHO][ALTO], int pos_x, int pos_y){
    laberinto_camino[pos_x][pos_y] = 0;
    const direccion[4][2] = {{0,2}, {0,-2}, {2,0}, {-2,0}};


}

int main(){
    int matriz_A[ANCHO][ALTO] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
              {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
              {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
              {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
              {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
              {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
              {1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1},
              {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
              {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    printf("\nMatriz\n");
    imprimirMatriz(matriz_A);
    printf("\n\n");

    int laberinto[ANCHO][ALTO];
    crearLaberinto(laberinto);
    int i, j;
    for(i = 0; i < ANCHO; i++){
        for (j = 0; j < ALTO; j++){
            printf("%d", laberinto[i][j]);
        }
        printf("\n");
    }
    return 0;
}

void imprimirMatriz(int matriz_B[ANCHO][ALTO]){
    int i, j;
    for(i = 0; i < ANCHO; i++){
        char dibujo[80] = "";
        for (j = 0; j < ALTO; j++){
            if (matriz_B[i][j] == 1){
                strncat(dibujo, "⬜️", 7); 
            }
            else{
                strncat(dibujo, "  ", 3);
            }
        }
        printf("%s\n", dibujo);
    }
}

