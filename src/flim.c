#include "flim.h"
#include <assert.h>

struct flim *flim;

void flm_note_on(void *data)
{
    struct note_data *note = (struct note_data *) data;
    flm_midi_note_on(flim->output, note->pitch, note->velocity);
}

void flm_note_off(void *data)
{
    struct note_data *note = (struct note_data *) data;
    flm_midi_note_off(flim->output, note->pitch, note->velocity);
}

static int flm_note(lua_State *L)
{
    struct note_data *note = malloc(sizeof(struct note_data));

    int time      = luaL_checknumber(L, 1);
    note->pitch    = luaL_checknumber(L, 2);
    note->velocity = luaL_checknumber(L, 3);
    int duration  = luaL_checknumber(L, 4);
    
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
    t->lua_function = lua_tostring(L, 2);
    t->task_type = LUA_FUNCTION;
    flm_scheduler_add_task(flim->scheduler, t);
    return 1;
}

void test_sound_fn(void *data)
{
    puts("Test sound called");
}

static int test_sound(lua_State *L)
{
    PtTimestamp time = lua_tonumber(L, 1);

    struct task *t = task_create(time);
    t->c_function = test_sound_fn;
    t->task_type = C_FUNCTION;
    t->fn_data = NULL;

    flm_scheduler_add_task(flim->scheduler, t);
    return 0;
}

lua_State * flm_init_lua()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, test_sound);
    lua_setglobal(L, "test_sound");

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
    return luaL_dostring(flim->lua, code);
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

