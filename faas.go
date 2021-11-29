package graalvmfaas

/*
#include "faas.h"
*/
import "C"
import "unsafe"
import "reflect"
import "fmt"

type Faas struct {
	f *C.faas_t
}

func NewFaas(libpath string) (*Faas, error) {
	fptr := C.faas_create(C.CString(libpath))
	if errcode := C.faas_init(fptr); errcode != 0 {
		C.faas_destroy(fptr)
		return nil, fmt.Errorf("faas init error code %d", int(errcode))
	}
	return &Faas{f: fptr}, nil
}

func (f *Faas) Destory() error {
	ret := C.faas_destroy(f.f)
	if int(ret) != 0 {
		return fmt.Errorf("faas destroy error code %d", ret)
	}
	return nil
}

func (f *Faas) Fn(in []byte) ([]byte, func(), error) {
	var inlen = C.int(len(in))
	var outptr *C.char
	var outlen C.int

	var code C.int = C.faas_fn(
		f.f,
		(*C.char)(unsafe.Pointer(&in[0])),
		inlen,
		&outptr,
		&outlen,
	)
	if int(code) != 0 {
		return nil, nil, fmt.Errorf("faas fn fail %d", int(code))
	}

	sh := &reflect.SliceHeader{
		Data: uintptr(unsafe.Pointer(outptr)),
		Len:  int(outlen),
		Cap:  int(outlen),
	}
	out := *(*[]byte)(unsafe.Pointer(sh))
	return out, func() { C.graalvm_unmanaged_memory_free(f.f, outptr) }, nil
}
