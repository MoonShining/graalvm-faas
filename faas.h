#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "gen_graal_isolate_dynamic.h"

typedef int(*abi_fn_t)(graal_isolatethread_t*, char*, int, char **, int*);
typedef void(*graalvm_unmanaged_memory_free_fn_t)(graal_isolatethread_t*, char*);

struct faas {
    char *libpath;
    void *lib;
    graal_create_isolate_fn_t graal_create_isolate;
    graal_tear_down_isolate_fn_t graal_tear_down_isolate;
    graal_isolatethread_t *isolate_thread;
    abi_fn_t fn;
    graalvm_unmanaged_memory_free_fn_t unmanaged_memory_free;
};

typedef struct faas faas_t;

// create faas handler
faas_t* faas_create(char* path);

// destory faas handler
int faas_destroy(faas_t *f);

// initialize faas handler
int faas_init(faas_t *f);

// the faas function implement in java
int faas_fn(faas_t *f, char *in, int inlen, char **out, int *outlen);

// free java allocated memory
void graalvm_unmanaged_memory_free(faas_t *f, char *in);
