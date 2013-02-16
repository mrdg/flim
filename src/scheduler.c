#include <stdlib.h>
#include "scheduler.h"

/*
 * (60 (secs per min.) / 480 (bangs per min.)) / 10 = 0.0125
 * resolution = 0.01 secs = 10 ms
 */

#define RESOLUTION_MS 10

void exec_task(struct scheduler *scheduler, struct task *t)
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
    struct scheduler *scheduler =
        (struct scheduler * ) data;

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

struct scheduler * create_scheduler(JSContext *cx)
{
    struct scheduler *scheduler = malloc(sizeof(struct scheduler));
    scheduler->queue = pqueue_create();
    scheduler->cx = cx;
    return scheduler;
}

void start_scheduler(struct scheduler * scheduler)
{
    PtError err;
    err = Pt_Start(RESOLUTION_MS, *dispatch, scheduler);

    if (err != ptNoError) {
        printf("Error starting the timer\n");
        exit(1);
    }

}

void schedule_task(struct scheduler *sched, struct task *t)
{
    enqueue(sched->queue, t);
}

