################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/Src/__template.c \
../Components/Src/bh1750.c \
../Components/Src/bh1750_config.c \
../Components/Src/dio.c \
../Components/Src/led.c \
../Components/Src/led_config.c \
../Components/Src/pid2dof.c \
../Components/Src/pid2dof_config.c \
../Components/Src/pwm.c \
../Components/Src/serial_api.c 

OBJS += \
./Components/Src/__template.o \
./Components/Src/bh1750.o \
./Components/Src/bh1750_config.o \
./Components/Src/dio.o \
./Components/Src/led.o \
./Components/Src/led_config.o \
./Components/Src/pid2dof.o \
./Components/Src/pid2dof_config.o \
./Components/Src/pwm.o \
./Components/Src/serial_api.o 

C_DEPS += \
./Components/Src/__template.d \
./Components/Src/bh1750.d \
./Components/Src/bh1750_config.d \
./Components/Src/dio.d \
./Components/Src/led.d \
./Components/Src/led_config.d \
./Components/Src/pid2dof.d \
./Components/Src/pid2dof_config.d \
./Components/Src/pwm.d \
./Components/Src/serial_api.d 


# Each subdirectory must supply rules for building sources it contributes
Components/Src/%.o Components/Src/%.su: ../Components/Src/%.c Components/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/Desktop/Temperature_regulation_system/Components/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components-2f-Src

clean-Components-2f-Src:
	-$(RM) ./Components/Src/__template.d ./Components/Src/__template.o ./Components/Src/__template.su ./Components/Src/bh1750.d ./Components/Src/bh1750.o ./Components/Src/bh1750.su ./Components/Src/bh1750_config.d ./Components/Src/bh1750_config.o ./Components/Src/bh1750_config.su ./Components/Src/dio.d ./Components/Src/dio.o ./Components/Src/dio.su ./Components/Src/led.d ./Components/Src/led.o ./Components/Src/led.su ./Components/Src/led_config.d ./Components/Src/led_config.o ./Components/Src/led_config.su ./Components/Src/pid2dof.d ./Components/Src/pid2dof.o ./Components/Src/pid2dof.su ./Components/Src/pid2dof_config.d ./Components/Src/pid2dof_config.o ./Components/Src/pid2dof_config.su ./Components/Src/pwm.d ./Components/Src/pwm.o ./Components/Src/pwm.su ./Components/Src/serial_api.d ./Components/Src/serial_api.o ./Components/Src/serial_api.su

.PHONY: clean-Components-2f-Src

