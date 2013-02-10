#include <stdlib.h>

#include "scheduler.h"
#include "pqueue.h"

/*
 * (60 (secs per min.) / 480 (bangs per min.)) / 10 = 0.0125
 * resolution = 0.01 secs = 10 ms
 */

#define RESOLUTION_MS 10

void exec_task(struct flm_scheduler *scheduler, struct task *t)
{
    if (t->task_type == JS_FUNCTION) {
        jsval rval;
        JS_CallFunctionValue(scheduler->cx,
                NULL, t->js_function, t->js_argc, t->js_args, &rval);

        free(t->js_args);
    } else if (t->task_type == C_FUNCTION) {
        t->c_function(t->fn_data);
    }
}

void dispatch(PtTimestamp time, void *data)
{
    struct flm_scheduler *scheduler =
        (struct flm_scheduler * ) data;

    struct task *first = dequeue(scheduler->queue, time);

    if (first == NULL) {
        return;
    } else {

        struct task *current = first;
        struct task *prev    = current;

        assert(current == first);
        assert(prev == current);

        while (current != NULL) {
            exec_task(scheduler, current);
            prev = current;
            current = current->next;
            free(prev);
        }
    }

}

struct flm_scheduler * flm_scheduler_create(JSContext *cx)
{
    struct flm_scheduler *scheduler = malloc(sizeof(struct flm_scheduler));
    scheduler->queue = pqueue_create();
    scheduler->cx = cx;
    return scheduler;
}

void flm_scheduler_start(struct flm_scheduler * scheduler)
{
    PtError err;
    err = Pt_Start(RESOLUTION_MS, *dispatch, scheduler);

    if (err != ptNoError) {
        printf("Error starting the timer\n");
        exit(1);
    }

}

int flm_scheduler_add_task(struct flm_scheduler *sched, struct task *t)
{
    enqueue(sched->queue, t);
    return  1;
}

