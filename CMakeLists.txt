cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE cmake/gcc-arm-none-eabi.cmake)
# Allow assembler
project("PiTer_CAN_Sniffer" C CXX ASM)

include(cmake/compiler_flags.cmake)
enable_language(ASM)

set(FREERTOS_CONFIG_FILE_DIRECTORY "${PROJECT_SOURCE_DIR}/Core/Inc")
set(FREERTOS_HEAP "4")
set(FREERTOS_PORT "GCC_ARM_CM4F")

add_subdirectory(FreeRTOS/Source freertos_kernel)




set(INCLUDE_DIRS_CMSIS

    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
    Core/Src/utils
)

set(INCLUDE_DIRS_APP

    Core/Inc
    Core/Src
    Core/Src/OSAL
    Core/Src/CAN
    Core/Src/DebugModule/stillAliveSignal
    Core/Src/PcCommunication
    Core/Src/HwDrivers/RCC_Clock
    Core/Src/HwDrivers/USART2
    Core/Src/HwDrivers/CAN1
    Core/Src/PcCommunication/Transmiter
    Core/Src/PcCommunication/Receiver
)


set(GLOBAL_DEFINES
    -DDEBUG
    -DSTM32F446xx
)


set(C_SRCS_SYSTEM

    Core/Src/utils/stm32f4xx_it.c
    Core/Src/utils/syscalls.c
    Core/Src/utils/sysmem.c
    Core/Src/utils/system_stm32f4xx.c
    Core/Startup/startup_stm32f446retx.s
)

set(C_SRCS_APP

    Core/Src/main.c
    Core/Src/OSAL/OSAL.c
    Core/Src/DebugModule/stillAliveSignal/IstillAliveTimer.c
    Core/Src/DebugModule/stillAliveSignal/led_blink.c
    Core/Src/DebugModule/stillAliveSignal/stillAliveSignal.c
    Core/Src/DebugModule/stillAliveSignal/timer2_irq.c
    Core/Src/PcCommunication/PCCommunication.c
    Core/Src/PcCommunication/IPCCommunication_LowLevel.c
    Core/Src/PcCommunication/Transmiter/PCCommunication_Transmiter.c
    Core/Src/PcCommunication/Receiver/PCCommunication_Receiver.c
    Core/Src/PcCommunication/USART2_driver.c
    Core/Src/CAN/ICAN_LowLevel.c
    Core/Src/HwDrivers/RCC_Clock/RCC_clock.c
    Core/Src/HwDrivers/USART2/USART2.c
    Core/Src/HwDrivers/CAN1/CAN1.c
)


add_executable(${PROJECT_NAME} ${C_SRCS_SYSTEM} ${C_SRCS_APP})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${INCLUDE_DIRS_CMSIS} ${INCLUDE_DIRS_APP})
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE ${GLOBAL_DEFINES})

target_link_libraries(${PROJECT_NAME} freertos_kernel)


add_custom_command(
    TARGET ${PROJECT_NAME} 
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
)

#generate hex file
add_custom_command(
    TARGET ${PROJECT_NAME} 
    COMMAND ${CMAKE_OBJCOPY} -O ihex
    $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
)

#generate bin file
add_custom_command(
    TARGET ${PROJECT_NAME} 
    COMMAND ${CMAKE_OBJCOPY} -O binary
    $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
)

#generate extended listing
add_custom_command(
    TARGET ${PROJECT_NAME} 
    COMMAND ${CMAKE_OBJDUMP} -h -S
        $<TARGET_FILE:${PROJECT_NAME}> > ${PROJECT_NAME}.lss
)

#generate memory dump
add_custom_command(
    TARGET ${PROJECT_NAME} 
    COMMAND ${CMAKE_OBJDUMP} -x --syms
        $<TARGET_FILE:${PROJECT_NAME}> > ${PROJECT_NAME}.dmp
)





