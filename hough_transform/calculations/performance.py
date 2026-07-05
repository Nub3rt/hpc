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

            vote_sum = 0.0
            count = 0

            with open(path, newline="") as f:
                reader = csv.DictReader(f)

                for row in reader:
                    vote_sum += (
                        float(row["vote_end"]) -
                        float(row["vote_start"])
                    )
                    count += 1

            if count:
                edges_data[(nodes, cores, edges)] = vote_sum / count

tmult_data = {}

for nodes in nodes_values:
    for cores in cores_values:
        for tmult in tmult_values:
            filename = f"mpi.o.{nodes}x{cores}x320000x{tmult}"
            path = os.path.join("../mpi_out_theta", filename)

            if not os.path.exists(path):
                print(f"error: file doesn't exist: {path}")
                continue

            vote_sum = 0.0
            count = 0

            with open(path, newline="") as f:
                reader = csv.DictReader(f)

                for row in reader:
                    vote_sum += (
                        float(row["vote_end"]) -
                        float(row["vote_start"])
                    )
                    count += 1

            if count:
                tmult_data[(nodes, cores, tmult)] = vote_sum / count

#
# EDGES TABLES
#

for nodes in nodes_values:

    print()
    print(f"EDGES TABLE (nodes = {nodes}, tmult = 16)")
    print()

    header = ["cores/edges"] + [str(e) for e in edges_values]
    print("\t".join(header))

    for label in ["vote", "speedup", "efficiency"]:
        for cores in cores_values:

            row = [f"{cores} {label}"]

            for edges in edges_values:

                vote = edges_data.get((nodes, cores, edges))
                base = edges_data.get((nodes, 1, edges))

                if vote is None or base is None:
                    row.append("-")
                elif label == "vote":
                    row.append(f"{vote:.6f}")
                elif label == "speedup":
                    row.append(f"{base / vote:.6f}")
                else:
                    row.append(f"{base / vote / cores:.6f}")

            print("\t".join(row))

#
# THETA TABLES
#

for nodes in nodes_values:

    print()
    print(f"THETA TABLE (nodes = {nodes}, edges = 320000)")
    print()

    header = ["cores/theta"] + [str(t) for t in tmult_values]
    print("\t".join(header))

    for label in ["vote", "speedup", "efficiency"]:
        for cores in cores_values:

            row = [f"{cores} {label}"]

            for tmult in tmult_values:

                vote = tmult_data.get((nodes, cores, tmult))
                base = tmult_data.get((nodes, 1, tmult))

                if vote is None or base is None:
                    row.append("-")
                elif label == "vote":
                    row.append(f"{vote:.6f}")
                elif label == "speedup":
                    row.append(f"{base / vote:.6f}")
                else:
                    row.append(f"{base / vote / cores:.6f}")

            print("\t".join(row))
