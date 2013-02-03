#include "flim.h"
#include <assert.h>

struct flim *flim;

void flm_note_on(void *data)
{
    struct note_data *note = (struct note_data *) data;
    flm_midi_note_on(flim->output, note->pitch, note->velocity, note->channel);
}

void flm_note_off(void *data)
{
    struct note_data *note = (struct note_data *) data;
    flm_midi_note_off(flim->output, note->pitch, note->velocity, note->channel);
}

static int flm_note(lua_State *L)
{
    struct note_data *note = malloc(sizeof(struct note_data));

    int time      = luaL_checknumber(L, 1);
    note->pitch    = luaL_checknumber(L, 2);
    note->velocity = luaL_checknumber(L, 3);
    int duration  = luaL_checknumber(L, 4);
    if lua_isnoneornil(L, 5) {
        note->channel = 1;
    } else {
        note->channel = luaL_checknumber(L, 5);
    }

    struct task *note_on  = task_create(time);
    struct task *note_off = task_create(time + duration);

    note_on->c_function  = flm_note_on;
    note_on->fn_data     = note;
    note_on->task_type  = C_FUNCTION;

    note_off->c_function = flm_note_off;
    note_off->fn_data    = note;
    note_off->task_type  = C_FUNCTION;

    flm_scheduler_add_task(flim->scheduler, note_on);
    flm_scheduler_add_task(flim->scheduler, note_off);

    return 0;
}

static int now(lua_State *L)
{
    lua_pushnumber(L, Pt_Time());
    return 1;
}

static int flm_callback(lua_State *L)
{
    PtTimestamp time = lua_tonumber(L, 1);
    struct task *t = task_create(time);

    int nargs = lua_gettop(L);
    if (nargs > 2) {
        t->args_key = luaL_ref(L, LUA_REGISTRYINDEX);
    } else {
        t->args_key = LUA_NOREF;
    }

    t->function_key = luaL_ref(L, LUA_REGISTRYINDEX);
    t->task_type = LUA_FUNCTION;

    flm_scheduler_add_task(flim->scheduler, t);
    return 1;
}

lua_State * flm_init_lua()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_loadfile(L, "./runtime/flim.lua") == 0) {
        lua_pcall(L, 0, 0, 0);
    }

    lua_pushcfunction(L, flm_callback);
    lua_setglobal(L, "callback");

    lua_pushcfunction(L, flm_note);
    lua_setglobal(L, "note");

    lua_pushcfunction(L, now);
    lua_setglobal(L, "now");

    return L;
}

int flm_eval(struct flim *flim, char *code)
{
    int ret = luaL_dostring(flim->lua, code);
    if (ret == 1) {
        flm_log(lua_tostring(flim->lua, -1));
    }

    return ret;
}


struct flim * flm_new()
{
    flim = malloc(sizeof(struct flim));

    flim->lua = flm_init_lua();

    flim->scheduler = flm_scheduler_create(flim->lua);
    flm_scheduler_start(flim->scheduler);

    flim->output = create_midi_out();

    return flim;
}

