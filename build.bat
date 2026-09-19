@echo off
setlocal enabledelayedexpansion

echo =========================================================
echo   Building STM32_BLACK_BOX (DevEBox STM32F407VGT6)
echo =========================================================

:: Search for STM32CubeIDE bundled toolchain if not on PATH
where arm-none-eabi-gcc >nul 2>nul
if %errorlevel% neq 0 (
    for /d %%A in ("C:\ST\STM32CubeIDE*") do (
        for /d %%B in ("%%A\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32*") do (
            if exist "%%B\tools\bin\arm-none-eabi-gcc.exe" (
                set "PATH=%%B\tools\bin;!PATH!"
                echo [*] Found GCC: %%B\tools\bin
            )
        )
    )
)

where cmake >nul 2>nul
if %errorlevel% neq 0 (
    for /d %%A in ("C:\ST\STM32CubeIDE*") do (
        for /d %%B in ("%%A\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.cmake*") do (
            if exist "%%B\tools\bin\cmake.exe" (
                set "PATH=%%B\tools\bin;!PATH!"
                echo [*] Found CMake: %%B\tools\bin
            )
        )
    )
)

where ninja >nul 2>nul
if %errorlevel% neq 0 (
    for /d %%A in ("C:\ST\STM32CubeIDE*") do (
        for /d %%B in ("%%A\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.ninja*") do (
            if exist "%%B\tools\bin\ninja.exe" (
                set "PATH=%%B\tools\bin;!PATH!"
                echo [*] Found Ninja: %%B\tools\bin
            )
        )
    )
)

:: Create build folder
if not exist build mkdir build
cd build

echo [*] Configuring with CMake and Ninja...
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=Debug ..
if %errorlevel% neq 0 (
    echo [!] CMake configuration failed!
    cd ..
    exit /b %errorlevel%
)

echo [*] Building project...
ninja
if %errorlevel% neq 0 (
    echo [!] Build failed!
    cd ..
    exit /b %errorlevel%
)

cd ..
echo =========================================================
echo   Build Successful!
echo   Output files in: build\
echo   - STM32_BLACK_BOX.elf
echo   - STM32_BLACK_BOX.hex
echo   - STM32_BLACK_BOX.bin
echo =========================================================
