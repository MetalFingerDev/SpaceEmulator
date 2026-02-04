#!/usr/bin/env python3
"""Simple CSV plot helper for SpaceEmulator gravity output
Usage:
  python scripts/plot_gravity.py out.csv
"""
import sys
import csv
import matplotlib.pyplot as plt
from collections import defaultdict

if len(sys.argv) < 2:
    print("Usage: plot_gravity.py <csv-file>")
    sys.exit(1)

path = sys.argv[1]
data = defaultdict(list)
with open(path, newline="") as f:
    reader = csv.DictReader(f)
    for row in reader:
        step = int(row["step"])
        body = int(row["body"])
        x = float(row["x"])
        y = float(row["y"])
        data[body].append((step, x, y))

for body, rows in data.items():
    rows.sort()
    steps = [r[0] for r in rows]
    xs = [r[1] for r in rows]
    ys = [r[2] for r in rows]
    plt.plot(xs, ys, label=f"body {body}", marker="o")

plt.xlabel("x (m)")
plt.ylabel("y (m)")
plt.title("Gravity simulation (trajectory)")
plt.legend()
plt.grid(True)
plt.show()
