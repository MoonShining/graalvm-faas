### Intro

本项目提供通过cgo调用graalvm native 镜像编译成的shared lib， 来执行java代码的能力

#### quickstart

`go run demo/main.go`

#### 项目结构

- faas.c 核心逻辑，加载动态库，提取符号，操作graalvm， 执行native调用
- faas.go go api封装

#### 内存交互

`Fn`函数提供了最基本的二进制交互能力，也就是go可以传一块内存给java， java可以返回一块内存给go

核心函数是`int faas_fn(faas_t *f, char *in, int inlen, char **out, int *outlen)`

*Go Call Java:*
- go传递给c一个go slice的data ptr
- c认为是一个char*指针，这个char*指针会传递给java的CCharPointer
- java可以通过CCharPointer读取数据(正常情况下是只读的,如果需要修改，必须确保操作不越界)

*Java Return To Go:*
- java使用UnmanagedMemory.malloc 返回一个指针outptr，并向outptr指向的内存中填充好返回值
- java使用CCharPointerPointer向faas_fn中的**out指针写入值，*out=outptr
- cgo返回，go通过将指针转换为slice，来方便的操作返回的内存（注意使用完后free）。