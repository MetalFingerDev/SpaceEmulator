# Agent Context: SpaceEmulator (C++/OpenMP) 🚀

## Project purpose

SpaceEmulator is a compact educational C++ physics engine example that demonstrates performance-oriented vector operations and simple task parallelism using OpenMP. It is intentionally small so contributors and learners can focus on parallel patterns, correctness, and build portability across Windows (MinGW) and POSIX systems.

---

## Quick facts

- **Language:** C++17
- **Parallelism:** OpenMP (loop work-sharing, reductions, sections)
- **Build system:** CMake (out-of-source builds encouraged)
- **Primary target:** Windows (MSYS2 / MinGW-w64) and Linux

---

## Repository layout

- `include/` — public headers (`physics_engine.hpp`)
- `src/` — implementation (`engine.cpp`, `main.cpp`)
- `scripts/` — legacy wrapper scripts (e.g., `clean_build.*`); prefer using root `build` (POSIX) and `build.bat` (Windows)
- `build/` — out-of-source build artifacts (gitignored)
- `CMakeLists.txt` — CMake configuration
- `AGENTS.md`, `README.md` — project metadata and contributor guidance

---

## Build & run

### Windows (recommended: MSYS2 / MINGW64)

1. Open **MSYS2 MINGW64** shell or Git Bash (MINGW64 environment).
2. Create and enter build directory:

```bash
mkdir -p build && cd build
```

3. Configure using MinGW Makefiles (important on Windows):

```bash
cmake -G "MinGW Makefiles" ..
```

4. Build:

```bash
mingw32-make
```

5. Run:

```bash
./PhysicsEngine.exe
```

> If CMake chooses Visual Studio by mistake, remove the `build/` folder and re-run `cmake -G "MinGW Makefiles" ..`.

### Linux / macOS

```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -- -j$(nproc)
./PhysicsEngine
```

---

## Development tips

- Use out-of-source builds (`build/`) to keep the repo clean.
- Add `-DENABLE_TESTING=ON` in CMake (if tests are added) to enable unit tests.
- Consider adding `ccache` and `-j` parallel builds to speed iteration.

---

## Parallelism notes

- Loops that iterate over independent ranges use `#pragma omp parallel for`.
- Reductions must use `reduction(+:sum)` (or appropriate operator) to avoid data races.
- Use `#pragma omp parallel sections` for coarse-grained independent tasks.
- A simple gravity simulation example (point masses, O(N^2) pairwise) has been added: see `simulate_gravity_step` and the `Body` struct in `include/physics.hpp` for details.

  - The `main` executable supports CSV dumps for post-processing via `--csv out.csv`. CSV columns are `step,body,x,y,vx,vy` (sampled per-step or at a user-defined interval).
  - The simulator has CLI options: `--scenario {two-body,three-body,random}`, `--steps`, `--dt`, `--sample`, `--n` and `--seed` to produce more complex datasets for plotting or analysis.
  - Use the `scripts/plot_gravity.py` helper to create SVG visualizations. It supports `trajectory` (default) and `distance` plot types (`-t distance`) and accepts `--pairs` (e.g. `0-1,0-2` or `all`). This script has no runtime Python dependencies and writes SVG output by default.

---

## Troubleshooting

- "cmake: command not found" or "mingw32-make: command not found": ensure MINGW64's `bin` directory (e.g., `C:\msys64\mingw64\bin`) is on `PATH`.
- Link or compile errors referencing OpenMP: ensure you are building with a GCC toolchain that supports OpenMP (MinGW-w64 GCC versions generally do).
- If a previously generated build files target the wrong generator, delete `build/` and reconfigure with the correct `-G` option.

---

## CI suggestion

Add a GitHub Actions workflow that:

- Builds on Windows (MSYS2/MINGW64) and Ubuntu
- Runs any unit tests
- Optionally runs static analysis (clang-tidy, cpplint) and code format checks

---

## How to contribute

- Fork -> branch -> implement -> open PR
- Keep changes focused and add tests for behavior changes
- Follow consistent commit message style (short summary, blank line, body)

---

## For AI agents (operational guidance) 🤖

Purpose: help contributors by automating edits, refactors, tests, CI setup, and documentation while preserving build reproducibility.

Agent rules and constraints:

1. Work only on source, build configuration, scripts, and documentation. Do NOT modify files under `build/` or other generated artifacts.
2. Before changing CMake or build scripts, ensure the change is cross-platform (Windows & Linux) or document platform-specific differences clearly.
3. For any non-trivial change, create or update unit tests and verify a full build on both Windows (MinGW) and Linux (or simulate using GitHub Actions). Report failures with exact commands and logs.
4. When adding dependencies, update `README.md` with installation steps and add CI steps to install those deps.
5. Use the root `build` (POSIX) helper when a rebuild is required; `scripts/` contains legacy wrappers for backwards compatibility; avoid in-place modifications of `build/` content.
6. If modifying any public API (`include/*.hpp`), add a short migration note in the PR description and update any affected tests.
7. Add `CMakePresets.json` when introducing standard build profiles (Debug/Release) and document usage in `README.md`.
8. For performance-related changes, provide micro-benchmarks or before/after results and include reproducible commands to run them.
9. Keep PRs small and focused; include test artifacts (input/expected) when relevant.

Agent reporting format for actions (use this in PRs or automated messages):

- Summary: one-line change summary
- Files touched: bullet list
- Build: commands run and results (Success/Failure + key logs)
- Tests: pass/fail and failing tests
- Notes: follow-up items or recommended next steps

---

## Known issues

- Project currently has a small test surface; adding unit tests is encouraged.
- No CI is configured by default (add workflows per CI suggestion above).

---
