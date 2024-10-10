#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_LIBROS 5  // Número total de libros

sem_t semaforoLibros;

void* prestarLibro(void* arg) {
    int id = *((int*)arg);

    std::cout << "Thread " << id << ": Intentando prestar un libro...\n";

    // Intentar adquirir el semáforo 
    sem_wait(&semaforoLibros);

    std::cout << "Thread " << id << ": Prestó un libro. Libros restantes: " << (sem_getvalue(&semaforoLibros, &id), NUM_LIBROS - id) << "\n";

    sleep(5);

    sem_post(&semaforoLibros);

    std::cout << "Thread " << id << ": Devolvió un libro. Libros disponibles: " << (sem_getvalue(&semaforoLibros, &id), NUM_LIBROS - id) << "\n";

    return NULL;
}

int main() {
    pthread_t threads[10];
    int thread_ids[10];

    // Inicializar el semáforo con el valor del número de libros disponibles
    sem_init(&semaforoLibros, 0, NUM_LIBROS);

    // Crear 10 threads que intentan prestar y devolver libros
    for (int i = 0; i < 10; ++i) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, prestarLibro, &thread_ids[i]);
    }

    // Esperar a que todos los threads terminen
    for (int i = 0; i < 10; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&semaforoLibros);

    return 0;
}
