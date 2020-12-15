#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <pthread.h>
#include <unistd.h> // sleep()
 
static int counter = 0;
 
typedef struct use_mutex_tag {
    pthread_mutex_t mutex;
} use_mutex_t;
 
void* minus(void *args) {
    int local;
    use_mutex_t *arg = (use_mutex_t*) args;
    pthread_mutex_lock(&(arg->mutex));
    local = counter;
    printf("min %d\n", counter);
    local = local - 1;
    counter = local;
    pthread_mutex_unlock(&(arg->mutex));
    sleep(1);
    return NULL;
}
 
void* plus(void *args) {
    int local;
    use_mutex_t *arg = (use_mutex_t*) args;
    pthread_mutex_lock(&(arg->mutex));
    local = counter;
    printf("pls %d\n", counter);
    local = local + 1;
    counter = local;
    pthread_mutex_unlock(&(arg->mutex));
    sleep(1);
    return NULL;
}
 
#define NUM_OF_THREADS 100
 
int main() {
    pthread_t threads[NUM_OF_THREADS];
    size_t i;
    use_mutex_t param;
 
    printf("counter = %d\n", counter);
    pthread_mutex_init(&(param.mutex), NULL);
    for (i = 0; i < NUM_OF_THREADS/2; i++) {
        pthread_create(&threads[i], NULL, minus, &param);
    }
    for (; i < NUM_OF_THREADS; i++) {
        pthread_create(&threads[i], NULL, plus, &param);
    }
    for (i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&(param.mutex));
    printf("counter = %d", counter);
    _getch();
    return 0;
}