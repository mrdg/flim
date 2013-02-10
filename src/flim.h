#include "../js-1.8.5/js/src/jsapi.h"

#include "server.h"
#include "midi_out.h"
#include "scheduler.h"
#include "logger.h"
#include "pqueue.h"

struct flim {
    struct flm_scheduler *scheduler;
    struct midi_out *output;
    JSRuntime *js_runtime;
    JSContext *js_context;
    JSObject *global;
};

struct flim * flm_new();
int eval(struct flim *flim, char *code);

