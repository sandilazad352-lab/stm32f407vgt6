# PowerShell Flash Script for STM32_BLACK_BOX via ST-Link
$ErrorActionPreference = "Stop"

Write-Host "=========================================================" -ForegroundColor Cyan
Write-Host "  Flashing STM32_BLACK_BOX via ST-LINK (SWD)" -ForegroundColor Cyan
Write-Host "=========================================================" -ForegroundColor Cyan

$hexPath = Join-Path $PSScriptRoot "build\STM32_BLACK_BOX.hex"
if (-not (Test-Path $hexPath)) {
    Write-Host "[*] Hex file not found. Building project first..." -ForegroundColor Yellow
    & (Join-Path $PSScriptRoot "build.ps1")
}

# Locate STM32_Programmer_CLI
$cli = Get-Command "STM32_Programmer_CLI.exe" -ErrorAction SilentlyContinue
if (-not $cli) {
    $found = Get-ChildItem -Path "C:\ST" -Filter "*cubeprogrammer*" -Recurse -Depth 4 -ErrorAction SilentlyContinue |
             Where-Object { Test-Path (Join-Path $_.FullName "tools\bin\STM32_Programmer_CLI.exe") } |
             Select-Object -First 1
    if ($found) {
        $cliPath = Join-Path $found.FullName "tools\bin\STM32_Programmer_CLI.exe"
    } else {
        $legacy = "C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe"
        if (Test-Path $legacy) {
            $cliPath = $legacy
        }
    }
} else {
    $cliPath = $cli.Source
}

if (-not $cliPath -or -not (Test-Path $cliPath)) {
    Write-Error "STM32_Programmer_CLI or ST-LINK_CLI could not be located!"
    exit 1
}

Write-Host "[*] Using Programmer: $cliPath" -ForegroundColor Green
Write-Host "[*] Programming and verifying Flash..." -ForegroundColor Yellow

if ($cliPath -like "*ST-LINK_CLI.exe") {
    & $cliPath -c SWD -P $hexPath 0x08000000 -V -Rst
} else {
    & $cliPath -c port=SWD -w $hexPath -v -rst
}

if ($LASTEXITCODE -eq 0) {
    Write-Host "=========================================================" -ForegroundColor Green
    Write-Host "  Flashing Succeeded! MCU reset and running." -ForegroundColor Green
    Write-Host "=========================================================" -ForegroundColor Green
} else {
    Write-Error "Flashing failed! Ensure ST-Link is connected."
}
