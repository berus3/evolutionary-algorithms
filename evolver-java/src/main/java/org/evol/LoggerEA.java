package org.evol;

import org.uma.jmetal.solution.integersolution.IntegerSolution;

import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Locale;

public class LoggerEA {

    private final FileWriter writer;

    private double bestEver = Double.POSITIVE_INFINITY;
    private int changeCount = 0;
    private String lastBestGenome = "";

    private static final DateTimeFormatter TS_FMT =
            DateTimeFormatter.ofPattern("yyyyMMdd_HHmm");

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
            int anchorCount
    ) {
        try {
            String ts = LocalDateTime.now().format(TS_FMT);
            String path = basePath.replace(".csv", "_" + ts + ".csv");

            writer = new FileWriter(path);

            // ===============================
            // Metadata header
            // ===============================
            writer.write("# seed=" + seed + "\n");
            writer.write("# instance=" + instance.name() + "\n");
            writer.write("# popSize=" + popSize + "\n");
            writer.write("# generations=" + generations + "\n");
            writer.write("# crossover=BlockUniformCrossover(p=" + crossoverProb + ")\n");
            writer.write("# mutation=FlipMutation(p0=" + p0 +
                         ",pMin=" + pMin +
                         ",alpha=" + alpha + ")\n");
            writer.write("# fitness=winrate - lambda*similarity\n");
            writer.write("# lambda_similarity=" + lambdaSimilarity + "\n");
            writer.write("# similarity_metric=cosine\n");
            writer.write("# similarity_space=buckets\n");
            writer.write("# similarity_window=±1\n");
            writer.write("# fights_per_team=" + fightsPerTeam + "\n");
            writer.write("# anchors=" + anchorCount + "\n");

            // ===============================
            // CSV header
            // ===============================
            writer.write(
                    "gen,changes," +
                    "best_fitness,best_winrate,best_similarity," +
                    "avg_fitness,worst_fitness," +
                    "best_genome\n"
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

        double bestWinrate = Double.NaN;
        double bestSimilarity = Double.NaN;

        if (bestSol != null) {
            Object wr = bestSol.attributes().get("winrate");
            Object sim = bestSol.attributes().get("similarity");

            if (wr instanceof Double) bestWinrate = (Double) wr;
            if (sim instanceof Double) bestSimilarity = (Double) sim;
        }

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
                    fmt(bestWinrate) + "," +
                    fmt(bestSimilarity) + "," +
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
    // Runtime footer
    // ===============================
    public void writeRuntime(double seconds) {
        try {
            writer.write("# runtime_seconds=" +
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
        if (sol == null) return "";

        List<Integer> v = sol.variables();
        int players = 5;
        int genesPerPlayer = v.size() / players;

        StringBuilder sb = new StringBuilder();

        for (int p = 0; p < players; p++) {
            sb.append("[P").append(p).append("] ");
            int start = p * genesPerPlayer;
            int end = start + genesPerPlayer;

            for (int i = start; i < end; i++) {
                sb.append(v.get(i));
                if (i < end - 1) sb.append(' ');
            }
            if (p < players - 1) sb.append("; ");
        }
        return sb.toString();
    }

    public void close() {
        try {
            writer.close();
        } catch (IOException ignored) {}
    }
}
