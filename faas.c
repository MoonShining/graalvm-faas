#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <gen_graal_isolate_dynamic.h>

struct faas {
    char *libpath;
    void *lib;
    graal_create_isolate_fn_t graal_create_isolate;
    graal_tear_down_isolate_fn_t graal_tear_down_isolate;
    graal_isolatethread_t *isolate_thread;
};

typedef struct faas faas_t;

faas_t* faas_new(char* path){
    faas_t *f = (faas_t *)malloc(sizeof(faas_t));
    f->libpath = path;
    return f;
}

int faas_init(faas_t *f){
    f->lib = dlopen(f->libpath, RTLD_LAZY);
    if (f->lib == NULL) {
        return -1;
    }
    f->graal_create_isolate = dlsym(f->lib, "graal_create_isolate");
    if (f->graal_create_isolate == NULL) {
        return -2;
    }
    f->graal_tear_down_isolate = dlsym(f->lib, "graal_tear_down_isolate");
    if (f->graal_tear_down_isolate == NULL) {
        return -3;
    }
    int ret = f->graal_create_isolate(NULL, NULL, &(f->isolate_thread));
    if (ret != 0) {
        return -4;
    }
    return 0;
}

int faas_free(faas_t *f){
    free(f->libpath);
    f->graal_tear_down_isolate(f->isolate_thread);
    dlclose(f->lib);
    free(f);
    return 0;
}

int faas_add(faas_t *f, int a, int b) {
    int(*add)(graal_isolatethread_t*, int, int) = dlsym(f->lib, "add");
    if (add == NULL) {
        fprintf(stderr, "Could not find plugin_func: %s\n", dlerror());
        exit(1);
    }
    int result = add(f->isolate_thread, a, b);
    return result;
}

void faas_memory(faas_t *f, char *in, int inlen, char **out, int *outlen) {
    void(*memory)(graal_isolatethread_t*, char*, int, char **, int*) = dlsym(f->lib, "memory");
    if (memory == NULL) {
        fprintf(stderr, "Could not find plugin_func: %s\n", dlerror());
        exit(1);
    }
    memory(f->isolate_thread, in, inlen, out, outlen);
}
