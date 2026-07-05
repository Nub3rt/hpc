import csv
import os

nodes_values = [1, 2, 4, 8, 16]
cores_values = [1, 2, 4, 8, 16]
edges_values = [20000, 40000, 80000, 160000, 320000]
tmult_values = [1, 2, 4, 8, 16]

edges_data = {}

for nodes in nodes_values:
    for cores in cores_values:
        for edges in edges_values:
            filename = f"mpi.o.{nodes}x{cores}x{edges}x16"
            path = os.path.join("../mpi_out_edges", filename)

            if not os.path.exists(path):
                print(f"error: file doesn't exist: {path}")
                continue

            run_sum = vote_sum = peaks_sum = 0.0
            count = 0

            with open(path, newline="") as f:
                reader = csv.DictReader(f)

                for row in reader:
                    run_sum += float(row["run_end"]) - float(row["run_start"])
                    vote_sum += float(row["vote_end"]) - float(row["vote_start"])
                    peaks_sum += float(row["peaks_end"]) - float(row["peaks_start"])
                    count += 1

            if count:
                edges_data[(nodes, cores, edges)] = (
                    run_sum / count,
                    vote_sum / count,
                    peaks_sum / count,
                )

tmult_data = {}

for nodes in nodes_values:
    for cores in cores_values:
        for tmult in tmult_values:
            filename = f"mpi.o.{nodes}x{cores}x320000x{tmult}"
            path = os.path.join("../mpi_out_theta", filename)

            if not os.path.exists(path):
                print(f"error: file doesn't exist: {path}")
                continue

            run_sum = vote_sum = peaks_sum = 0.0
            count = 0

            with open(path, newline="") as f:
                reader = csv.DictReader(f)

                for row in reader:
                    run_sum += float(row["run_end"]) - float(row["run_start"])
                    vote_sum += float(row["vote_end"]) - float(row["vote_start"])
                    peaks_sum += float(row["peaks_end"]) - float(row["peaks_start"])
                    count += 1

            if count:
                tmult_data[(nodes, cores, tmult)] = (
                    run_sum / count,
                    vote_sum / count,
                    peaks_sum / count,
                )

# ------------------------------------------------------------------
# EDGES TABLES
# ------------------------------------------------------------------

for nodes in nodes_values:
    print()
    print(f"EDGES TABLE (nodes = {nodes}, tmult = 16)")
    print()

    header = ["cores/edges"] + [str(e) for e in edges_values]
    print("\t".join(header))

    for cores in cores_values:
        for idx, label in enumerate(["run", "vote", "peaks"]):
            row = [f"{cores} {label}"]

            for edges in edges_values:
                value = edges_data.get((nodes, cores, edges))

                if value is None:
                    row.append("-")
                else:
                    row.append(f"{value[idx]:.2f}")

            print("\t".join(row))

# ------------------------------------------------------------------
# THETA TABLES
# ------------------------------------------------------------------

for nodes in nodes_values:
    print()
    print(f"THETA TABLE (nodes = {nodes}, edges = 320000)")
    print()

    header = ["cores/theta"] + [str(t) for t in tmult_values]
    print("\t".join(header))

    for cores in cores_values:
        for idx, label in enumerate(["run", "vote", "peaks"]):
            row = [f"{cores} {label}"]

            for tmult in tmult_values:
                value = tmult_data.get((nodes, cores, tmult))

                if value is None:
                    row.append("-")
                else:
                    row.append(f"{value[idx]:.2f}")

            print("\t".join(row))
