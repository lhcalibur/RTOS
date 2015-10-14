#CROSS_COMPILE	?= /Users/cooltouya/Programs/linaro-arm-none-eabi/bin/arm-none-eabi-
CROSS_COMPILE	?= /Users/cooltouya/Programs/gcc-arm-none-eabi-4_9-2015q2/bin/arm-none-eabi-
DEBUG	?= 1
USE_FPU	?= 0

# Now #define for project
ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG_PRINT_ON_UART
else
endif

######## Location Configuration ########

INIT_DIR = init
ARCH_DIR = arch/armv7
INCLUDE_DIR = include
IRQ_DIR = irq
LIB_DIR = lib
MM_DIR = mm
HAL_DIR = hal
SYS_DIR = sys
TASK_DIR = task
SCHED_DIR = sched
CMSIS_DIR = $(LIB_DIR)/CMSIS
DRIVERS_DIR = drivers

LINKER_DIR = $(LIB_DIR)/ldscript
LDSCRIPT = $(LINKER_DIR)/flash.ld
######## Build Configuration ########
VPATH += $(INIT_DIR)
VPATH += $(CMSIS_DIR)/STM32F4xx/Source
VPATH += $(LIB_DIR)
VPATH += $(ARCH_DIR)/src
VPATH += $(SYS_DIR)/src
VPATH += $(MM_DIR)/src
VPATH += $(IRQ_DIR)/src
VPATH += $(HAL_DIR)/src
VPATH += $(TASK_DIR)/src
VPATH += $(SCHED_DIR)/src
VPATH += $(LIB_DIR)/STM32F4xx_HAL_Driver/Src
VPATH += $(LIB_DIR)/BSP/STM32F4xx-Nucleo
VPATH += $(DRIVERS_DIR)/src
INCLUDES += -I.
INCLUDES += -I$(CMSIS_DIR)/Include -I$(CMSIS_DIR)/STM32F4xx/Include -I$(INCLUDE_DIR)
INCLUDES += -I$(LIB_DIR)/STM32F4xx_HAL_Driver/Inc -I$(LIB_DIR)/BSP/STM32F4xx-Nucleo

OBJ = startup_stm32f411xe.o
OBJ += system_stm32f4xx.o stm32f4xx_nucleo.o syscalls.o
OBJ += main.o
OBJ += exception.o restorecontext.o switchcontext.o

OBJ += $(patsubst %.cpp,%.o,$(notdir $(wildcard $(SYS_DIR)/src/*.cpp)))
OBJ += $(patsubst %.cpp,%.o,$(notdir $(wildcard $(MM_DIR)/src/*.cpp)))
OBJ += $(patsubst %.cpp,%.o,$(notdir $(wildcard $(IRQ_DIR)/src/*.cpp)))
OBJ += $(patsubst %.cpp,%.o,$(notdir $(wildcard $(HAL_DIR)/src/*.cpp)))
OBJ += $(patsubst %.cpp,%.o,$(notdir $(wildcard $(ARCH_DIR)/src/*.cpp)))
OBJ += $(patsubst %.cpp,%.o,$(notdir $(wildcard $(TASK_DIR)/src/*.cpp)))
OBJ += $(patsubst %.cpp,%.o,$(notdir $(wildcard $(SCHED_DIR)/src/*.cpp)))
OBJ += $(patsubst %.cpp,%.o,$(notdir $(wildcard $(DRIVERS_DIR)/src/*.cpp)))

ST_LIB_C = $(wildcard $(LIB_DIR)/STM32F4xx_HAL_Driver/Src/*.c)
ST_LIB_O = $(patsubst %.c,%.o,$(notdir $(ST_LIB_C)))

OBJ += $(ST_LIB_O)

######## Compilation Configuration ########

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)g++
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
AR = $(CROSS_COMPILE)ar

ifeq ($(USE_FPU), 1)
PROCESSOR = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
else
PROCESSOR = -mcpu=cortex-m4 -mthumb
endif

#LIBS = --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -lnosys -Wl,--end-group
LIBS = 
#STFLAGS = -DUSE_STM32F4XX_NUCLEO -DSTM32F411xE -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DUSE_STDPERIPH_DRIVER 
STFLAGS = -DSTM32F411xE -DSTM32F4XX -DSTM32F40_41xxx -DHSE_VALUE=8000000 -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO

ifeq ($(USE_FPU), 1)
STFLAGS += -D__FPU_USED
endif

ifeq ($(DEBUG), 1)
#CFLAGS += -O0 -g
CFLAGS += -Os -g
else
CFLAGS += -Os -g3
endif

CFLAGS += -std=c++0x -std=gnu++0x -std=c++11 
#CFLAGS += $(PROCESSOR) $(INCLUDES) $(STFLAGS) -Wall -fno-strict-aliasing
CFLAGS += $(PROCESSOR) $(INCLUDES) $(STFLAGS) -Wall -fpermissive

#-Wextra -Wimplicit-function-declaration \
	  -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes 
# Compiler flags to generate dependency files:
CFLAGS += -MD -MP -MF $(BIN)/dep/$(@).d -MQ $(@)

# Permits to remove un-used functions and global variables from output file
#CFLAGS += -ffunction-sections -fdata-sections

ASFLAGS = $(PROCESSOR) $(INCLUDES)
LDFLAGS = $(PROCESSOR) --static $(LIBS) -T$(LDSCRIPT) -nostartfiles -Wl,-Map=$(PROG).map,--cref,--gc-sections 


# Program name
PROG = rtos
# Where to compile the .o
BIN = bin
VPATH += $(BIN)

# Dependency files to include
DEPS := $(foreach o,$(OBJ),$(BIN)/dep/$(o).d)

######## Targets ########

all: build
build: compile

compile: $(PROG).elf $(PROG).bin $(PROG).hex
CC_COMMAND = $(CC) $(CFLAGS) -c $< -o $(BIN)/$@
.c.o:
	@$(CC_COMMAND)
.cpp.o:
	@$(CC_COMMAND)
.S.o:
	@$(CC_COMMAND)

LD_COMMAND = $(LD) $(LDFLAGS) $(foreach o,$(OBJ),$(BIN)/$(o)) -o $@
$(PROG).elf: $(OBJ) $(LDSCRIPT)
	@$(LD_COMMAND)

AR_COMMAND = $(AR) rcs $(BIN)/$@ $(foreach o,$(ST_LIB_O),$(BIN)/$(o))
st-lib.a: $(ST_LIB_O)
	@$(AR_COMMAND)

#$(ST_LIB_O): $(ST_LIB_C)
#	@$(CC_COMMAND)


HEX_COMMAND = $(OBJCOPY) $< -O ihex $@
$(PROG).hex: $(PROG).elf
	@$(HEX_COMMAND)

BIN_COMMAND = $(OBJCOPY) $< -O binary --pad-to 0 $@
$(PROG).bin: $(PROG).elf
	@$(BIN_COMMAND)

CLEAN_O_COMMAND = rm -f $(BIN)/*.o
clean_o:
	@$(CLEAN_O_COMMAND)

CLEAN_COMMAND = rm -f $(PROG).elf $(PROG).bin $(PROG).hex $(PROG).map $(BIN)/dep/*.d $(BIN)/*.a
clean: clean_o
	@$(CLEAN_COMMAND)

