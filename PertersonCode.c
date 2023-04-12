#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int idx, length;
  int id;
} thread_arg, *ptr_thread_arg;

pthread_t threads[2];

int turn = 0;           // indica de quem éa vez
int wants[2] = {0, 0};  // a tarefa i quer acessar?

void enter(int task) {
  int other = 1 - task;  // indica a outra tarefa
  wants[task] = 1;       // task quer acessar a seção crítica
  turn = other;
  while ((turn == other) && wants[other])
    ;  // espera ocupada
}

void leave(int task) {
  wants[task] = 0;  // task libera a seção crítica
}

void *thread_func(void *arg) {
  int i;
  ptr_thread_arg targ = (ptr_thread_arg)arg;
  int task = targ->id;

  for (i = targ->idx; i < (targ->idx + targ->length); i++) {
    enter(task);
    printf("Thread %d ? value %d\n", pthread_self(), i);
    leave(task);
  }
}

int main(int argc, char **argv) {
  thread_arg arguments[2];
  int i;
  for (i = 0; i < 2; i++) {
    arguments[i].idx = i * 10;
    arguments[i].length = 10;
    arguments[i].id = i;
    pthread_create(&(threads[i]), NULL, thread_func, &(arguments[i]));
  }
  for (i = 0; i < 2; i++) {
    pthread_join(threads[i], NULL);
  }
}