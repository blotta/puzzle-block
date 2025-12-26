# Set the target directory where you want to unzip SDL2
$baseDir = "dependencies"

$packages = @(
    @{
        Name = "SDL2"
        Url  = "https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-devel-2.32.10-VC.zip"
        ZipFolder = "SDL2-2.32.10"
    },
    @{
        Name = "SDL2_image"
        Url  = "https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.8/SDL2_image-devel-2.8.8-VC.zip"
        ZipFolder = "SDL2_image-2.8.8"
    },
    @{
        Name = "SDL2_ttf"
        Url  = "https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.24.0/SDL2_ttf-devel-2.24.0-VC.zip"
        ZipFolder = "SDL2_ttf-2.24.0"
    },
    @{
        Name = "SDL2_mixer"
        Url  = "https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.1/SDL2_mixer-devel-2.8.1-VC.zip"
        ZipFolder = "SDL2_mixer-2.8.1"
    }
)

# ===== Ensure base directory exists =====
New-Item -ItemType Directory -Force -Path $baseDir | Out-Null

foreach ($pkg in $packages) {
    Write-Host "==== Setting up $($pkg.Name) ===="

    $tempZip = "$env:TEMP\$($pkg.Name).zip"
    $tempExtract = "$env:TEMP\$($pkg.Name)_extract"
    $finalDir = Join-Path $baseDir $pkg.Name

    if (Test-Path $finalDir) {
        Write-Host "Skipping. To Re-download, delete the $finalDir directory first."
        continue
    }

    # Download
    Write-Host "Downloading $($pkg.Url)"
    Invoke-WebRequest -Uri $pkg.Url -OutFile $tempZip

    # Extract ZIP
    if (Test-Path $tempExtract) {
        Remove-Item $tempExtract -Recurse -Force
    }
    Expand-Archive -Path $tempZip -DestinationPath $tempExtract

    # Move & rename folder
    $sourcePath = Join-Path $tempExtract $pkg.ZipFolder

    if (-not (Test-Path $sourcePath)) {
        Write-Error "Folder $($pkg.ZipFolder) not found!"
        continue
    }

    # if (Test-Path $finalDir) {
    #     Remove-Item $finalDir -Recurse -Force
    # }

    Move-Item -Path $sourcePath -Destination $finalDir

    # Cleanup
    Remove-Item $tempZip -Force
    Remove-Item $tempExtract -Recurse -Force

    Write-Host "$($pkg.Name) installed to $finalDir`n"
}

Write-Host "All SDL libraries installed successfully!"
