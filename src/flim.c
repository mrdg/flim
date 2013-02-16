#include "flim.h"
#include <assert.h>
#include <sys/stat.h>

struct flim *flim;

void reportError(JSContext *cx, const char *message, JSErrorReport *report) {
            flm_log(message);
}

static JSBool js_print(JSContext *cx, uintN argc, jsval *vp)
{
    jsval *argv;
    uintN i;
    JSString *str;
    char *bytes;

    argv = JS_ARGV(cx, vp);
    for (i = 0; i < argc; i++) {
        str = JS_ValueToString(cx, argv[i]);
        if (!str)
            return JS_FALSE;
        bytes = JS_EncodeString(cx, str);
        if (!bytes)
            return JS_FALSE;
        fprintf(stdout, "%s%s", i ? " " : "", bytes);
        JS_free(cx, bytes);
    }

    fputc('\n', stdout);
    fflush(stdout);

    JS_SET_RVAL(cx, vp, JSVAL_VOID);
    return JS_TRUE;
}

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

static JSBool note(JSContext *cx, uintN argc, jsval *vp)
{
    struct note_data *note = malloc(sizeof(struct note_data));
    int time, duration;
    if (argc < 5) {
        JS_ConvertArguments(cx, argc, JS_ARGV(cx, vp), "uuuu",
            &time, &note->pitch, &note->velocity, &duration);

        note->channel = 1;
    } else {
        JS_ConvertArguments(cx, argc, JS_ARGV(cx, vp), "uuuuu",
                &time, &note->pitch, &note->velocity, &duration, &note->channel);
    }

    struct task *note_on  = task_create(time);
    struct task *note_off = task_create(time + duration);

    note_on->c_function  = flm_note_on;
    note_on->fn_data     = note;
    note_on->task_type  = C_FUNCTION;

    note_off->c_function = flm_note_off;
    note_off->fn_data    = note;
    note_off->task_type  = C_FUNCTION;

    schedule_task(flim->scheduler, note_on);
    schedule_task(flim->scheduler, note_off);

    return JS_TRUE;
}


static JSBool now(JSContext *cx, uintN argc, jsval *vp)
{
    JS_SET_RVAL(cx, vp, INT_TO_JSVAL(Pt_Time()));
    return JS_TRUE;
}

static JSBool at(JSContext *cx, uintN argc, jsval *vp)
{
    if (argc < 2) {
        flm_log("'at' requires at least 2 arguments: <time> <function>");
        return JS_FALSE;
    }

    jsval *arguments = JS_ARGV(cx, vp);

    uint32_t time;
    JS_ValueToECMAUint32(cx, arguments[0], &time);
    struct task *t = task_create(time);

    t->js_argc = 0;

    if (JS_TypeOfValue(cx, arguments[1]) == JSTYPE_FUNCTION) {
        t->js_function = arguments[1];
    } else {
        flm_log("2nd argument of 'at' should be a function");
        return JS_FALSE;
    }

    t->js_argc = (argc > 2 ? argc - 2 : 1);
    jsval *fn_args = malloc(t->js_argc * sizeof(jsval));
    if (argc > 2) {
        int i;
        for (i = 2; i < argc; i++) {
            fn_args[i - 2] = arguments[i];
        }
    } else {
        fn_args[0] = time;
    }

    t->js_args = fn_args;
    t->task_type = JS_FUNCTION;
    schedule_task(flim->scheduler, t);
    return JS_TRUE;
}

static JSFunctionSpec js_functions[] = {
    JS_FS("print", js_print, 0, 0),
    JS_FS("note", note, 4, 0),
    JS_FS("now", now, 0, 0),
    JS_FS("at", at, 2, 0),
    JS_FS_END
};


static JSClass global_class = { "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, NULL,
    JSCLASS_NO_OPTIONAL_MEMBERS };

char * read_file(char * name) {
    FILE *file = fopen(name, "r");
    if (!file) {
        printf("Error opening file: %s\n", name);
        return NULL;
    }

    struct stat st;
    fstat(fileno(file), &st);
    char *buffer;
    buffer = malloc((st.st_size + 1) * sizeof(char));
    fread(buffer, sizeof(char), st.st_size, file);
    fclose(file);
    buffer[st.st_size] = '\0';
    return buffer;
}

void eval_file(char * name)
{
    char *code = read_file(name);
    eval(flim, code);
    free(code);
}

void initialize_js(struct flim *flim)
{
    flim->js_runtime = JS_NewRuntime(8L * 1024L * 1024L);
    flim->js_context = JS_NewContext(flim->js_runtime, 8192);
    JS_SetOptions(flim->js_context, JSOPTION_VAROBJFIX | JSOPTION_METHODJIT);
    JS_SetVersion(flim->js_context, JSVERSION_LATEST);
    JS_SetErrorReporter(flim->js_context, reportError);

    flim->global = JS_NewCompartmentAndGlobalObject(
                                    flim->js_context, &global_class, NULL);
    JS_InitStandardClasses(flim->js_context, flim->global);
    JS_DefineFunctions(flim->js_context, flim->global, js_functions);

    eval_file("runtime/flim.js");
    eval_file("runtime/underscore.js");
}

void eval(struct flim *flim, char *code)
{
    jsval rval;
    JS_EvaluateScript(flim->js_context, flim->global,
                       code, strlen(code), "script", 1, &rval);
}

struct flim * flm_new()
{
    flim = malloc(sizeof(struct flim));
    initialize_js(flim);
    flim->scheduler = create_scheduler(flim->js_context);
    start_scheduler(flim->scheduler);
    flim->output = create_midi_out();
    return flim;
}

