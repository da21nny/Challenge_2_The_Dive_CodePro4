#include <stdio.h>

// Prototipo de la función para calcular el factorial
long long calcular_factorial(int numero);

int main() {
    int numero_ingresado;

    // Se solicita la entrada (Estructura de Secuencia)
    printf("Ingrese un número entero positivo: ");
    scanf("%d", &numero_ingresado);

    // Selección (Estructura de Decisión)
    if (numero_ingresado < 0) {
        printf("Error: El factorial no está definido para números negativos.\n");
    } else {
        // Llamada a la función
        long long resultado_factorial = calcular_factorial(numero_ingresado);
        printf("El factorial de %d es: %lld\n", numero_ingresado, resultado_factorial);
    }

    return 0;
}

// Implementación de la función (Modularidad)
long long calcular_factorial(int numero) {
    if (numero == 0 || numero == 1) {
        return 1;
    }

    long long factorial = 1;
    
    // Iteración (Estructura de Bucle)
    for (int i = 2; i <= numero; i++) {
        factorial *= i; // Usando notación abreviada
    }

    return factorial;
}