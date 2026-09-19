# STM32_BLACK_BOX (DevEBox STM32F407VGT6) - Pure CMSIS + FreeRTOS

Ultra-lightweight embedded project scaffold for the **DevEBox STM32F4XX-M** development board based on the **STM32F407VGT6** MCU. Built with **pure CMSIS** and **FreeRTOS Kernel** (no HAL overhead), configured for **CMake + Ninja** and **STM32CubeIDE**.

---

## Hardware & OS Specifications

| Peripheral / Feature | Configuration | Notes |
| :--- | :--- | :--- |
| **MCU** | STM32F407VGT6 | Cortex-M4F @ 168 MHz (FPU hardware enabled) |
| **Internal Flash** | 1 MB | `0x08000000`–`0x080FFFFF` |
| **RTOS** | FreeRTOS Kernel | Preemptive scheduler, 1 ms tick, 48 KB heap (`heap_4.c`) |
| **User LED (D2)** | `PA1` (Active LOW) | Driven by `vHeartbeatTask` |
| **User Button (K1)** | `PA0` (Active HIGH) | Monitored by `vButtonTask` to adjust blink rate |
| **System Crystal (HSE)**| 8.000 MHz | PLL configured to 168 MHz SYSCLK |

---

## Directory Structure

```
STM32_BLACK_BOX/
├── CMakeLists.txt                 # Top-level CMake configuration
├── cmake/
│   └── gcc-arm-none-eabi.cmake    # Toolchain file for cross-compilation
├── STM32F407VGTX_FLASH.ld         # 1MB Flash linker script
├── STM32F407VGTX_RAM.ld           # RAM execution linker script
├── .project                       # Eclipse / STM32CubeIDE project descriptor
├── .cproject                      # Eclipse / STM32CubeIDE compiler/linker settings
├── .settings/                     # STM32CubeIDE preferences
├── Core/
│   ├── Inc/
│   │   ├── main.h                 # Application header
│   │   ├── board_defs.h           # Direct CMSIS register macros for pins
│   │   ├── FreeRTOSConfig.h       # FreeRTOS Kernel configuration (Cortex-M4F)
│   │   └── stm32f4xx_it.h         # Interrupt handlers
│   ├── Src/
│   │   ├── main.c                 # FreeRTOS tasks & CMSIS 168MHz clock
│   │   ├── stm32f4xx_it.c         # Exception handlers & stack overflow hook
│   │   ├── system_stm32f4xx.c     # CMSIS system initialization
│   │   ├── syscalls.c             # Newlib POSIX stubs
│   │   └── sysmem.c               # Dynamic memory allocator stub
│   └── Startup/
│       └── startup_stm32f407xx.s   # Reset handler and vector table
├── Drivers/
│   └── CMSIS/                     # ARM CMSIS Core & ST STM32F407 Device headers
├── Middleware/
│   └── FreeRTOS/
│       └── Source/                # FreeRTOS Kernel + ARM_CM4F GCC Port + heap_4
├── build.bat                      # 1-Click build script (Command Prompt)
├── build.ps1                      # 1-Click build script (PowerShell)
├── flash.bat                      # 1-Click flash script (Command Prompt)
├── flash.ps1                      # 1-Click flash script (PowerShell)
└── README.md                      # Documentation
```

---

## Building the Project

### Option A: 1-Click Script (Recommended)
The scripts automatically detect the `arm-none-eabi-gcc`, `cmake`, and `ninja` tools bundled in your STM32CubeIDE installation:

- **Command Prompt**:
  ```cmd
  build.bat
  ```
- **PowerShell**:
  ```powershell
  .\build.ps1
  ```

Output files will be generated in `build/`:
- `STM32_BLACK_BOX.elf`
- `STM32_BLACK_BOX.hex`
- `STM32_BLACK_BOX.bin`

### Option B: Manual CMake
```cmd
mkdir build
cd build
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=Debug ..
ninja
```

---

## Using with STM32CubeIDE

1. Open **STM32CubeIDE**.
2. Click **File** &rarr; **Import...**
3. Select **General** &rarr; **Existing Projects into Workspace** and click **Next**.
4. Set **Select root directory** to `c:\Users\Snowtex\STM32_BLACK_BOX`.
5. Check `STM32_BLACK_BOX` and click **Finish**.
6. Build with **Project** &rarr; **Build Project** (`Ctrl + B`) or run with an ST-Link debugger (`F11`).

---

## Flashing the Board

### 1-Click Flashing (ST-Link SWD)
To flash the compiled firmware directly via your connected ST-Link:

- **Command Prompt**:
  ```cmd
  flash.bat
  ```
- **PowerShell**:
  ```powershell
  .\flash.ps1
  ```
- **Ninja (via CMake)**:
  ```cmd
  ninja -C build flash
  ```

This automatically discovers `STM32_Programmer_CLI`, programs sector 0, verifies the flash, and resets the MCU to execute the firmware.

### Via DFU (USB Bootloader)
1. Set the onboard `BT0` pin to `3V3` (high).
2. Connect the board to your PC using a USB cable.
3. Open **STM32CubeProgrammer**, select **USB** interface, and click **Connect**.
4. Download `build/STM32_BLACK_BOX.bin` starting at address `0x08000000`.
5. Reset `BT0` pin to `GND` and press `RST` to run.
