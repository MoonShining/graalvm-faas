package main

import (
	"fmt"
	"log"
)

import "github.com/MoonShining/graalvmfaas"

func main() {
	// 加载动态库
	fa, err := graalvmfaas.NewFaas("java/libgraalvmfaas.dylib")
	if err != nil {
		log.Fatal(err)
	}
	defer fa.Destory()

	// 传入一段内存, 并获取返回的内存
	out, free, err := fa.Fn([]byte("abcde"))
	if err != nil {
		log.Fatal(err)
	}
	defer free()
	fmt.Println("java return str:", string(out))
}
