################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../empty_min.cfg 

CMD_SRCS += \
../MSP_EXP430FR6989.cmd 

C_SRCS += \
../main.c \
../ring_buffer.c \
../uart.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg 

C_DEPS += \
./main.d \
./ring_buffer.d \
./uart.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./main.obj \
./ring_buffer.obj \
./uart.obj 

GEN_MISC_DIRS__QUOTED += \
"configPkg" 

OBJS__QUOTED += \
"main.obj" \
"ring_buffer.obj" \
"uart.obj" 

C_DEPS__QUOTED += \
"main.d" \
"ring_buffer.d" \
"uart.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

C_SRCS__QUOTED += \
"../main.c" \
"../ring_buffer.c" \
"../uart.c" 


