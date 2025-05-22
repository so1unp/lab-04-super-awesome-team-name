#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#define SEM_NAME_LEN 255

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s [OPCION] [PARAMETRO]\n", argv[0]);
    fprintf(stderr, "Opciones:\n");
    fprintf(stderr, "\t-c semaforo valor\tcrea semáforo con el nombre y valor inicial indicado.\n");
    fprintf(stderr, "\t-u semaforo\t\trealiza un UP en el semáforo indicado.\n");
    fprintf(stderr, "\t-d semaforo\t\trealiza un DOWN en el semaforo indicado.\n");
    fprintf(stderr, "\t-b semaforo\t\telimina el semáforo indicado.\n");
    fprintf(stderr, "\t-i semaforo\t\tinformación del semáforo indicado.\n");
    fprintf(stderr, "\t-h\t\t\timprime este mensaje.\n");
}

// esta funcion recibe el nombre de un semafoto e intenta abrirlo,
// en caso de que el semaforo termina el programa con un fallo
sem_t* abrir_semaforo(const char *nombre) {
    sem_t *s = sem_open(nombre, 0);
    if (s == SEM_FAILED) {
        perror("No se pudo abrir el semáforo");
        exit(EXIT_FAILURE);
    }
    return s;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    char sem_name[SEM_NAME_LEN];
    snprintf(sem_name, SEM_NAME_LEN, "%s", argv[2]);

    sem_t *semasphor;

    switch(option) {
        case 'c':
            // snprintf(sem_name, sizeof(SEM_NAME_LEN), "/%s", argv[2]);
            semasphor = sem_open(sem_name, O_CREAT, 0644, atoi(argv[3]));
            if (semasphor != SEM_FAILED) {
                printf("semaforo %s creado exitosamente \n", argv[2]);
                sem_close(semasphor);
            }
            else {
                printf("no se pudo crear el semaforo \n");
            }
            break;
        case 'u':
            semasphor = abrir_semaforo(sem_name);

            int semUpValue = sem_post(semasphor);
            if (semUpValue == 0) {
                printf("el valor del semaforo se incremento en 1 unidad \n");
            }
            else {
                printf("no se pudo incrementar el valor del semaforo %s \n", sem_name);
            }

            sem_close(semasphor);
            break;
        case 'd':
            semasphor = abrir_semaforo(sem_name);

            int semDownValue = sem_wait(semasphor);
            if (semDownValue == 0) {
                printf("el valor del semaforo se decremento en 1 unidad \n");
            }
            else {
                printf("no se pudo decremetar el valor del semaforo %s \n", sem_name);
            }
            sem_close(semasphor);

            break;
        case 'b':
            int deleteSem = sem_unlink(sem_name);
            if (deleteSem == 0) {
                printf("Semaforo %s eliminado correctamente \n", sem_name);
            }
            else {
                printf("el semaforo %s no pudo ser elminado \n", sem_name);
            }
            break;
        case 'i':
            semasphor = abrir_semaforo(sem_name);

            int semValue = -1;
            sem_getvalue(semasphor, &semValue);
            if (semValue == 0) {
                printf ("Semaforo: %s -- Valor: %d \n", sem_name, semValue);
            }
            else {
                printf("no se pudo obtener el valor del semaforo %s \n", sem_name);
            }

            sem_close(semasphor);
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Opción desconocida: %s \n", argv[1]);
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
