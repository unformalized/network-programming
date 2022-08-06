#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

void *read(void *arg);
void *accu(void *arg);

static sem_t sem_one;
static sem_t sem_two;
static int num;

int main(int argc, char *argv[]) {
    pthread_t id_t1, id_t2;
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);

    pthread_create(&id_t1, NULL, read, NULL);
    pthread_create(&id_t2, NULL, accu, NULL);

    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);

    return 0;
}

void *read(void *arg) {
    int i;
    for (i = 0; i < 5; i++) {
        fputs("Input num: ", stdout);
        sem_wait(&sem_two); // sem_two -1（0），并且在 accu 执行之前
                            // sem_post(&sem_two) 若再次执行会进行阻塞
        scanf("%d", &num);
        sem_post(&sem_one); // sem_one +1，让下面的程序顺利进行
    }

    return NULL;
}

void *accu(void *arg) {
    int sum = 0, i;
    for (i = 0; i < 5; i++) {
        // 由于初始值为 0，必定会阻塞, 必须等到 read 执行完
        // sem_post(&sem_one); 若再次执行会阻塞
        sem_wait(&sem_one);
        sum += num;
        sem_post(&sem_two);
    }
    printf("Result: %d\n", sum);
    return NULL;
}
