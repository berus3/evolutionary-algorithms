import csv
import glob
import os
import statistics

INPUT_PATTERN = "evaluation_popSize_100_*.csv"
OUTPUT_FILE = "evaluation_summary.csv"

def read_csv(path):
    with open(path, newline="") as f:
        return list(csv.DictReader(f))

def mean_sd(values):
    if len(values) == 0:
        return "", ""
    if len(values) == 1:
        return values[0], 0.0
    return statistics.mean(values), statistics.stdev(values)

rows_out = []

for path in sorted(glob.glob(INPUT_PATTERN)):
    instance = os.path.basename(path) \
        .replace("evaluation_popSize_100_", "") \
        .replace(".csv", "")

    rows = read_csv(path)

    cols = rows[0].keys()

    stats = {"instance": instance}

    for col in cols:
        if col == "seed":
            continue

        values = [float(r[col]) for r in rows]

        avg, sd = mean_sd(values)

        stats[f"avg_{col}"] = round(avg, 4)
        stats[f"sd_{col}"]  = round(sd, 4)

    rows_out.append(stats)

# ---- write output ----
fieldnames = ["instance"]
for k in rows_out[0]:
    if k != "instance":
        fieldnames.append(k)

with open(OUTPUT_FILE, "w", newline="") as f:
    writer = csv.DictWriter(f, fieldnames=fieldnames)
    writer.writeheader()
    for r in rows_out:
        writer.writerow(r)

print(f"Written {OUTPUT_FILE}")
