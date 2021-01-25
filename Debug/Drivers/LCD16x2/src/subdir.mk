################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LCD16x2/src/LCD16x2.c 

OBJS += \
./Drivers/LCD16x2/src/LCD16x2.o 

C_DEPS += \
./Drivers/LCD16x2/src/LCD16x2.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LCD16x2/src/LCD16x2.o: ../Drivers/LCD16x2/src/LCD16x2.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/projects/embbeded/LCD16x2_DRIVERS/Drivers/LCD16x2/inc" -I"C:/projects/embbeded/LCD16x2_DRIVERS/utilities" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/LCD16x2/src/LCD16x2.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

