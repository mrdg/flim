#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "server.h"
#include "midi_out.h"
#include "scheduler.h"

#include "pqueue.h"

struct flim {
    struct flm_scheduler *scheduler;
    struct midi_out *output;
    lua_State *lua;
};

struct flim * flm_new();
int flm_eval(struct flim *flim, char *code);

