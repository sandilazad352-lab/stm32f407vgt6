@echo off
setlocal enabledelayedexpansion

echo =========================================================
echo   Flashing STM32_BLACK_BOX via ST-LINK (SWD)
echo =========================================================

:: Check if binary exists, build if not
if not exist "build\STM32_BLACK_BOX.hex" (
    echo [*] Hex file not found. Running build first...
    call build.bat
    if %errorlevel% neq 0 exit /b %errorlevel%
)

:: Locate STM32_Programmer_CLI
set "PROGRAMMER_CLI="
where STM32_Programmer_CLI >nul 2>nul
if %errorlevel% equ 0 (
    set "PROGRAMMER_CLI=STM32_Programmer_CLI"
) else (
    for /d %%A in ("C:\ST\STM32CubeIDE*") do (
        for /d %%B in ("%%A\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer*") do (
            if exist "%%B\tools\bin\STM32_Programmer_CLI.exe" (
                set "PROGRAMMER_CLI=%%B\tools\bin\STM32_Programmer_CLI.exe"
            )
        )
    )
)

if "!PROGRAMMER_CLI!"=="" (
    if exist "C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe" (
        set "PROGRAMMER_CLI=C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe"
        echo [*] Using ST-LINK Utility CLI: !PROGRAMMER_CLI!
        "!PROGRAMMER_CLI!" -c SWD -P "build\STM32_BLACK_BOX.hex" 0x08000000 -V -Rst
        exit /b %errorlevel%
    ) else (
        echo [!] STM32_Programmer_CLI or ST-LINK_CLI not found!
        exit /b 1
    )
)

echo [*] Using Programmer: !PROGRAMMER_CLI!
echo [*] Programming and verifying Flash...
"!PROGRAMMER_CLI!" -c port=SWD -w "build\STM32_BLACK_BOX.hex" -v -rst

if %errorlevel% equ 0 (
    echo =========================================================
    echo   Flashing Succeeded! MCU reset and running.
    echo =========================================================
) else (
    echo [!] Flashing failed! Ensure ST-Link is connected and not locked by another tool.
)
