# Neural Network Test App

## How to build

Requirements:
- Windows
- PowerShell
- CMake
- Compiler

Run
```
cmake -B build -S .
```
in repository root directory to generate project files.

Run
```
cmake --build build --config Release
```
in repository root directory to build the executable.

----------------------

## How to run

When running the first time, extract the MNIST PNG data from the submodule by executing
```
./extract_mnist_png
```

To run the application executable, run
```
./build/Release/NeuralNetworkTestApp.exe
```
in repository root directory.