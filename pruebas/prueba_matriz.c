#include <stdio.h>
#include <string.h>

#define M 11

int ancho = 11;
int alto = 11;
int i, j;

//Prototipo
void imprimirMatriz(int[M][M]);
void generarMatriz(int[ancho][alto]);

void generarMatriz(int laberinto_ficticio[ancho][alto]){
    /*if((ancho %= 2) == 0){
        ancho += 1;
    }
    if((alto %= 2) == 0){
        alto += 1;
    }*/
    for(i = 0; i < ancho; i++){
        for(j = 0; j < alto; j++){
            laberinto_ficticio [i][j] = 1;
        }
    }
}

int main(){
    int matriz_A[M][M] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
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

    int laberinto[ancho][alto];
    generarMatriz(laberinto);
    for(i = 0; i < ancho; i++){
        for (j = 0; j < alto; j++){
            printf("%d", laberinto[i][j]);
        }
        printf("\n");
    }
    return 0;
}

void imprimirMatriz(int matriz_B[M][M]){
    for(i = 0; i < M; i++){
        char dibujo[80] = "";
        for (j = 0; j < M; j++){
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

