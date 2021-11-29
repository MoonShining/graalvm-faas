package com.graalvmfaas;

import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.function.CEntryPoint;
import org.graalvm.nativeimage.c.type.CCharPointer;
import org.graalvm.nativeimage.c.type.CIntPointer;
import org.graalvm.nativeimage.c.type.CCharPointerPointer;
import org.graalvm.nativeimage.UnmanagedMemory;
import org.graalvm.nativeimage.c.type.CTypeConversion;

public class GraalvmFaasBase
{
    @CEntryPoint(name = "faas_entrypoint")
    public static int faasEntrypoint(
            IsolateThread isolateThread,
            CCharPointer in, int inlen, 
            CCharPointerPointer out, CIntPointer outlen
    ) {
        byte[] inbytes = new byte[inlen];
        for(int i = 0; i< inlen; i++){
            inbytes[i] = in.read(i);
        }

        byte[] outbytes = call(inbytes);

        CCharPointer outptr = (CCharPointer)UnmanagedMemory.malloc(outbytes.length);
        for(int i = 0; i< outbytes.length; i++){
            outptr.write(i, outbytes[i]);
        }
        out.write(outptr);
        outlen.write(outbytes.length);
        return 0;
    }

    @CEntryPoint(name = "unmanaged_memory_free")
    public static void unmanagedFree(
            IsolateThread isolateThread,
            CCharPointer in
    ) {
        UnmanagedMemory.free(in);
    }

    private static byte[] call(byte[] in) {
        return new StringBuilder(new String(in)).reverse().toString().getBytes();
    }
}