# PowerShell Script to Launch Interactive GDB Debug Session
$ErrorActionPreference = "Stop"

Write-Host "=========================================================" -ForegroundColor Cyan
Write-Host "  Starting Debug Session for DevEBox STM32F407VGT6" -ForegroundColor Cyan
Write-Host "=========================================================" -ForegroundColor Cyan

# 1. Verify/Build ELF binary
$elf = Join-Path $PSScriptRoot "build\STM32_BLACK_BOX.elf"
if (-not (Test-Path $elf)) {
    Write-Host "[*] ELF not found. Building project first..." -ForegroundColor Yellow
    & (Join-Path $PSScriptRoot "build.ps1")
}

# 2. Check if OpenOCD is already running on port 3333
$listening = Get-NetTCPConnection -LocalPort 3333 -ErrorAction SilentlyContinue | Where-Object { $_.State -eq "Listen" }
if (-not $listening) {
    Write-Host "[*] Launching OpenOCD GDB server in background on port 3333..." -ForegroundColor Yellow
    Start-Process powershell -ArgumentList "-ExecutionPolicy Bypass -NoExit -File `"$PSScriptRoot\openocd_server.ps1`"" -WindowStyle Minimized
    Start-Sleep -Seconds 2
} else {
    Write-Host "[*] OpenOCD is already running on port 3333." -ForegroundColor Green
}

# 3. Locate GDB client
$gdbExe = "C:\ST\STM32CubeIDE_2.2.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.14.3.rel1.win32_1.0.100.202602081740\tools\bin\arm-none-eabi-gdb.exe"
if (-not (Test-Path $gdbExe)) {
    $found = Get-ChildItem -Path "C:\ST" -Recurse -Filter "arm-none-eabi-gdb.exe" -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($found) { $gdbExe = $found.FullName }
}

Write-Host "[*] Connecting GDB to target at localhost:3333..." -ForegroundColor Green
Write-Host "[*] Target halted at Reset_Handler / main. Type 'c' to continue, 'n' to step, 'q' to exit." -ForegroundColor Cyan

& $gdbExe -q $elf -ex "target remote localhost:3333" -ex "monitor reset halt" -ex "load" -ex "b main" -ex "c"
