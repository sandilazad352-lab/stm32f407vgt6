# PowerShell Build Script for STM32_BLACK_BOX
$ErrorActionPreference = "Stop"

Write-Host "=========================================================" -ForegroundColor Cyan
Write-Host "  Building STM32_BLACK_BOX (DevEBox STM32F407VGT6)" -ForegroundColor Cyan
Write-Host "=========================================================" -ForegroundColor Cyan

# Locate STM32CubeIDE tools if not on PATH
function Add-ToolPathIfPresent ($pattern, $exe) {
    if (-not (Get-Command $exe -ErrorAction SilentlyContinue)) {
        $found = Get-ChildItem -Path "C:\ST" -Filter $pattern -Recurse -Depth 5 -ErrorAction SilentlyContinue |
                 Where-Object { Test-Path (Join-Path $_.FullName "tools\bin\$exe") } |
                 Select-Object -First 1
        if ($found) {
            $binPath = Join-Path $found.FullName "tools\bin"
            $env:PATH = "$binPath;$env:PATH"
            Write-Host "[*] Added to PATH: $binPath" -ForegroundColor Green
        }
    }
}

Add-ToolPathIfPresent "*gnu-tools-for-stm32*" "arm-none-eabi-gcc.exe"
Add-ToolPathIfPresent "*cmake*" "cmake.exe"
Add-ToolPathIfPresent "*ninja*" "ninja.exe"

# Verify tools
$gcc = Get-Command "arm-none-eabi-gcc.exe" -ErrorAction SilentlyContinue
$cmake = Get-Command "cmake.exe" -ErrorAction SilentlyContinue
$ninja = Get-Command "ninja.exe" -ErrorAction SilentlyContinue

if (-not $gcc -or -not $cmake -or -not $ninja) {
    Write-Error "Required tools (arm-none-eabi-gcc, cmake, ninja) not found! Ensure STM32CubeIDE is installed."
    exit 1
}

# Ensure build directory exists
$buildDir = Join-Path $PSScriptRoot "build"
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

Push-Location $buildDir
try {
    Write-Host "[*] Configuring with CMake and Ninja..." -ForegroundColor Yellow
    & cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE="../cmake/gcc-arm-none-eabi.cmake" -DCMAKE_BUILD_TYPE=Debug ..
    if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed!" }
    
    Write-Host "[*] Building targets..." -ForegroundColor Yellow
    & ninja
    if ($LASTEXITCODE -ne 0) { throw "Ninja build failed!" }

    Write-Host "=========================================================" -ForegroundColor Green
    Write-Host "  Build Succeeded!" -ForegroundColor Green
    Write-Host "  Binaries in: $buildDir" -ForegroundColor Green
    Write-Host "=========================================================" -ForegroundColor Green
}
finally {
    Pop-Location
}
