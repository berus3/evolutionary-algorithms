import sys
import csv

if len(sys.argv) != 3:
    print("Usage: python print_column.py <file.csv> <column_index>")
    sys.exit(1)

csv_path = sys.argv[1]
col_idx = int(sys.argv[2])

with open(csv_path, newline="") as f:
    reader = csv.reader(f)
    header = next(reader)

    if col_idx < 0 or col_idx >= len(header):
        raise ValueError(
            f"Invalid column index {col_idx}. "
            f"CSV has {len(header)} columns."
        )

    for row in reader:
        print(row[col_idx])
