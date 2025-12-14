import csv
from pathlib import Path
from collections import defaultdict
import statistics

LOG_DIR = Path("hofs")
OUT_FILE = "parsed_logs.csv"

def parse_filename(name: str):
    if "_HOF_" not in name:
        return None

    base = name[:-4]  # quitar .csv
    parts = base.split("_")

    kv = {}
    for p in parts:
        if "=" in p:
            k, v = p.split("=", 1)
            kv[k] = v

    required = ["seed", "pop", "p0", "cross", "inst"]
    for r in required:
        if r not in kv:
            return None

    return {
        "seed": int(kv["seed"]),
        "pop": kv["pop"],
        "p0": kv["p0"],
        "cross": kv["cross"],
        "inst": kv["inst"],
    }


# configuracion -> list of (fitness, winrate)
data = defaultdict(list)

files = list(LOG_DIR.iterdir())
print(f"[INFO] Archivos encontrados en hofs/: {len(files)}")

for file in files:
    if not file.name.endswith(".csv"):
        continue

    info = parse_filename(file.name)
    if info is None:
        continue

    config = f"pop={info['pop']},p0={info['p0']},cross={info['cross']},inst={info['inst']}"

    with open(file, newline="") as f:
        reader = csv.reader(f)

        header = next(reader, None)  # SE IGNORA (está mal)
        row = next(reader, None)     # mejor individuo

        if row is None or len(row) < 3:
            continue

        try:
            fitness = float(row[0])   # Ahora fitness está en la columna 1 (segunda columna)
            winrate = float(row[1])   # Ahora winrate está en la columna 2 (tercera columna)
        except ValueError as e:
            print(f"[WARN] No se pudo parsear {file.name}: {e}")
            continue

        data[config].append((fitness, winrate))


with open(OUT_FILE, "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow([
        "configuracion",
        "fitness_promedio",
        "fitness_std",
        "winrate_promedio",
        "winrate_std",
        "n_seeds"
    ])

    for config, values in sorted(data.items()):
        fitness_vals = [v[0] for v in values]
        winrate_vals = [v[1] for v in values]

        writer.writerow([
            config,
            statistics.mean(fitness_vals),
            statistics.stdev(fitness_vals) if len(fitness_vals) > 1 else 0.0,
            statistics.mean(winrate_vals),
            statistics.stdev(winrate_vals) if len(winrate_vals) > 1 else 0.0,
            len(values)
        ])

print(f"[OK] {OUT_FILE} generado ({len(data)} configuraciones)")
