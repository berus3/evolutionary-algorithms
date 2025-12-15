import csv
from pathlib import Path
from collections import defaultdict
import statistics

LOG_DIR = Path("calibration_CF")   # ajustá si hace falta
OUT_FILE = "top10_best_stats_by_config.csv"

def parse_filename(name: str):
    if "_TOP10_" not in name or not name.endswith(".csv"):
        return None

    base = name[:-4]
    parts = base.split("_")

    kv = {}
    for p in parts:
        if "=" in p:
            k, v = p.split("=", 1)
            kv[k] = v

    if "inst" not in kv or "seed" not in kv:
        return None

    return {
        "inst": kv["inst"],
        "seed": int(kv["seed"]),
        "pop": kv.get("pop"),
        "p0": kv.get("p0"),
        "cross": kv.get("cross"),
    }

def read_best_individual(path: Path):
    # rank,fitness,winrate,genome
    with path.open(newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            if row["rank"] == "1":
                return float(row["fitness"]), float(row["winrate"])
    return None

# config -> list[(fitness, winrate)]
data = defaultdict(list)

for file in LOG_DIR.iterdir():
    info = parse_filename(file.name)
    if info is None:
        continue

    best = read_best_individual(file)
    if best is None:
        continue

    config = f"pop={info['pop']},p0={info['p0']},cross={info['cross']},inst={info['inst']}"
    data[config].append(best)

with open(OUT_FILE, "w", newline="") as f:
    w = csv.writer(f)
    w.writerow([
        "configuracion",
        "fitness_promedio",
        "fitness_std",
        "winrate_promedio",
        "winrate_std",
        "n_seeds",
    ])

    for config, values in sorted(data.items()):
        fitness_vals = [v[0] for v in values]
        winrate_vals = [v[1] for v in values]

        w.writerow([
            config,
            statistics.mean(fitness_vals),
            statistics.stdev(fitness_vals) if len(fitness_vals) > 1 else 0.0,
            statistics.mean(winrate_vals),
            statistics.stdev(winrate_vals) if len(winrate_vals) > 1 else 0.0,
            len(values)
        ])

print(f"[OK] {OUT_FILE} generado")
