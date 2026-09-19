from __future__ import annotations

from pathlib import Path

import matplotlib
import numpy as np
import pandas as pd

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Patch

SIZES = [128, 256, 500, 512, 750, 1024]  # plotting order (numeric)
PARAMS = [8, 16, 32, 64, 128]  # b and t values

NAIVE_COLOR = "#fff"
PARAM_COLORS = [
    "#ff0000",  # red
    "#ffa500",  # orange
    "#008000",  # green
    "#800080",  # purple
    "#104281",  # blue
]  # one blue per b/t
AWARE_COLOR = "#eb6834"
OBLIV_COLOR = "#1baf7a"
PAD_COLOR = "#eda100"

plt.rcParams.update(
    {
        "figure.facecolor": "white",
        "axes.grid": True,
        "grid.color": "#e6e5e1",
        "grid.linewidth": 0.6,
        "axes.spines.top": False,
        "axes.spines.right": False,
        "axes.axisbelow": True,
        "font.size": 10,
        "legend.frameon": False,
    }
)


def load(path: str) -> pd.DataFrame:
    df = pd.read_csv(path)
    df.columns = [c.strip() for c in df.columns]
    df["matrix_size"] = df["matrix_size"].astype(int)
    df["time_ms"] = df["time_ms"].astype(float)
    if "block_or_threshold" in df:
        df["block_or_threshold"] = (
            pd.to_numeric(df["block_or_threshold"], errors="coerce")
            .fillna(0)
            .astype(int)
        )
    if "elapsed_padding_ms" in df:
        df["elapsed_padding_ms"] = pd.to_numeric(
            df["elapsed_padding_ms"], errors="coerce"
        )
    print(f"loaded {len(df)} rows from {path}")
    return df


def rep_value(values: np.ndarray, stat: str) -> float:
    return float(np.median(values)) if stat == "median" else float(np.mean(values))


def draw_boxes(
    ax,
    groups: list[np.ndarray],
    positions: list[float],
    colors: list[str],
    width: float,
    stat: str,
    log: bool,
) -> None:
    """Draw one box per group, scatter the raw runs, and label the representative value."""
    groups = [np.asarray(g, dtype=float) for g in groups]
    bp = ax.boxplot(
        groups,
        positions=positions,
        widths=width,
        patch_artist=True,
        showmeans=True,
        showfliers=False,
        medianprops=dict(color="black", linewidth=1.4),
        meanprops=dict(
            marker="D", markerfacecolor="white", markeredgecolor="black", markersize=5
        ),
        whiskerprops=dict(color="#52514e"),
        capprops=dict(color="#52514e"),
    )
    for patch, c in zip(bp["boxes"], colors):
        patch.set_facecolor(c)
        patch.set_alpha(0.75)
        patch.set_edgecolor("#333333")
    if log:
        ax.set_yscale("log")
    rng = np.random.default_rng(0)
    valid = [g[~np.isnan(g)] for g in groups if np.any(~np.isnan(g))]
    lo = min(np.min(g) for g in valid)
    hi = max(np.max(g) for g in valid)
    span = hi - lo if hi > lo else abs(hi) or 1.0
    for g, x in zip(groups, positions):
        g = g[~np.isnan(g)]
        if len(g) == 0:
            continue
        jitter = rng.uniform(-width * 0.25, width * 0.25, size=len(g))
        ax.scatter(
            x + jitter, g, s=9, color="black", alpha=0.45, zorder=3, linewidths=0
        )
        r = rep_value(g, stat)
        label = f"{r:.3g}" if r < 100 else f"{r:.0f}"
        top = np.max(g)
        y = top * 1.08 if log else top + 0.04 * span
        ax.text(x, top, label, ha="center", va="bottom", fontsize=7, color="#333333")
    # Text is not part of autoscaling, so set the y-limits explicitly with room for the labels.
    # if log:
    #     ax.set_ylim(lo / 1.3, hi * 1.6)
    # else:
    #     ax.set_ylim(lo - 0.08 * span, hi + 0.22 * span)


def finish(fig, ax, title: str, ylabel: str, out: Path, name: str) -> None:
    ax.set_title(title)
    ax.set_ylabel(ylabel)
    ax.set_xlabel("matrix size n")
    fig.tight_layout()
    fig.savefig(out / name, dpi=180)
    plt.close(fig)
    print("wrote", out / name)


# Plots
def plot_naive(df: pd.DataFrame, stat: str, log: bool, out: Path, rows: list) -> None:
    sizes = [s for s in SIZES if s in set(df.matrix_size)]
    groups = [df.loc[df.matrix_size == s, "time_ms"].to_numpy() for s in sizes]
    fig, ax = plt.subplots(figsize=(8.27, 11.69))
    draw_boxes(
        ax, groups, list(range(len(sizes))), [NAIVE_COLOR] * len(sizes), 0.55, stat, log
    )
    ax.set_xticks(range(len(sizes)), [str(s) for s in sizes])
    for s, g in zip(sizes, groups):
        rows.append(
            (
                "naive",
                s,
                "",
                len(g),
                np.median(g),
                np.mean(g),
                np.std(g, ddof=1) if len(g) > 1 else 0,
            )
        )
    finish(
        fig,
        ax,
        f"Naive Matrix Multiplication ({stat} labelled)",
        "Execution Time (ms)",
        out,
        "naive_boxplot.png",
    )


