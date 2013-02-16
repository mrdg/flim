#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#ifndef __PORTTIME
#define __PORTTIME
#include "porttime.h"
#endif

#include "../js-1.8.5/js/src/jsapi.h"

enum task_types {
    C_FUNCTION   = 0,
    JS_FUNCTION = 1
};

struct task {
    PtTimestamp start;
    struct task *next;
    int task_type;

    jsval js_function;
    jsval *js_args;
    int js_argc;

    void (* c_function) (void *data);
    void *fn_data;
} task;

struct pqueue {
    pthread_mutex_t lock;
    struct task *head;
    int length;
};

struct pqueue * pqueue_create();
struct task * task_create(PtTimestamp start);
void enqueue(struct pqueue *queue, struct task *t);
struct task * dequeue(struct pqueue *queue, PtTimestamp now);
