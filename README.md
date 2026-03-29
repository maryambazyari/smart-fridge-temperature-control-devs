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
- `cadmium_generated/simulation_results/` : simulation output log
- `devs_graph/` : DEVS-Graph JSON model files
- `docs/` : final report and DEVS model form
- `screenshots/` : screenshots of DEVS-Graph models
- `scripts/` : helper script to build and run the simulation
- `external/` : local copy of required dependencies
- `README.md` : project instructions and notes

---

## Core Files

Inside `cadmium_generated/`:
- `TempSensor.hpp`
- `Controller.hpp`
- `Compressor.hpp`
- `message_types.hpp`
- `main.cpp`
- `CMakeLists.txt`

---

## Clone and Run

```bash
git clone git@github.com:maryambazyari/smart-fridge-temperature-control-devs.git
cd smart-fridge-temperature-control-devs
chmod +x scripts/run_simulation.sh
./scripts/run_simulation.sh
```

The script automatically:
- checks the required folders
- prepares Boost headers if needed
- builds the project
- runs the simulation
- saves the output log

---

## Output

The simulation output is saved in:

`cadmium_generated/simulation_results/simulation_result.txt`

The output log contains atomic model state traces for:
- TempSensor (TS)
- Controller (CTRL)
- Compressor (COMP)

---

## Input / Parameters

This baseline smart fridge simulation does not require external input files.

The initial temperature, hysteresis thresholds, and warming/cooling rates are defined in:
- `cadmium_generated/main.cpp`
- the atomic model header files

---

## Notes

This repository contains the generated Cadmium implementation, DEVS-Graph model files, screenshots, simulation output, and final documentation for the Smart Fridge Temperature Control DEVS assignment.

