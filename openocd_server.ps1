# OpenOCD GDB Server Launcher for DevEBox STM32F407VGT6 via ST-Link
$ErrorActionPreference = "Stop"

# Check if port 3333 is already in use
$listening = Get-NetTCPConnection -LocalPort 3333 -ErrorAction SilentlyContinue | Where-Object { $_.State -eq "Listen" }
if ($listening) {
    Write-Host "[*] OpenOCD already running on port 3333 (Listening on port 3333 for gdb connections)." -ForegroundColor Green
    # Keep process active so VS Code task doesn't immediately exit
    while ($true) { Start-Sleep -Seconds 10 }
    exit 0
}

# Locate OpenOCD executable and scripts
$openocdExe = "C:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.openocd.win32_2.4.500.202604080855\tools\bin\openocd.exe"
$scriptsDir = "C:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.debug.openocd_2.3.400.202606220929\resources\openocd\st_scripts"

if (-not (Test-Path $openocdExe) -or -not (Test-Path $scriptsDir)) {
    # Fallback search in C:\ST
    $foundExe = Get-ChildItem -Path "C:\ST" -Recurse -Filter "openocd.exe" -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($foundExe) { $openocdExe = $foundExe.FullName }
    $foundScripts = Get-ChildItem -Path "C:\ST" -Recurse -Filter "st_scripts" -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($foundScripts) { $scriptsDir = $foundScripts.FullName }
}

Write-Host "=========================================================" -ForegroundColor Cyan
Write-Host "  Starting OpenOCD GDB Server for STM32F407 (Port 3333)" -ForegroundColor Cyan
Write-Host "=========================================================" -ForegroundColor Cyan

& $openocdExe -s $scriptsDir -f interface/stlink-dap.cfg -f target/stm32f4x.cfg
