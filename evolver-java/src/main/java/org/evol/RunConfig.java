package org.evol;

public class RunConfig {

    public final int seed;
    public final int popSize;
    public final double p0;
    public final double pMin;
    public final double alpha;
    public final double crossoverProb;
    public final RPGInstance instance;

    public RunConfig(
            int seed,
            int popSize,
            double p0,
            double pMin,
            double alpha,
            double crossoverProb,
            RPGInstance instance
    ) {
        this.seed = seed;
        this.popSize = popSize;
        this.p0 = p0;
        this.pMin = pMin;
        this.alpha = alpha;
        this.crossoverProb = crossoverProb;
        this.instance = instance;
    }

    /** Unique identifier for logging */
    public String tag() {
        return String.format(
                "seed=%d_pop=%d_p0=%.3f_cross=%.2f_inst=%s",
                seed, popSize, p0, crossoverProb, instance
        );
    }
}
