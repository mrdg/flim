#include "../js-1.8.5/js/src/jsapi.h"

#include "server.h"
#include "midi_out.h"
#include "scheduler.h"
#include "logger.h"

struct flim {
    struct scheduler *scheduler;
    struct midi_out *output;
    JSRuntime *js_runtime;
    JSContext *js_context;
    JSObject *global;
};

struct flim * flm_new();
void eval(struct flim *flim, char *code);

