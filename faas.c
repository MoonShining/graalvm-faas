#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include "gen_graal_isolate_dynamic.h"
#include "faas.h"

faas_t* faas_create(char* path){
    faas_t *f = (faas_t *)malloc(sizeof(faas_t));
    f->libpath = path;
    f->isolate_thread = NULL;
    f->graal_create_isolate = NULL;
    f->graal_tear_down_isolate = NULL;
    f->fn = NULL;
    f->unmanaged_memory_free = NULL;
    f->lib = NULL;
    return f;
}

int faas_destroy(faas_t *f){
    if(f->libpath != NULL){
        free(f->libpath);
        f->libpath = NULL;
    }
    if(f->isolate_thread != NULL){
    	f->graal_tear_down_isolate(f->isolate_thread);
	    f->isolate_thread = NULL;
    }
    if(f->graal_create_isolate != NULL){
	    f->graal_create_isolate = NULL;
    }
    if(f->graal_tear_down_isolate != NULL){
	    f->graal_tear_down_isolate = NULL;
    }
    if(f->fn != NULL){
	    f->fn = NULL;
    }
    if(f->unmanaged_memory_free != NULL){
	    f->unmanaged_memory_free = NULL;
    }
    if(f->lib != NULL){
    	dlclose(f->lib);
        f->lib = NULL;
    }
    free(f);
    return 0;
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
    f->fn = dlsym(f->lib, "faas_entrypoint"); // impl in java native
    if (f->fn == NULL) {
	    return -5;
    }
    f->unmanaged_memory_free = dlsym(f->lib, "unmanaged_memory_free"); // must impl in java
    if (f->unmanaged_memory_free == NULL) {
	    return -6;
    }
    return 0;
}

// call native func
int faas_fn(faas_t *f, char *in, int inlen, char **out, int *outlen) {
    return f->fn(f->isolate_thread, in, inlen, out, outlen);
}

void graalvm_unmanaged_memory_free(faas_t *f, char *in) {
    f->unmanaged_memory_free(f->isolate_thread, in);
}
