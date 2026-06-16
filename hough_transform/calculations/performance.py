import csv
import os

cores_values = [1, 2, 4, 8, 16]
edges_values = [20000, 40000, 80000, 160000, 320000]
tmult_values = [1, 2, 4, 8, 16]

edges_data = {}
for cores in cores_values:
    for edges in edges_values:
        filename = f"mpi.o.{cores}x{edges}x16"
        path = os.path.join("../mpi_out_edges", filename)

        if not os.path.exists(path):
            print(f"error: file doesn't exist: {path}")
            continue

        vote_sum = 0.0
        count = 0

        with open(path, newline="") as f:
            reader = csv.DictReader(f)

            for row in reader:
                vote_sum += float(row["vote_end"]) - float(row["vote_start"])
                count += 1

        if count:
            edges_data[(cores, edges)] = vote_sum / count

tmult_data = {}
for cores in cores_values:
    for tmult in tmult_values:
        filename = f"mpi.o.{cores}x320000x{tmult}"
        path = os.path.join("../mpi_out_theta", filename)

        if not os.path.exists(path):
            print(f"error: file doesn't exist: {path}")
            continue

        vote_sum = 0.0
        count = 0

        with open(path, newline="") as f:
            reader = csv.DictReader(f)

            for row in reader:
                vote_sum += float(row["vote_end"]) - float(row["vote_start"])
                count += 1

        if count:
            tmult_data[(cores, tmult)] = vote_sum / count

print("EDGES TABLE (t_mult = 16)")
print()

header = ["cores/edges"] + [str(x) for x in edges_values]
print("\t".join(header))

for label in ["vote", "speedup", "efficiency"]:
    for cores in cores_values:
        row = [f"{cores} {label}"]

        for edges in edges_values:
            vote = edges_data.get((cores, edges))
            base = edges_data.get((1, edges))

            if vote is None or base is None:
                row.append("-")
            elif label == "vote":
                row.append(f"{vote:.6f}")
            elif label == "speedup":
                row.append(f"{base / vote:.6f}")
            else:
                row.append(f"{base / vote / cores:.6f}")

        print("\t".join(row))

print()
print("T_MULT TABLE (edges = 320000)")
print()

header = ["cores/theta"] + [str(x) for x in tmult_values]
print("\t".join(header))

for label in ["vote", "speedup", "efficiency"]:
    for cores in cores_values:
        row = [f"{cores} {label}"]

        for tmult in tmult_values:
            vote = tmult_data.get((cores, tmult))
            base = tmult_data.get((1, tmult))

            if vote is None or base is None:
                row.append("-")
            elif label == "vote":
                row.append(f"{vote:.6f}")
            elif label == "speedup":
                row.append(f"{base / vote:.6f}")
            else:
                row.append(f"{base / vote / cores:.6f}")

        print("\t".join(row))
