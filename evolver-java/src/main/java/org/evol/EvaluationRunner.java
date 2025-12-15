package org.evol;

import java.io.*;
import java.nio.file.*;
import java.util.*;

public class EvaluationRunner {

    private static final List<Integer> SEEDS = List.of(
            31,32,33,34,35,36,37,38,39,40,
            41,42,43,44,45,46,47,48,49,50,
            51,52,53,54,55,56,57,58,59,60
    );

    private static final List<RPGInstance> INSTANCES = List.of(
            RPGInstance.SUPPORT_LIKE_PIECEWISE,
            RPGInstance.FULL_DAMAGE_PIECEWISE,
            RPGInstance.LINEAR
    );

    private static final int FIGHTS_PER_REFERENCE = 10;

    private static final Path REF_PATH =
            Paths.get("evaluation_popSize_100/reference_tiers.csv");

    private static final Path LOG_DIR =
            Paths.get("evaluation_popSize_100/results");

    public static void main(String[] args) throws Exception {

        Files.createDirectories(LOG_DIR);

        int[] refGenomes = loadReferenceGenomes();
        int refCount = refGenomes.length / RPGProblem.GENOME_SIZE;

        for (RPGInstance instance : INSTANCES) {

            Path out = LOG_DIR.resolve(
                    "evaluation_popSize_100_" + instance.name() + ".csv");

            try (PrintWriter pw = new PrintWriter(Files.newBufferedWriter(out))) {

                pw.print("seed,ae_fitness,ae_winrate");
                for (int i = 0; i < refCount; i++)
                    pw.print(",tier" + (i + 1) + "_winrate");
                pw.println();

                for (int seed : SEEDS) {

                    RPGNativeBridge.setSeed(seed);
                    RPGNativeBridge.setInstance(instance.id);

                    int[] popGenomes = loadFinalPopulation(seed, instance);
                    int[] hofGenomes = loadHallOfFame(seed, instance);
                    int[] bestAeGenome = loadBestAeGenome(seed, instance);

                    int[] evalGenomes = concat(bestAeGenome, refGenomes);
                    int evalCount = 1 + refCount;

                    double[] winrates =
                            RPGNativeBridge.evaluateReferences(
                                    evalGenomes,
                                    evalCount,
                                    popGenomes,
                                    popGenomes.length / RPGProblem.GENOME_SIZE,
                                    hofGenomes,
                                    hofGenomes.length / RPGProblem.GENOME_SIZE,
                                    FIGHTS_PER_REFERENCE
                            );

                    double aeFitness = loadBestAeFitness(seed, instance);

                    pw.print(seed + "," + aeFitness + "," + winrates[0]);
                    for (int i = 0; i < refCount; i++)
                        pw.print("," + winrates[i + 1]);
                    pw.println();
                }
            }
        }
    }

    // =========================================================
    // LOADERS
    // =========================================================

    private static int[] loadReferenceGenomes() throws IOException {
        List<int[]> genomes = new ArrayList<>();

        try (BufferedReader br = Files.newBufferedReader(REF_PATH)) {
            br.readLine(); // header
            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.split(",", 2);
                String[] genes = parts[1].replace("\"", "").trim().split("\\s+");

                if (genes.length != RPGProblem.GENOME_SIZE)
                    throw new IllegalStateException("Invalid reference genome size");

                int[] g = new int[genes.length];
                for (int i = 0; i < genes.length; i++)
                    g[i] = Integer.parseInt(genes[i]);

                genomes.add(g);
            }
        }
        return flatten(genomes);
    }

    private static int[] loadBestAeGenome(int seed, RPGInstance inst) throws IOException {
        Path p = findFile("evaluation_popSize_100", "TOP100", seed, inst);

        try (BufferedReader br = Files.newBufferedReader(p)) {
            br.readLine(); // header
            String line = br.readLine(); // best
            if (line == null) throw new IllegalStateException("Empty TOP100");

            String[] parts = line.split(",", 4);
            return parseGenome(parts[parts.length - 1]);
        }
    }

    private static int[] loadFinalPopulation(int seed, RPGInstance inst) throws IOException {
        Path p = findFile("evaluation_popSize_100", "TOP100", seed, inst);
        return loadGenomesFromCsv(p);
    }

    private static int[] loadHallOfFame(int seed, RPGInstance inst) throws IOException {
        Path p = findFile("evaluation_popSize_100", "HOF", seed, inst);
        return p == null ? new int[0] : loadGenomesFromCsv(p);
    }

    private static double loadBestAeFitness(int seed, RPGInstance inst)
        throws IOException {

    Path p = findFile(
            "evaluation_popSize_100",
            "TOP100",
            seed,
            inst
    );

    try (BufferedReader br = Files.newBufferedReader(p)) {
        String header = br.readLine(); // rank,fitness,winrate,genome
        if (header == null)
            throw new IllegalStateException("Empty TOP100 file: " + p.getFileName());

        String line = br.readLine(); // rank 1
        if (line == null)
            throw new IllegalStateException("No data in TOP100 file: " + p.getFileName());

        String[] parts = line.split(",", 4);

        // parts[1] = fitness
        return Double.parseDouble(parts[1]);
    }
}

    // =========================================================
    // HELPERS
    // =========================================================

    private static Path findFile(String dir, String tag, int seed, RPGInstance inst)
            throws IOException {

        try (var s = Files.list(Paths.get(dir))) {
            return s.filter(p -> {
                String f = p.getFileName().toString();
                if (!f.contains("seed=" + seed)) return false;
                if (!f.contains("inst=" + inst.name())) return false;
                if (tag != null && !f.contains(tag)) return false;
                return f.endsWith(".csv");
            }).findFirst().orElseThrow();
        }
    }

    private static int[] loadGenomesFromCsv(Path p) throws IOException {

    List<int[]> genomes = new ArrayList<>();

    try (BufferedReader br = Files.newBufferedReader(p)) {
        String header = br.readLine(); // header
        String line;

        while ((line = br.readLine()) != null) {
            line = line.trim();
            if (line.isEmpty() || line.startsWith("#"))
                continue;

            // separar SOLO por la última coma
            int lastComma = line.lastIndexOf(',');
            if (lastComma < 0)
                throw new IllegalStateException(
                        "No comma found in line of " + p.getFileName());

            String genomePart = line.substring(lastComma + 1)
                    .replace("\"", "")
                    .trim();

            String[] genes = genomePart.split("\\s+");

            if (genes.length != RPGProblem.GENOME_SIZE)
                throw new IllegalStateException(
                        "Invalid genome size (" + genes.length +
                        ") in " + p.getFileName());

            int[] g = new int[genes.length];
            for (int i = 0; i < genes.length; i++)
                g[i] = Integer.parseInt(genes[i]);

            genomes.add(g);
        }
    }

    return flatten(genomes);
}

    private static int[] parseGenome(String s) {
        String[] g = s.replace("\"", "").trim().split("\\s+");
        int[] out = new int[g.length];
        for (int i = 0; i < g.length; i++)
            out[i] = Integer.parseInt(g[i]);
        return out;
    }

    private static int[] concat(int[] a, int[] b) {
        int[] out = new int[a.length + b.length];
        System.arraycopy(a, 0, out, 0, a.length);
        System.arraycopy(b, 0, out, a.length, b.length);
        return out;
    }

    private static int[] flatten(List<int[]> list) {
        int g = RPGProblem.GENOME_SIZE;
        int[] out = new int[list.size() * g];
        for (int i = 0; i < list.size(); i++)
            System.arraycopy(list.get(i), 0, out, i * g, g);
        return out;
    }
}
