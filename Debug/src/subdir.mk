################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AES.cpp \
../src/main.cpp 

OBJS += \
./src/AES.o \
./src/main.o 

CPP_DEPS += \
./src/AES.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -std=c++0x -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -DDEBUG -DSTM32F103xB -DUSE_HAL_DRIVER -I"/home/mdk/workspace/ecryption_and_data_/HAL_Driver/Inc/Legacy" -I"/home/mdk/workspace/ecryption_and_data_/inc" -I"/home/mdk/workspace/ecryption_and_data_/CMSIS/device" -I"/home/mdk/workspace/ecryption_and_data_/CMSIS/core" -I"/home/mdk/workspace/ecryption_and_data_/HAL_Driver/Inc" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


