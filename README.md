# SpaceEmulator

> NOTE: Local experimental plotting changes were reverted on 2026-02-04 to restore the stable repository state.
>
> A minimal C++ physics engine example focused on learning **OpenMP** parallelization and high-performance vector operations.

---

## Quick overview

- **Language:** C++17
- **Parallelism:** OpenMP (loop work-sharing, reduction, sections)
- **Build system:** CMake

---

## Prerequisites

- **CMake >= 3.10**
- **MinGW-w64 (GCC)** on Windows or another C++17-capable compiler
- **OpenMP** support (bundled with GCC)

> Tip: On Windows use **MSYS2/MINGW64** and ensure `C:\msys64\mingw64\bin` is on your `PATH` for `cmake` and `mingw32-make` to be available.

---

## Build

### Windows (MSYS2 / MINGW64)

1. Open an MSYS2 MINGW64 shell or Git Bash in the project root.
2. Create and enter the build folder:

```bash
mkdir -p build && cd build
```

3. Configure with MinGW Makefiles:

```bash
cmake -G "MinGW Makefiles" ..
```

4. Build:

```bash
mingw32-make
```

### Linux / macOS

```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -- -j$(nproc)
```

### Convenience script

Alternatively use the included helper script on POSIX systems:

```bash
./build.sh
```

If you need a Windows helper, use `build.bat` (if present) or follow the Windows instructions above.

---

## Run

After building, the executable will be in the `build/` directory.

On POSIX systems (Linux / macOS):

```bash
./PhysicsEngine
```

On Windows (MSYS2 / MINGW64 or PowerShell / CMD):

```powershell
.\PhysicsEngine.exe
# or from the repository root
build\PhysicsEngine.exe
```

---

## Project structure

- `include/` — header files (API)
- `src/` — implementation files
- `CMakeLists.txt` — project build configuration
- `AGENTS.md` — AI-assistant context

---

## Parallel features implemented

- **Parallel loop work-sharing** using `#pragma omp parallel for`
- **Reduction** for safe accumulation (e.g., dot products)
- **Sections** for independent parallel tasks

---

## Gravity simulation

A simple Newtonian point-mass gravity example has been added. Use `simulate_gravity_step(std::vector<Body>& bodies, double dt, double G)` to advance a set of `Body` point masses by one timestep. The `main` demo runs a tiny two-body scenario and prints a readable table (per-step) of positions (meters) and velocities (m/s).

> Note: The implementation is O(N^2) pairwise interactions with a small softening term; replace with Barnes–Hut or other hierarchical method for large N.

### CSV output & plotting

The `main` program supports CSV dumps for post-processing (columns: `step,body,x,y,vx,vy`):

```bash
./PhysicsEngine.exe --csv out.csv
```

The simulator includes simple CLI options for running scenarios and controlling output:

- `--scenario {two-body,three-body,random}`
- `--steps N` (number of steps)
- `--dt seconds` (timestep size)
- `--sample interval` (sample every N steps)
- `--n N` and `--seed S` (for `random` scenario)

Example — random 6-body run dumping CSV:

```bash
./PhysicsEngine.exe --scenario random --n 6 --seed 1 --steps 1000 --dt 60 --csv scripts/complex_gravity.csv --sample 1
```

Use the included plotting helper to visualize results (no extra Python runtime deps required; it writes SVGs):

- Trajectory (x vs y) — default

```bash
python scripts/plot_gravity.py scripts/complex_gravity.csv -o scripts/complex_trajectories.svg
```

- Distance vs time (pairwise distances) — use `-t distance` and `--pairs` to select pairs (e.g. `0-1` or `all`):

```bash
python scripts/plot_gravity.py scripts/complex_gravity.csv -t distance --pairs 0-1 -o scripts/pair_0-1.svg
```

## To convert SVG to PNG use a tool like Inkscape or Pillow (note: Pillow may require system libs such as zlib/libjpeg on Windows if built from source).

### Running tests

You can build and run the included basic tests by enabling the `ENABLE_TESTING` CMake option and using CTest:

```bash
mkdir -p build && cd build
cmake -DENABLE_TESTING=ON -G "MinGW Makefiles" ..
mingw32-make
ctest -V
```

The simple `gravity_test` checks basic momentum conservation for a tiny two-body scenario.

---

## Troubleshooting

- "cmake: command not found" or "mingw32-make: command not found": add your MINGW64 `bin` path to `PATH`.
- If CMake generates a Visual Studio solution unintentionally, re-run with `-G "MinGW Makefiles"`.
- For missing OpenMP support, ensure you're using a GCC toolchain with OpenMP enabled.

---

## Clean helpers

Use the helper script to quickly clean and recreate the `build/` directory:

- POSIX / Git Bash: `./build.sh`
- Windows (MSYS2 / MINGW64): `build.bat` (if present) or follow the Windows build steps above)

---

## Tips

- Use `ccache` and parallel build flags to speed up iterative builds.
- Consider adding a `CMakePresets.json` for standardized invocation across machines.

---

