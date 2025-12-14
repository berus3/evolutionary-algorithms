package org.evol;

import org.uma.jmetal.solution.integersolution.IntegerSolution;

import java.util.ArrayList;
import java.util.List;

public class HallOfFame {

    public static class Entry {
        public final IntegerSolution sol;
        public final double fitness;

        public Entry(IntegerSolution sol, double fitness) {
            this.sol = sol;
            this.fitness = fitness;
        }
    }

    private final int maxSize;
    private final List<Entry> entries = new ArrayList<>();

    public HallOfFame(int maxSize) {
        this.maxSize = maxSize;
    }

    public List<Entry> getEntries() {
        return entries;
    }

	private boolean sameGenome(IntegerSolution a, IntegerSolution b) {
		return a.variables().equals(b.variables());
	}



    public void tryInsert(IntegerSolution sol) {
		for (Entry e : entries) {
			if (sameGenome(e.sol, sol)) {
				return;
			}
		}

		Object f = sol.attributes().get("fitness");
		if (!(f instanceof Double)) return;

		double fitness = (Double) f;
		IntegerSolution clone = (IntegerSolution) sol.copy();

		if (entries.size() < maxSize) {
			entries.add(new Entry(clone, fitness));
			return;
		}

		int worstIdx = 0;
		double worstFit = entries.get(0).fitness;

		for (int i = 1; i < entries.size(); i++) {
			if (entries.get(i).fitness < worstFit) {
				worstFit = entries.get(i).fitness;
				worstIdx = i;
			}
		}

		if (fitness <= worstFit) return;

		entries.set(worstIdx, new Entry(clone, fitness));
	}

}
