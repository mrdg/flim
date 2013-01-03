#include <stdio.h>
#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#ifndef __PORTTIME
#define __PORTTIME
#include "porttime.h"
#endif

struct flm_scheduler {
    struct pqueue *queue;
    lua_State *lua;
};

int scheduler_start();

typedef void (*task)(PtTimestamp tasktime);
// int flm_scheduler_add_task(struct flm_scheduler *sched, struct task *t);

struct flm_scheduler * flm_scheduler_create(lua_State *L);
void flm_scheduler_start(struct flm_scheduler * scheduler);
