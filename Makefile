#CROSS_COMPILE	?= /Users/cooltouya/Programs/linaro-arm-none-eabi/bin/arm-none-eabi-
CROSS_COMPILE	?= /Users/cooltouya/Programs/gcc-arm-none-eabi-4_9-2015q1/bin/arm-none-eabi-
DEBUG	?= 1
USE_FPU	?= 1

# Now #define for project
ifeq ($(DEBUG), 1)
else
endif

######## Location Configuration ########

INIT_DIR = init
ARCH_DIR = arch/armv7
INCLUDE_DIR = include
IRQ_DIR = irq
LIB_DIR = lib
MM_DIR = mm
SYS_DIR = sys

LINKER_DIR = $(LIB)/ldscript
LDSCRIPT = $(LINKER_DIR)/flash.ld
######## Build Configuration ########
VPATH += $(INIT_DIR)
VPATH += $(MM_DIR)/src
INCLUDES += -I.

OBJ = main.o

OBJ += $(patsubst %.cpp,%.o,$(notdir $(wildcard $(MM_DIR)/src/*.cpp)))
######## Compilation Configuration ########

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)g++
SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy

ifeq ($(USE_FPU), 1)
PROCESSOR = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
else
PROCESSOR = -mcpu=cortex-m4 -mthumb
endif

#LIBS = --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm -lrdimon -lnosys -Wl,--end-group
LIBS = 
#STFLAGS = -DUSE_STM32F4XX_NUCLEO -DSTM32F411xE -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 
STFLAGS = 
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
CFLAGS += $(PROCESSOR) $(INCLUDES) $(STFLAGS) -Wall 

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
.cpp.o:
	@$(CC_COMMAND)
.S.o:
	@$(CC_COMMAND)

LD_COMMAND = $(LD) $(LDFLAGS) $(foreach o,$(OBJ),$(BIN)/$(o)) -o $@
$(PROG).elf: $(OBJ) $(LDSCRIPT)
	@$(LD_COMMAND)

HEX_COMMAND = $(OBJCOPY) $< -O ihex $@
$(PROG).hex: $(PROG).elf
	@$(HEX_COMMAND)

BIN_COMMAND = $(OBJCOPY) $< -O binary --pad-to 0 $@
$(PROG).bin: $(PROG).elf
	@$(BIN_COMMAND)

CLEAN_O_COMMAND = rm -f $(BIN)/*.o
clean_o:
	@$(CLEAN_O_COMMAND)

CLEAN_COMMAND = rm -f $(PROG).elf $(PROG).bin $(PROG).hex $(PROG).map $(BIN)/dep/*.d
clean: clean_o
	@$(CLEAN_COMMAND)

