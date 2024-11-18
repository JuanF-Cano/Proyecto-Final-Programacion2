/* #include <stdio.h>
#include <stdlib.h>

typedef struct {
    char nombre[50];
    int id_comprador;
} Comprador;

typedef struct {
    char nombre_producto[50];
    int id_producto;
    Comprador *compradores; // puntero a un array de compradores
    int total_compradores;
} Producto;

int main() {
    Producto producto;
    producto.total_compradores = 0;
    producto.compradores = NULL; // Inicialización del puntero a NULL

    // Agregar dinámicamente compradores
    for (int i = 0; i < 3; i++) {
        // Reservar espacio para un nuevo comprador
        producto.compradores = realloc(producto.compradores, (producto.total_compradores + 1) * sizeof(Comprador));

        // Llenar los datos del comprador
        sprintf(producto.compradores[producto.total_compradores].nombre, "Comprador %d", i + 1);
        producto.compradores[producto.total_compradores].id_comprador = 100 + i;
        producto.total_compradores++;
    }

    // Imprimir los datos de los compradores
    for (int i = 0; i < producto.total_compradores; i++) {
        printf("Comprador %d: %s (ID: %d)\n", i + 1, producto.compradores[i].nombre, producto.compradores[i].id_comprador);
    }

    // Liberar la memoria
    free(producto.compradores);

    return 0;
}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

// Estructura de un nodo de la tabla hash
typedef struct Node {
    int id_producto;
    char nombre_producto[50];
    struct Node *next; // Para manejar colisiones (encadenamiento)
} Node;

// Crear una función hash simple
unsigned int hash(int id_producto) {
    return id_producto % SIZE;
}

// Insertar en la tabla hash
void insertar(Node *tabla_hash[], int id_producto, const char *nombre_producto) {
    unsigned int indice = hash(id_producto);
    Node *nuevo = malloc(sizeof(Node));
    if (nuevo == NULL) {
        perror("Error de memoria");
        exit(EXIT_FAILURE);
    }
    nuevo->id_producto = id_producto;
    strcpy(nuevo->nombre_producto, nombre_producto);
    nuevo->next = tabla_hash[indice]; // Insertar al inicio de la lista
    tabla_hash[indice] = nuevo;
}

// Buscar en la tabla hash
Node *buscar(Node *tabla_hash[], int id_producto) {
    unsigned int indice = hash(id_producto);
    Node *temp = tabla_hash[indice];
    while (temp != NULL) {
        if (temp->id_producto == id_producto) {
            return temp; // Encontrado
        }
        temp = temp->next;
    }
    return NULL; // No encontrado
}

int main() {
    Node *tabla_hash[SIZE] = {NULL}; // Inicializar la tabla hash con NULL

    // Insertar datos en la tabla hash
    insertar(tabla_hash, 101, "Laptop");
    insertar(tabla_hash, 202, "Smartphone");

    // Buscar un producto
    Node *resultado = buscar(tabla_hash, 101);
    if (resultado != NULL) {
        printf("Producto encontrado: %s (ID: %d)\n", resultado->nombre_producto, resultado->id_producto);
    } else {
        printf("Producto no encontrado.\n");
    }

    // Liberar memoria y limpiar la tabla (no mostrado)

    return 0;
}
