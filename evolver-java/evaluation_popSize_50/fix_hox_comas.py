import pathlib

BASE_DIR = pathlib.Path(".")

def fix_file(path: pathlib.Path):
    lines = path.read_text().splitlines()
    if not lines:
        return False

    fixed = [lines[0]]  # header
    changed = False

    for line in lines[1:]:
        if not line.strip():
            fixed.append(line)
            continue

        # separar en: prefix , genome
        try:
            prefix, genome = line.rsplit(",", 1)
        except ValueError:
            fixed.append(line)
            continue

        genome = genome.strip()

        # si ya tiene comillas, no tocar
        if genome.startswith('"') and genome.endswith('"'):
            fixed.append(line)
            continue

        # agregar comillas
        fixed.append(f'{prefix},"{genome}"')
        changed = True

    if changed:
        path.write_text("\n".join(fixed) + "\n")

    return changed


def main():
    hof_files = list(BASE_DIR.glob("*_HOF_*.csv"))

    if not hof_files:
        print("No HOF files found.")
        return

    fixed_count = 0
    for f in hof_files:
        if fix_file(f):
            print(f"Fixed: {f.name}")
            fixed_count += 1

    print(f"\nDone. Fixed {fixed_count}/{len(hof_files)} HOF files.")


if __name__ == "__main__":
    main()
