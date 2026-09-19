# CMake toolchain file for arm-none-eabi
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Try finding compiler if not in PATH
if(NOT CMAKE_C_COMPILER)
    find_program(ARM_GCC_EXE "arm-none-eabi-gcc"
        HINTS
        "C:/ST/STM32CubeIDE_2.2.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.14.3.rel1.win32_1.0.100.202602081740/tools/bin"
    )
    if(ARM_GCC_EXE)
        set(CMAKE_C_COMPILER ${ARM_GCC_EXE} CACHE FILEPATH "C Compiler")
        get_filename_component(ARM_TOOLCHAIN_BIN_DIR ${ARM_GCC_EXE} DIRECTORY)
        set(CMAKE_CXX_COMPILER "${ARM_TOOLCHAIN_BIN_DIR}/arm-none-eabi-g++.exe" CACHE FILEPATH "CXX Compiler")
        set(CMAKE_ASM_COMPILER "${ARM_TOOLCHAIN_BIN_DIR}/arm-none-eabi-gcc.exe" CACHE FILEPATH "ASM Compiler")
        set(CMAKE_OBJCOPY "${ARM_TOOLCHAIN_BIN_DIR}/arm-none-eabi-objcopy.exe" CACHE FILEPATH "Objcopy")
        set(CMAKE_OBJDUMP "${ARM_TOOLCHAIN_BIN_DIR}/arm-none-eabi-objdump.exe" CACHE FILEPATH "Objdump")
        set(CMAKE_SIZE "${ARM_TOOLCHAIN_BIN_DIR}/arm-none-eabi-size.exe" CACHE FILEPATH "Size")
    else()
        set(CMAKE_C_COMPILER arm-none-eabi-gcc)
        set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
        set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
        set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
        set(CMAKE_OBJDUMP arm-none-eabi-objdump)
        set(CMAKE_SIZE arm-none-eabi-size)
    endif()
endif()

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Processor architecture flags for Cortex-M4 with FPU (hard)
set(ARM_OPTIONS -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb)

add_compile_options(
    ${ARM_OPTIONS}
    -Wall
    -Wextra
    -fdata-sections
    -ffunction-sections
)

add_compile_definitions(
    STM32F407xx
    HSE_VALUE=8000000UL
)

add_link_options(
    ${ARM_OPTIONS}
    -Wl,--gc-sections
    --specs=nano.specs
    --specs=nosys.specs
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map,--cref
)
