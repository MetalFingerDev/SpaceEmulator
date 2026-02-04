#!/usr/bin/env python3
"""Plot gravity CSV to an SVG (no dependencies).

Usage:
  python scripts/plot_gravity.py data.csv -o out.svg
"""
import argparse
import csv
from collections import defaultdict


def write_svg(data, out_path, w=800, h=600):
    all_x = [x for rows in data.values() for (_, x, _) in rows]
    all_y = [y for rows in data.values() for (_, _, y) in rows]
    if not all_x or not all_y:
        raise SystemExit("No data to plot")

    min_x, max_x = min(all_x), max(all_x)
    min_y, max_y = min(all_y), max(all_y)
    dx = max_x - min_x or 1.0
    dy = max_y - min_y or 1.0
    pad = 0.05
    min_x -= dx * pad
    max_x += dx * pad
    min_y -= dy * pad
    max_y += dy * pad

    def sx(x):
        return int((x - min_x) / (max_x - min_x) * (w - 60) + 30)

    def sy(y):
        return int(h - ((y - min_y) / (max_y - min_y) * (h - 60) + 30))

    colors = [
        "#e41a1c",
        "#377eb8",
        "#4daf4a",
        "#984ea3",
        "#ff7f00",
        "#a65628",
        "#f781bf",
        "#999999",
    ]

    lines = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="{h}">',
        '<rect width="100%" height="100%" fill="white"/>',
        '<text x="30" y="20" font-size="14">Gravity simulation</text>',
    ]

    for i, (body, rows) in enumerate(sorted(data.items())):
        rows.sort()
        pts = " ".join(f"{sx(x)},{sy(y)}" for (_, x, y) in rows)
        col = colors[i % len(colors)]
        if pts:
            lines.append(
                f'<polyline points="{pts}" fill="none" stroke="{col}" stroke-width="2"/>'
            )
            for _, x, y in rows:
                lines.append(f'<circle cx="{sx(x)}" cy="{sy(y)}" r="3" fill="{col}"/>')
        lines.append(
            f'<text x="{w-150}" y="{30 + i*16}" font-size="12">body {body}</text>'
        )

    lines.append(f'<text x="30" y="{h-10}" font-size="12">x (m)</text>')
    lines.append(
        f'<text x="10" y="30" font-size="12" transform="rotate(-90 10,30)">y (m)</text>'
    )
    lines.append("</svg>")

    with open(out_path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))


def write_distance_svg(data, out_path, pairs=None, w=800, h=600):
    # Prepare per-body dict of step -> (x,y)
    body_steps = {
        b: {step: (x, y) for (step, x, y) in rows} for b, rows in data.items()
    }
    steps = sorted({step for rows in data.values() for (step, _, _) in rows})
    if not steps:
        raise SystemExit("No steps available")

    # build list of pairs
    bodies = sorted(data.keys())
    if pairs is None or pairs == "all":
        pairs = [
            (bodies[i], bodies[j])
            for i in range(len(bodies))
            for j in range(i + 1, len(bodies))
        ]
    else:
        # parse string like '0-1,0-2'
        parsed = []
        for p in pairs.split(","):
            a, b = p.split("-")
            parsed.append((int(a), int(b)))
        pairs = parsed

    # compute distances per pair per step
    series = {}
    all_d = []
    for a, b in pairs:
        pts = []
        for s in steps:
            if (
                a in body_steps
                and b in body_steps
                and s in body_steps[a]
                and s in body_steps[b]
            ):
                xa, ya = body_steps[a][s]
                xb, yb = body_steps[b][s]
                d = ((xa - xb) ** 2 + (ya - yb) ** 2) ** 0.5
                pts.append((s, d))
                all_d.append(d)
        series[(a, b)] = pts

    if not all_d:
        raise SystemExit("No distance data computed")

    min_s, max_s = min(steps), max(steps)
    min_d, max_d = min(all_d), max(all_d)
    pad = 0.05
    min_d -= (max_d - min_d) * pad
    max_d += (max_d - min_d) * pad

    def sx_step(s):
        return int((s - min_s) / (max_s - min_s or 1.0) * (w - 60) + 30)

    def sy_d(d):
        return int(h - ((d - min_d) / (max_d - min_d or 1.0) * (h - 60) + 30))

    colors = ["#e41a1c", "#377eb8", "#4daf4a", "#984ea3", "#ff7f00", "#a65628"]

    lines = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{w}" height="{h}">',
        '<rect width="100%" height="100%" fill="white"/>',
        '<text x="30" y="20" font-size="14">Inter-body distance vs time</text>',
    ]

    for i, ((a, b), pts) in enumerate(sorted(series.items())):
        color = colors[i % len(colors)]
        pts_svg = " ".join(f"{sx_step(s)},{sy_d(d)}" for (s, d) in pts)
        if pts_svg:
            lines.append(
                f'<polyline points="{pts_svg}" fill="none" stroke="{color}" stroke-width="2"/>'
            )
            for s, d in pts:
                lines.append(
                    f'<circle cx="{sx_step(s)}" cy="{sy_d(d)}" r="2" fill="{color}"/>'
                )
        lines.append(
            f'<text x="{w-240}" y="{30 + i*16}" font-size="12">pair {a}-{b}</text>'
        )

    lines.append(f'<text x="30" y="{h-10}" font-size="12">step</text>')
    lines.append(
        f'<text x="10" y="30" font-size="12" transform="rotate(-90 10,30)">distance (m)</text>'
    )
    lines.append("</svg>")

    with open(out_path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines))


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("csvfile", help="CSV file with columns step,body,x,y")
    p.add_argument("-o", "--out", help="Output SVG", default="trajectory.svg")
    p.add_argument(
        "-t", "--type", help="Plot type: trajectory or distance", default="trajectory"
    )
    p.add_argument(
        "--pairs", help="Pairs for distance (e.g. '0-1,0-2') or 'all'", default="all"
    )
    args = p.parse_args()

    data = defaultdict(list)
    with open(args.csvfile, newline="") as f:
        for r in csv.DictReader(f):
            data[int(r["body"])].append((int(r["step"]), float(r["x"]), float(r["y"])))

    if args.type == "trajectory":
        write_svg(data, args.out)
        print(f"Wrote SVG to {args.out}")
    elif args.type == "distance":
        write_distance_svg(
            data, args.out, pairs=(None if args.pairs == "all" else args.pairs)
        )
        print(f"Wrote distance SVG to {args.out}")
    else:
        raise SystemExit(f"Unknown plot type: {args.type}")


if __name__ == "__main__":
    main()
