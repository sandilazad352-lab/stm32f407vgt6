cmake_minimum_required(VERSION 3.20)

# Default to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build: Debug, Release" FORCE)
endif()

project(STM32_BLACK_BOX C CXX ASM)

# Generate compile_commands.json for IDE IntelliSense and code navigation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Toolchain check
if(NOT CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compilation toolchain not provided, defaulting to cmake/gcc-arm-none-eabi.cmake")
    include(cmake/gcc-arm-none-eabi.cmake)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Optimization and debug flags
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -g0 -DNDEBUG")

# Pure CMSIS Core & Startup Sources
set(CORE_SOURCES
    Core/Src/main.c
    Core/Src/clock.c
    Core/Src/gpio.c
    Core/Src/stm32f4xx_it.c
    Core/Src/system_stm32f4xx.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c
    Core/Startup/startup_stm32f407xx.s
)

# FreeRTOS Kernel Sources
set(FREERTOS_SOURCES
    Middleware/FreeRTOS/Source/tasks.c
    Middleware/FreeRTOS/Source/queue.c
    Middleware/FreeRTOS/Source/list.c
    Middleware/FreeRTOS/Source/timers.c
    Middleware/FreeRTOS/Source/event_groups.c
    Middleware/FreeRTOS/Source/stream_buffer.c
    Middleware/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    Middleware/FreeRTOS/Source/portable/MemMang/heap_4.c
)

# Executable target
add_executable(${PROJECT_NAME}.elf
    ${CORE_SOURCES}
    ${FREERTOS_SOURCES}
)

# Include Directories
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    Core/Inc
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Middleware/FreeRTOS/Source/include
    Middleware/FreeRTOS/Source/portable/GCC/ARM_CM4F
)

# Compiler definitions
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE
    STM32F407xx
    HSE_VALUE=8000000UL
)

# Linker script & options
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/STM32F407VGTX_FLASH.ld)
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -T${LINKER_SCRIPT}
)

# Post-build commands: generate HEX, BIN, and display size
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} --format=berkeley $<TARGET_FILE:${PROJECT_NAME}.elf>
    COMMENT "Building ${PROJECT_NAME}.hex and ${PROJECT_NAME}.bin..."
)

# ST-Link flash target
find_program(STM32_PROGRAMMER_CLI "STM32_Programmer_CLI"
    HINTS "C:/ST/STM32CubeIDE_2.2.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.win32_2.2.500.202603051304/tools/bin"
)

if(STM32_PROGRAMMER_CLI)
    add_custom_target(flash
        COMMAND ${STM32_PROGRAMMER_CLI} -c port=SWD -w ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex -v -rst
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Flashing ${PROJECT_NAME}.hex via ST-LINK (SWD)..."
    )
endif()
