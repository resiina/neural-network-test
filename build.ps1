# Generate solution
cmake -B build -S .

if ($?) {
    # Build
    cmake --build build --config Release

    if($?) {
        # Build succeeded, run
        "SUCCESS"
    }
    else {
        throw "Project build failed"
    }
}
else {
    throw "Project generation failed"
}
