#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#ifndef __PORTTIME
#define __PORTTIME
#include "porttime.h"
#endif

#define QUEUE_LENGTH 250

enum task_types {
    C_FUNCTION   = 0,
    LUA_FUNCTION = 1
};

struct task {
    PtTimestamp start;
    struct task *next;
    int task_type;
    int function_key;
    const char *lua_function;
    void (* c_function) (void *data);
    void *fn_data;
};

struct pqueue {
    pthread_mutex_t lock;
    struct task *head;
    int length;
};

struct pqueue * pqueue_create();
struct task * task_create(PtTimestamp start);
void enqueue(struct pqueue *queue, struct task *t);
struct task * dequeue(struct pqueue *queue, PtTimestamp now);
