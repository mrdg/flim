#include <stdio.h>
#include <stdlib.h>

#include "pqueue.h"

#ifndef __PORTTIME
#define __PORTTIME
#include "porttime.h"
#endif

#include "js/jsapi.h"

struct scheduler {
    struct pqueue *queue;
    JSContext *cx;
};

struct scheduler * create_scheduler(JSContext *cx);
void start_scheduler(struct scheduler *scheduler);
void schedule_task(struct scheduler *sched, struct task *t);
