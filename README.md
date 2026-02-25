# Smart Fridge Temperature Control DEVS Model

## Project Overview
This repository contains a DEVS-based smart fridge temperature control system implemented in C++ using the Cadmium simulator.

The model includes:
- **TempSensor (TS)**: periodically outputs fridge temperature
- **Controller (CTRL)**: applies hysteresis control and generates ON/OFF compressor commands
- **Compressor (COMP)**: receives commands and returns cooling status feedback

The system is organized hierarchically and includes:
- atomic model implementations
- DEVS-Graph artifacts (JSON exports)
- screenshots of DEVS-Graph diagrams
- simulation execution results
- final report document

---

## Repository Structure

- `cadmium_generated/` : Cadmium C++ source files and CMake build configuration
- `cadmium_generated/simulation_results/` : simulation output log(s)
- `devs_graph/` : DEVS-Graph JSON model files
- `docs/` : final report document
- `screenshots/` : DEVS-Graph screenshots (and optional build/run screenshots)
- `scripts/` : helper scripts to build and run the simulation
- `README.md` : project instructions and notes

---

## Core Files (Cadmium Implementation)

Inside `cadmium_generated/`:
- `TempSensor.hpp`
- `Controller.hpp`
- `Compressor.hpp`
- `message_types.hpp`
- `main.cpp`
- `CMakeLists.txt`

---

## Build and Run (MSYS2 UCRT64)

### Manual commands (used in development)
```bash
cd cadmium_generated
rm -rf build
mkdir build
cd build
/c/msys64/ucrt64/bin/cmake.exe -G "MinGW Makefiles" ..
mingw32-make -j1
./smart_fridge.exe


## Input / Parameters
This baseline smart fridge simulation does not require external input files.
The initial temperature, hysteresis thresholds, and warming/cooling rates are defined in `cadmium_generated/main.cpp` and the model header files.

## Output
A sample simulation execution log is included at:

`cadmium_generated/simulation_results/simulation_result.txt`

The output log contains atomic model state traces for:
- TempSensor (TS)
- Controller (CTRL)
- Compressor (COMP)

## Run Scripts
Scripts are included for reproducibility:
- `scripts/run_simulation.bat` (Windows)
- `scripts/run_simulation.sh` (Git Bash / MSYS2 / Unix-like shell)
