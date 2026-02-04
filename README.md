# SpaceEmulator

A minimal C++ physics engine example focused on learning **OpenMP** parallelization and high-performance vector operations.

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

## Build (Windows / MINGW64)

1. Open **Git Bash** or an MSYS2 MINGW64 shell in the project root.
2. Create and enter the build folder:

```bash
mkdir -p build && cd build
```

3. Configure with MinGW Makefiles (important on Windows):

```bash
cmake -G "MinGW Makefiles" ..
```

4. Build:

```bash
mingw32-make
```

> If you prefer out-of-source builds for different configurations:
>
> ```bash
> cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../
> mingw32-make
> ```

---

## Run

After building, the executable will be in the `build/` directory:

```bash
./PhysicsEngine.exe
```

On Windows PowerShell or CMD use `.uild\PhysicsEngine.exe`.

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

The `main` program supports a simple CSV dump for post-processing:

```bash
./PhysicsEngine --csv out.csv
```

Use the included plotting helper to visualize trajectories (requires Python + matplotlib):

```bash
python scripts/plot_gravity.py out.csv
```

Install requirements:

```bash
pip install -r requirements.txt
```

---

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

Use the provided scripts to quickly clean and recreate the `build/` directory:

- Windows (PowerShell / CMD): `scripts\clean_build.bat`
- POSIX / Git Bash: `scripts/clean_build.sh`

---

## Tips

- Use `ccache` and parallel build flags to speed up iterative builds.
- Consider adding a `CMakePresets.json` for standardized invocation across machines.

---
