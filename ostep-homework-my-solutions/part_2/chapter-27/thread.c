

#include <pthread.h>

int 
pthread_create(
    pthread_t *               thread,
    const pthread_attr_t *    attr,
    void *                    (*start_routine) (void*),
    void *                    arg
);

typedef struct __myarg_t {
    int a;
    int b;
} myarg_t;

void *mythread(void *arg) {
    myarg_t *m = (myarg_t *) arg;
    printf("%d %d\n", m->a, m->b);
    return NULL;
}

int
main(int argc, char *argv[]) {
    pthread_t p;
    int rc;

    myarg_t args;
    args.a = 10;
    args.b = 20;
    rc = pthread_create(&p, NULL, mythread, &args);
    // ...
}
