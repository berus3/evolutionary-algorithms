import csv
from pathlib import Path

# ===== PARAMETROS =====
LOG_DIR = Path("calibration_popSize")
INST = "PIECEWISE"
POP = "100"
P0 = "0.05"
CROSS = "0.25"

OUT_FILE = f"_results_inst={INST}_pop={POP}_p0={P0}_cross={CROSS}.csv"
# ======================

def matches_config(name: str) -> bool:
    return (
        name.endswith(".csv")
        and "_TOP10_" in name
        and f"inst={INST}" in name
        and f"pop={POP}" in name
        and f"p0={P0}" in name
        and f"cross={CROSS}" in name
    )

rows = []

for file in LOG_DIR.iterdir():
    if not matches_config(file.name):
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
