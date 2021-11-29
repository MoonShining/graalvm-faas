#ifndef __LIBADD_H
#define __LIBADD_H

#include <graal_isolate_dynamic.h>


#if defined(__cplusplus)
extern "C" {
#endif

typedef int (*add_fn_t)(graal_isolatethread_t*, int, int);

typedef void (*memory_fn_t)(graal_isolatethread_t*, char*, int, char**, int*);

typedef void (*vmLocatorSymbol_fn_t)(graal_isolatethread_t* thread);

#if defined(__cplusplus)
}
#endif
#endif
