1. Escenario & Reto
Tras la épica batalla mental entre el Gato y el Ratón, algo inesperado ocurrió.

El laboratorio subterráneo donde se libró su duelo empezó a temblar. Las luces parpadearon, las pantallas mostraron símbolos extraños, y justo cuando pensabas que era el final… un nuevo archivo apareció misteriosamente en tu carpeta de proyectos:

laberinto.c

Al abrirlo, encuentras solo una línea:

"Este es solo el comienzo. Escapa si puedes."

Ahora estás atrapado en un nuevo reto: el Laberinto Maestro, un mundo generado por código donde cada paso puede llevarte a la libertad… o a una pared.

Tu misión, valiente desarrollador, es crear un generador de laberintos aleatorios totalmente resolubles, y luego diseñar un algoritmo que los resuelva automáticamente. Esto no es un simple mapa: es un desafío de lógica, diseño y optimización que solo un verdadero estratega del código puede superar.

¿Lograrás construir el camino hacia la salida? ¿O te perderás en tu propio código?

2. Habilidades que Vas a Necesitar
Programación estructurada en C o C++ (el lenguaje clásico de los valientes).

Backtracking y BFS (porque no todo en la vida se resuelve con fuerza bruta).

Diseño algorítmico para generar estructuras lógicas sin callejones sin salida.

Visualización creativa con ASCII o emojis (¡sí, programar también puede ser bonito!).

Optimización de recursos y tiempos de ejecución (porque hasta los laberintos tienen deadlines).

3. Requisitos Obligatorios (Tu Mapa de Supervivencia)
Generación del Laberinto:
El laberinto debe ser generado de forma aleatoria en cada ejecución.

Debe tener una entrada (esquina superior izquierda) y una salida (esquina inferior derecha), siempre conectadas.

Asegúrate de que haya al menos un camino válido entre ambos extremos.

Resolución Automática del Laberinto:
Implementa un algoritmo que resuelva automáticamente el laberinto.

Puedes usar Backtracking, BFS o cualquier enfoque lógico que te lleve al éxito.

Tamaño Configurable:
El laberinto debe comenzar con un tamaño fijo (por ejemplo, 10x10).

Luego, modifícalo para que su tamaño se defina con parámetros externos, sin tocar el código fuente.

Visualización Clara:
Representa el laberinto en consola usando caracteres ASCII o emojis.

Muros: #

Caminos: *

Entrada y salida marcadas claramente.

La visualización debe permitir ver el recorrido resuelto.

Organización & Optimización:
Separa el programa en partes claras:

Generación del laberinto

Visualización

Resolución

Toma mediciones de tiempo de ejecución para laberintos de distintos tamaños.

Usa estructuras de datos eficientes para velocidad y legibilidad.

4. Entregables, Reglas y Bonus Opcionales
Entregables:

Un archivo .c o .cpp con tu implementación.

Un README corto explicando:

Cómo generaste el laberinto.

Qué algoritmo usaste para resolverlo.

Qué descubriste al medir tiempos y qué harías distinto la próxima vez.

Reglas:

No se permite usar bibliotecas externas para generación o resolución automática. Este reto se resuelve con código propio y creatividad.

El código debe ser legible, comentado y modular.

Bonus Opcionales:

Añade obstáculos especiales (puertas, trampas, portales).

Agrega una función de modo aleatorio vs. modo seguro (uno genera un solo camino, el otro múltiples rutas).

Implementa una interfaz por consola donde el usuario pueda explorar el laberinto manualmente antes de activar el solucionador automático.

¿Quieres ir más allá? Crea un modo visual animado en consola que muestre paso a paso cómo se resuelve el laberinto.

🚀 El Código Tiene Salida… Si Sabes Buscarla
Los muros no son el enemigo. La confusión, sí.
Con C como tu brújula y tu lógica como linterna, crea un sistema que no solo construya laberintos, sino que también sepa cómo salir de ellos.

Porque en Penguin Academy no solo escribimos líneas de código...
También escribimos historias que se compilan.
