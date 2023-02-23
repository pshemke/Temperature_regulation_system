################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/Src/__template.c \
../Components/Src/bmp2.c \
../Components/Src/bmp2_config.c \
../Components/Src/functions.c 

OBJS += \
./Components/Src/__template.o \
./Components/Src/bmp2.o \
./Components/Src/bmp2_config.o \
./Components/Src/functions.o 

C_DEPS += \
./Components/Src/__template.d \
./Components/Src/bmp2.d \
./Components/Src/bmp2_config.d \
./Components/Src/functions.d 


# Each subdirectory must supply rules for building sources it contributes
Components/Src/%.o Components/Src/%.su: ../Components/Src/%.c Components/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/Desktop/Temperature_regulation_system/Components/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components-2f-Src

clean-Components-2f-Src:
	-$(RM) ./Components/Src/__template.d ./Components/Src/__template.o ./Components/Src/__template.su ./Components/Src/bmp2.d ./Components/Src/bmp2.o ./Components/Src/bmp2.su ./Components/Src/bmp2_config.d ./Components/Src/bmp2_config.o ./Components/Src/bmp2_config.su ./Components/Src/functions.d ./Components/Src/functions.o ./Components/Src/functions.su

.PHONY: clean-Components-2f-Src

