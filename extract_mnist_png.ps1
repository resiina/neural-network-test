$pathTo7zip = "C:\Program Files\7-Zip\7z.exe"

if (!(Test-Path -Path $pathTo7zip))
{
    $pathTo7zip = "D:\Program Files\7-Zip\7z.exe"
} 

# Extract the package
& $pathTo7zip x "external/mnist_png/mnist_png.tar.gz"
& $pathTo7zip x "mnist_png.tar"

# Remove redundant intermediate package
Remove-Item "mnist_png.tar" -Force