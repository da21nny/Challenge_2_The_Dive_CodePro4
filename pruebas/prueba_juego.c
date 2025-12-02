#include <stdio.h>
#include <conio.h>  // Para getch() en Windows
#include <stdlib.h>

#define FILAS 10
#define COLUMNAS 10

// Símbolos del juego
#define PARED '#'
#define JUGADOR 'P'
#define VACIO ' '
#define SALIDA 'S'

// Variables globales
char laberinto[FILAS][COLUMNAS];
int jugadorX, jugadorY;

// Función para inicializar el laberinto
void inicializarLaberinto() {
    // Ejemplo de laberinto (0 = vacío, 1 = pared)
    int mapa[FILAS][COLUMNAS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 1, 2, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    
    // Convertir matriz numérica a caracteres
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (mapa[i][j] == 1) {
                laberinto[i][j] = PARED;
            } else if (mapa[i][j] == 2) {
                laberinto[i][j] = SALIDA;
            } else {
                laberinto[i][j] = VACIO;
            }
        }
    }
    
    // Posición inicial del jugador
    jugadorX = 1;
    jugadorY = 1;
}

// Función para limpiar la pantalla
void limpiarPantalla() {
    system("cls");  // Windows
    // system("clear"); // Linux/Mac
}

// Función para dibujar el laberinto
void dibujarLaberinto() {
    limpiarPantalla();
    
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            // Si es la posición del jugador, dibujar jugador
            if (i == jugadorY && j == jugadorX) {
                printf("%c ", JUGADOR);
            } else {
                printf("%c ", laberinto[i][j]);
            }
        }
        printf("\n");
    }
    
    printf("\nUsa W(arriba) S(abajo) A(izquierda) D(derecha)");
    printf("\nPresiona Q para salir\n");
}

// Función para verificar si el movimiento es válido
int movimientoValido(int x, int y) {
    // Verificar límites
    if (x < 0 || x >= COLUMNAS || y < 0 || y >= FILAS) {
        return 0;
    }
    
    // Verificar si no es pared
    if (laberinto[y][x] == PARED) {
        return 0;
    }
    
    return 1;
}

// Función para mover el jugador
void moverJugador(char direccion) {
    int nuevoX = jugadorX;
    int nuevoY = jugadorY;
    
    // Calcular nueva posición según la tecla presionada
    switch(direccion) {
        case 'w':
        case 'W':
            nuevoY--;  // Mover arriba
            break;
        case 's':
        case 'S':
            nuevoY++;  // Mover abajo
            break;
        case 'a':
        case 'A':
            nuevoX--;  // Mover izquierda
            break;
        case 'd':
        case 'D':
            nuevoX++;  // Mover derecha
            break;
        default:
            return;  // Tecla no válida
    }
    
    // Si el movimiento es válido, actualizar posición
    if (movimientoValido(nuevoX, nuevoY)) {
        jugadorX = nuevoX;
        jugadorY = nuevoY;
    }
}

// Función para verificar si el jugador llegó a la salida
int verificarVictoria() {
    if (laberinto[jugadorY][jugadorX] == SALIDA) {
        return 1;
    }
    return 0;
}

// Función principal
int main() {
    char tecla;
    int juegoActivo = 1;
    
    // Inicializar el laberinto
    inicializarLaberinto();
    
    // Bucle principal del juego
    while (juegoActivo) {
        // Dibujar el estado actual
        dibujarLaberinto();
        
        // Verificar victoria
        if (verificarVictoria()) {
            printf("\n¡FELICIDADES! ¡Llegaste a la salida!\n");
            juegoActivo = 0;
            break;
        }
        
        // Leer tecla presionada
        tecla = getch();  // Lee una tecla sin esperar Enter
        
        // Salir del juego
        if (tecla == 'q' || tecla == 'Q') {
            printf("\nSaliendo del juego...\n");
            juegoActivo = 0;
        } else {
            // Mover jugador
            moverJugador(tecla);
        }
    }
    
    return 0;
}