################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/led_operation.c \
../Source/main.c \
../Source/mongoose.c \
../Source/rest_api.c \
../Source/sensor_information.c 

OBJS += \
./Source/led_operation.o \
./Source/main.o \
./Source/mongoose.o \
./Source/rest_api.o \
./Source/sensor_information.o 

C_DEPS += \
./Source/led_operation.d \
./Source/main.d \
./Source/mongoose.d \
./Source/rest_api.d \
./Source/sensor_information.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.c Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -DDEBUG=1 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


