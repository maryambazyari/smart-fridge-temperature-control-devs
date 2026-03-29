#!/bin/bash
set -e

echo "================================"
echo "Smart Fridge DEVS Simulation"
echo "================================"

cd "$(dirname "$0")/.."

if [ ! -d "cadmium_generated" ]; then
    echo "ERROR: cadmium_generated folder not found."
    exit 1
fi

if [ ! -f "external/DESTimes/include/NDTime.hpp" ]; then
    echo "ERROR: external/DESTimes/include/NDTime.hpp not found."
    exit 1
fi

if [ ! -d "external/cadmium/include" ]; then
    echo "ERROR: external/cadmium/include not found."
    exit 1
fi

if [ ! -f "external/boost/boost/algorithm/string.hpp" ]; then
    echo "Preparing Boost headers..."
    cd external/boost
    chmod +x bootstrap.sh
    ./bootstrap.sh
    ./b2 headers
    cd ../..
fi

mkdir -p cadmium_generated/simulation_results
rm -rf cadmium_generated/build
mkdir -p cadmium_generated/build

cd cadmium_generated/build
cmake ..
cmake --build .
./smart_fridge > ../simulation_results/simulation_result.txt

echo "Simulation completed successfully."
echo "Output saved to cadmium_generated/simulation_results/simulation_result.txt"
