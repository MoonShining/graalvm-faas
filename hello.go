package main

/*
#include <faas.c>
*/
import "C"
import "log"
import "unsafe"
import "reflect"

func main(){
    faasPtr := C.faas_new(C.CString("libadd.dylib"))
    if code := C.faas_init(faasPtr); code != 0 {
        log.Fatalf("init return %d", code)
    }
    defer C.faas_free(faasPtr)


    res := C.faas_add(faasPtr, 1, 2)
    log.Println(res)
    log.Println("========")


    var in = []byte{1,2,3}
    var inlen = C.int(len(in))
    var outptr *C.char
    defer C.free(unsafe.Pointer(outptr))
    var outlen C.int
    C.faas_memory(
        faasPtr, 
        (*C.char)(unsafe.Pointer(&in[0])),
        inlen,
        &outptr, 
        &outlen,
    )
    sh := &reflect.SliceHeader{
        Data: uintptr(unsafe.Pointer(outptr)),
        Len: int(outlen),
        Cap: int(outlen),
    }
    outslice := *(*[]byte)(unsafe.Pointer(sh))
    log.Println(string(outslice))
}