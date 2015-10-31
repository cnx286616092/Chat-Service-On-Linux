################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/getPK.c \
../src/link.c \
../src/main.c \
../src/mess.c \
../src/server.c 

OBJS += \
./src/getPK.o \
./src/link.o \
./src/main.o \
./src/mess.o \
./src/server.o 

C_DEPS += \
./src/getPK.d \
./src/link.d \
./src/main.d \
./src/mess.d \
./src/server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 4 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


