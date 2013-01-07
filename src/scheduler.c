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
    if (t->task_type == LUA_FUNCTION) {
        lua_rawgeti(scheduler->lua, LUA_REGISTRYINDEX, t->function_key);
        int args = 0;

        if (t->args_key == LUA_NOREF) {
            lua_pushnumber(scheduler->lua, t->start);
            args = 1;
        } else {
            lua_rawgeti(scheduler->lua, LUA_REGISTRYINDEX, t->args_key);

            lua_pushnil(scheduler->lua);
            while (lua_next(scheduler->lua, -2) != 0) {
                lua_insert(scheduler->lua, -3);
                args++;
            }
            lua_pop(scheduler->lua, 1);
        }
        lua_pcall(scheduler->lua, args, 0, 0);

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

struct flm_scheduler * flm_scheduler_create(lua_State *L)
{
    struct flm_scheduler *scheduler = malloc(sizeof(struct flm_scheduler));
    scheduler->lua = L;
    scheduler->queue = pqueue_create();
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

