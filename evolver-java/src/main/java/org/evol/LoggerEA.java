package org.evol;

import org.uma.jmetal.solution.integersolution.IntegerSolution;

import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Comparator;
import java.util.List;
import java.util.Locale;

public class LoggerEA {

    private final FileWriter writer;
    private final String basePath;

    private double bestEver = Double.POSITIVE_INFINITY;
    private int changeCount = 0;
    private String lastBestGenome = "";

    private static final DateTimeFormatter TS_FMT =
            DateTimeFormatter.ofPattern("yyyyMMdd_HHmm");

    private final String timestamp;

    public LoggerEA(
            String basePath,
            int seed,
            RPGInstance instance,
            int popSize,
            int generations,
            double crossoverProb,
            double p0,
            double pMin,
            double alpha,
            double lambdaSimilarity,
            int fightsPerTeam,
            int anchorCount,
            int tournamentK,
            int elitismCount
    ) {
        try {
            this.basePath = basePath;
            this.timestamp = LocalDateTime.now().format(TS_FMT);

            String path = basePath.replace(".csv", "_" + timestamp + ".csv");
            writer = new FileWriter(path);

            writer.write("# seed=" + seed + "\n");
            writer.write("# instance=" + instance.name() + "\n");
            writer.write("# popSize=" + popSize + "\n");
            writer.write("# generations=" + generations + "\n");
            writer.write("# crossover=BlockUniform(p=" + crossoverProb + ")\n");
            writer.write("# mutation=p0=" + p0 + ",pMin=" + pMin + ",alpha=" + alpha + "\n");
            writer.write("# tournamentK=" + tournamentK + "\n");
            writer.write("# elitism=" + elitismCount + "\n");
            writer.write("# lambda_similarity=" + lambdaSimilarity + "\n");
            writer.write("# fights_per_team=" + fightsPerTeam + "\n");
            writer.write("# anchors=" + anchorCount + "\n");

            writer.write(
                    "gen,changes,best_fitness,best_winrate,best_similarity," +
                    "avg_fitness,worst_fitness,best_genome\n"
            );
            writer.flush();

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void log(int gen, List<IntegerSolution> pop) {
        double best = Double.POSITIVE_INFINITY;
        double worst = Double.NEGATIVE_INFINITY;
        double sum = 0.0;

        IntegerSolution bestSol = null;

        for (var s : pop) {
            double f = s.objectives()[0];
            sum += f;
            if (f < best) {
                best = f;
                bestSol = s;
            }
            worst = Math.max(worst, f);
        }

        double avg = sum / pop.size();

        double wr = (double) bestSol.attributes().getOrDefault("winrate", Double.NaN);
        double sim = (double) bestSol.attributes().getOrDefault("similarity", Double.NaN);

        String genomeStr = serializeGenome(bestSol);

        if (best < bestEver) {
            bestEver = best;
            changeCount++;
            lastBestGenome = genomeStr;
        }

        try {
            writer.write(
                    gen + "," +
                    changeCount + "," +
                    fmt(best) + "," +
                    fmt(wr) + "," +
                    fmt(sim) + "," +
                    fmt(avg) + "," +
                    fmt(worst) + "," +
                    "\"" + lastBestGenome + "\"\n"
            );
            writer.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    // ===============================
    // TOP-K population
    // ===============================
    public void writeTopIndividuals(List<IntegerSolution> population, int topK) {
        String out = basePath.replace(".csv", "_TOP" + topK + "_" + timestamp + ".csv");
        population.sort(Comparator.comparingDouble(s -> s.objectives()[0]));

        try (FileWriter fw = new FileWriter(out)) {
            fw.write("rank,fitness,winrate,genome\n");
            for (int i = 0; i < Math.min(topK, population.size()); i++) {
                var s = population.get(i);
                fw.write(
                        (i + 1) + "," +
                        fmt(s.objectives()[0]) + "," +
                        fmt((double) s.attributes().getOrDefault("winrate", Double.NaN)) + "," +
                        "\"" + serializeGenome(s) + "\"\n"
                );
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    // ===============================
    // Hall of Fame
    // ===============================
    public void writeHallOfFame(HallOfFame hof) {
        String out = basePath.replace(".csv", "_HOF_" + timestamp + ".csv");

        try (FileWriter fw = new FileWriter(out)) {
            fw.write("rank,fitness,winrate,genome\n");

            hof.getEntries().stream()
                    .sorted((a, b) -> Double.compare(b.fitness, a.fitness))
                    .forEachOrdered(e -> {
                        try {
                            double wr = (double) e.sol.attributes()
                                    .getOrDefault("winrate", Double.NaN);

                            fw.write(
                                    fmt(e.fitness) + "," +
                                    fmt(wr) + "," +
                                    "\"" + serializeGenome(e.sol) + "\"\n"
                            );
                        } catch (IOException ex) {
                            throw new RuntimeException(ex);
                        }
                    });

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void writeRuntime(double seconds) {
        try {
            writer.write("# Elitism = 4, runtime_seconds=" +
                    String.format(Locale.US, "%.3f", seconds) + "\n");
            writer.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private static String fmt(double x) {
        return Double.isFinite(x)
                ? String.format(Locale.US, "%.4f", x)
                : "";
    }

    private String serializeGenome(IntegerSolution sol) {
        var v = sol.variables();
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < v.size(); i++) {
            sb.append(v.get(i));
            if (i + 1 < v.size()) sb.append(' ');
        }
        return sb.toString();
    }

    public void close() {
        try { writer.close(); } catch (IOException ignored) {}
    }
}
