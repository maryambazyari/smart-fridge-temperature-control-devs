#!/usr/bin/env bash
set -e

echo "=========================================="
echo "Smart Fridge DEVS Simulation (Cadmium)"
echo "=========================================="

cd "$(dirname "$0")/.."

if [ ! -d "cadmium_generated" ]; then
  echo "ERROR: cadmium_generated folder not found."
  exit 1
fi

mkdir -p cadmium_generated/simulation_results
mkdir -p cadmium_generated/build

cd cadmium_generated/build

/c/msys64/ucrt64/bin/cmake.exe -G "MinGW Makefiles" ..
mingw32-make -j1

./smart_fridge.exe > ../simulation_results/simulation_result.txt

echo "Simulation completed successfully."
echo "Output saved to cadmium_generated/simulation_results/simulation_result.txt"