#!/bin/bash
set -e

echo "================================"
echo "Smart Fridge DEVS Simulation"
echo "================================"

cd "$(dirname "$0")/../cadmium_generated"
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
./smart_fridge > ../simulation_results/simulation_result.txt

echo "Simulation completed successfully."
echo "Output saved to cadmium_generated/simulation_results/simulation_result.txt"
