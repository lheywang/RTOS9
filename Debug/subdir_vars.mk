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
../adc.c \
../hal_LCD.c \
../main.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_MISC_DIRS += \
./configPkg 

C_DEPS += \
./adc.d \
./hal_LCD.d \
./main.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./adc.obj \
./hal_LCD.obj \
./main.obj 

GEN_MISC_DIRS__QUOTED += \
"configPkg" 

OBJS__QUOTED += \
"adc.obj" \
"hal_LCD.obj" \
"main.obj" 

C_DEPS__QUOTED += \
"adc.d" \
"hal_LCD.d" \
"main.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

C_SRCS__QUOTED += \
"../adc.c" \
"../hal_LCD.c" \
"../main.c" 


