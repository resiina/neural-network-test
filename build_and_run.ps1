# Generate solution
cmake -B build -S .

# Build
cmake --build build --config Release

# Run
Write-Host "Running application, output:"
Write-Host ""
Write-Host ""
./build/Release/NeuralNetworkTestApp.exe
Write-Host ""
Write-Host ""