@echo off
echo ==========================================
echo Smart Fridge DEVS Simulation (Cadmium)
echo ==========================================

REM Go to project root (one level up from scripts)
cd /d %~dp0\..

echo [1/4] Checking project folders...
if not exist cadmium_generated (
    echo ERROR: cadmium_generated folder not found.
    pause
    exit /b 1
)

if not exist cadmium_generated\simulation_results (
    mkdir cadmium_generated\simulation_results
)

echo [2/4] Entering build folder...
cd cadmium_generated

if exist build (
    echo Build folder exists.
) else (
    mkdir build
)

cd build

echo [3/4] Configuring with CMake...
"C:\msys64\ucrt64\bin\cmake.exe" -G "MinGW Makefiles" ..
if errorlevel 1 (
    echo ERROR: CMake configuration failed.
    pause
    exit /b 1
)

echo [4/4] Building project...
mingw32-make -j1
if errorlevel 1 (
    echo ERROR: Build failed.
    pause
    exit /b 1
)

echo Running simulation...
smart_fridge.exe > ..\simulation_results\simulation_result.txt
if errorlevel 1 (
    echo ERROR: Simulation execution failed.
    pause
    exit /b 1
)

echo ------------------------------------------
echo Simulation completed successfully.
echo Output saved to:
echo cadmium_generated\simulation_results\simulation_result.txt
echo ------------------------------------------
pause