package com.lib;

import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.function.CEntryPoint;
import org.graalvm.nativeimage.c.type.CCharPointer;
import org.graalvm.nativeimage.c.type.CIntPointer;
import org.graalvm.nativeimage.c.type.CCharPointerPointer;
import org.graalvm.nativeimage.UnmanagedMemory;

public class Add
{
    @CEntryPoint(name = "add")
    public static int add(
            IsolateThread isolateThread,
            int a,
            int b
    ) {
        return a + b;
    }

    @CEntryPoint(name = "memory")
    public static void memory(
            IsolateThread isolateThread,
            CCharPointer in, int inlen, 
            CCharPointerPointer out, CIntPointer outlen
    ) {
        // modify input
        in.write(0, (byte)0);

        // malloc c memory and return
        // caller must free it
        outlen.write(5);
        CCharPointer outptr = (CCharPointer)UnmanagedMemory.malloc(5);
        outptr.write(0, (byte)'h');
        outptr.write(1, (byte)'e');
        outptr.write(2, (byte)'l');
        outptr.write(3, (byte)'l');
        outptr.write(4, (byte)'o');
        out.write(outptr);
    }
}