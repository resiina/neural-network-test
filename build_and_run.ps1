$buildResult = ./build

if ($buildResult -eq "SUCCESS") {
    # Build succeeded, run
    ./run
}
else {
    # Build failed
    throw "Build failed, did not run application"
}