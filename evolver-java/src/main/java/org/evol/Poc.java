package org.evol;

public class Poc {

    static {
        try {
            System.loadLibrary("combat"); // libcombat.so
        } catch (UnsatisfiedLinkError e) {
            System.err.println("ERROR loading native library: " + e.getMessage());
            throw e;
        }
    }

    // JNI method
    public static native long nativeLoop(long n);
}
