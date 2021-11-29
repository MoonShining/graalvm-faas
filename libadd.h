#ifndef __LIBADD_H
#define __LIBADD_H

#include <graal_isolate.h>


#if defined(__cplusplus)
extern "C" {
#endif

int add(graal_isolatethread_t*, int, int);

void memory(graal_isolatethread_t*, char*, int, char**, int*);

void vmLocatorSymbol(graal_isolatethread_t* thread);

#if defined(__cplusplus)
}
#endif
#endif
