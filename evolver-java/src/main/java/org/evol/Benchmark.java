package org.evol;

public class Benchmark {
    public static void main(String[] args) {
        System.out.println("100_000_000_000L");
        final long N = 1_000_000_000_000L;

        // Java loop
        long startJava = System.nanoTime();
        long i = 0;
        while (i < N) {
            i++;
            if (i % 100_000_000_000L == 0) {
                System.out.println(i);
            }
        }
        long endJava = System.nanoTime();

        System.out.println("Java loop:  " + (endJava - startJava)/1e6 + " ms");

        // C++ native loop
        long startNative = System.nanoTime();
        long result = Poc.nativeLoop(N);
        long endNative = System.nanoTime();

        System.out.println("Native loop: " + (endNative - startNative)/1e6 + " ms");
        System.out.println("Native returned: " + result);
    }
}
