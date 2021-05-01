# Generate solution
cmake -B build -S .

if ($?) {
    # Build
    cmake --build build --config $args[0]

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
