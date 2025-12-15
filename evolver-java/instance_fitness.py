import csv
from pathlib import Path

LOG_DIR = Path("calibration_1")
OUT_FILE = "piecewise_top10_best_fitness.csv"

def is_balanced_top10(name: str) -> bool:
    return (
        name.endswith(".csv")
        and "_TOP10_" in name
        and "inst=PIECEWISE" in name
    )

rows = []

for file in LOG_DIR.iterdir():
    if not is_balanced_top10(file.name):
        continue

    with file.open(newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            if row["rank"] == "1":
                rows.append(float(row["fitness"]))
                break

with open(OUT_FILE, "w", newline="") as f:
    w = csv.writer(f)
    w.writerow(["fitness"])
    for v in rows:
        w.writerow([v])

print(f"[OK] {OUT_FILE} generado con {len(rows)} filas")
