package org.evol;

import java.util.List;

public class GridRunner {

    public static void main(String[] args) {

        List<Integer> seeds = List.of(1, 2, 3, 4 ,5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30);
        List<Integer> popSizes = List.of(50);
        List<Double> crossProbs = List.of(0.25, 0.5, 0.75);
        List<Double> p0s = List.of(0.002, 0.02 , 0.05);
        List<RPGInstance> instances = List.of(
				RPGInstance.EXPONENTIAL,
                RPGInstance.PIECEWISE,
                RPGInstance.BALANCED
        );

        for (int seed : seeds)
        for (int pop : popSizes)
        for (double cross : crossProbs)
        for (double p0 : p0s)
        for (RPGInstance inst : instances) {

            RunConfig cfg = new RunConfig(
                    seed,
                    pop,
                    p0,
                    0.002,
                    0.95,
                    cross,
                    inst
            );

            System.out.println("RUNNING: " + cfg.tag());
            MainEA.run(cfg);
        }
    }
}