def plot_param_sweep(
    df: pd.DataFrame,
    naive: pd.DataFrame | None,
    name: str,
    param_label: str,
    stat: str,
    log: bool,
    out: Path,
    rows: list,
    fname: str,
) -> None:
    """One panel per matrix size (2 x 3 grid), each with its own y-axis, so the
    spread of every box is visible even though 128 and 1024 differ by ~500x."""
    sizes = [s for s in SIZES if s in set(df.matrix_size)]
    params = [p for p in PARAMS if p in set(df.block_or_threshold)]
    ncols = 3
    nrows = int(np.ceil(len(sizes) / ncols))
    fig, axes = plt.subplots(
        nrows, ncols, figsize=(4.6 * ncols, 6.0 * nrows), squeeze=False
    )

    for ax, s in zip(axes.flat, sizes):
        groups, colors, ticks = [], [], []
        if naive is not None:
            groups.append(naive.loc[naive.matrix_size == s, "time_ms"].to_numpy())
            colors.append(NAIVE_COLOR)
            ticks.append("naive")
        for j, p in enumerate(params):
            g = df.loc[
                (df.matrix_size == s) & (df.block_or_threshold == p), "time_ms"
            ].to_numpy()
            groups.append(g)
            colors.append(PARAM_COLORS[j % len(PARAM_COLORS)])
            ticks.append(f"{param_label}={p}")
            rows.append(
                (
                    name,
                    s,
                    p,
                    len(g),
                    np.median(g) if len(g) else np.nan,
                    np.mean(g) if len(g) else np.nan,
                    np.std(g, ddof=1) if len(g) > 1 else 0,
                )
            )
        groups = [
            g if len(g) else np.array([np.nan]) for g in groups
        ]  # boxplot() rejects empty groups
        draw_boxes(ax, groups, list(range(len(groups))), colors, 0.6, stat, log)
        ax.set_xticks(range(len(groups)), ticks, fontsize=8)
        ax.set_title(f"N = {s}", fontsize=11)
        ax.set_ylabel("Time (ms)")
        ax.margins(y=0.15)  # room for the value labels

    for ax in list(axes.flat)[len(sizes) :]:
        ax.set_visible(False)

    fig.suptitle(
        f"{name} Matrix Multiplication, one panel per matrix size ({stat} labelled)",
        fontsize=12,
    )
    fig.tight_layout(rect=(0, 0, 1, 0.96))
    fig.savefig(out / fname, dpi=180)
    plt.close(fig)
    print("wrote", out / fname)


def plot_padding(df: pd.DataFrame, stat: str, out: Path, rows: list) -> None:
    if "elapsed_padding_ms" not in df or df["elapsed_padding_ms"].isna().all():
        print("no elapsed_padding_ms column with data; padding plot skipped")
        return
    sizes = [s for s in SIZES if s in set(df.matrix_size)]
    groups = [
        df.loc[df.matrix_size == s, "elapsed_padding_ms"].dropna().to_numpy()
        for s in sizes
    ]
    fig, ax = plt.subplots(figsize=(8, 4.8))
    draw_boxes(
        ax,
        [g if len(g) else np.array([np.nan]) for g in groups],
        list(range(len(sizes))),
        [PAD_COLOR] * len(sizes),
        0.55,
        stat,
        False,
    )
    padded = {s: 1 << (s - 1).bit_length() for s in sizes}
    ax.set_xticks(
        range(len(sizes)),
        [
            f"{s}\n(Padded to {padded[s]})" if padded[s] != s else f"{s}\n(No Padding)"
            for s in sizes
        ],
    )
    for s, g in zip(sizes, groups):
        rows.append(
            (
                "Padding",
                s,
                "",
                len(g),
                np.median(g) if len(g) else np.nan,
                np.mean(g) if len(g) else np.nan,
                np.std(g, ddof=1) if len(g) > 1 else 0,
            )
        )
    finish(
        fig,
        ax,
        f"Cache-oblivious: Time Spent Padding the Matrices ({stat} Labelled)",
        "Padding Time (ms)",
        out,
        "padding_overhead_boxplot.png",
    )


def main() -> None:
    output_path = Path("../figures/")

    rows: list = []
    naive_csv = load("../logs/naive_timings.csv")
    cache_aware_csv = load("../logs/cache_aware_timings.csv")
    cache_oblivious_csv = load("../logs/cache_oblivious_timings.csv")

    stat = "median"
    log = True

    plot_naive(naive_csv, stat, log, output_path, rows)
    plot_param_sweep(
        cache_aware_csv,
        naive_csv,
        "Cache-Aware",
        "b",
        stat,
        log,
        output_path,
        rows,
        "cache_aware_boxplot.png",
    )

    plot_param_sweep(
        cache_oblivious_csv,
        naive_csv,
        "Cache-Oblivious",
        "t",
        stat,
        log,
        output_path,
        rows,
        "cache_oblivious_boxplot.png",
    )

    summary = pd.DataFrame(
        rows,
        columns=[
            "algorithm",
            "matrix_size",
            "b_or_t",
            "runs",
            "median_ms",
            "mean_ms",
            "std_ms",
        ],
    )
    plot_padding(cache_oblivious_csv, stat, output_path, rows)
    summary.to_csv(output_path / f"summary_{stat}.csv", index=False)
    print("wrote", output_path / f"summary_{stat}.csv")


if __name__ == "__main__":
    main()
