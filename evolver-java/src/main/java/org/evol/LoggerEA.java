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

    public LoggerEA(String basePath) {
        try {
            String ts = LocalDateTime.now().format(TS_FMT);
            String path = basePath.replace(".csv", "_" + ts + ".csv");

            writer = new FileWriter(path);
            writer.write("gen,changes,best,avg,worst,best_genome\n");
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

        String genomeStr = serializeGenome(bestSol);

        // Detecta mejora histórica
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
                    fmt(avg) + "," +
                    fmt(worst) + "," +
                    "\"" + lastBestGenome + "\"\n"
            );
            writer.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private static String fmt(double x) {
        return String.format(Locale.US, "%.3f", x);
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
        try { writer.close(); }
        catch (IOException ignored) {}
    }
}
