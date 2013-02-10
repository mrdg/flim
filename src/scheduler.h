#include <stdio.h>
#include <stdlib.h>

#ifndef __PORTTIME
#define __PORTTIME
#include "porttime.h"
#endif

#include "../js-1.8.5/js/src/jsapi.h"

struct flm_scheduler {
    struct pqueue *queue;
    JSContext *cx;
};

int scheduler_start();

typedef void (*task)(PtTimestamp tasktime);
// int flm_scheduler_add_task(struct flm_scheduler *sched, struct task *t);

struct flm_scheduler * flm_scheduler_create(JSContext *cx);
void flm_scheduler_start(struct flm_scheduler * scheduler);
