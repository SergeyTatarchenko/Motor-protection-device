#
# flexible makefile for stm32f0
#	STM32F030R8_FLASH.ld
#-------------------------------------------------------------------
CC=/home/sergey/toolchain/gcc-arm-none-eabi-8-2019-q3-update/bin/arm-none-eabi-gcc
LD=/home/sergey/toolchain/gcc-arm-none-eabi-8-2019-q3-update/bin/arm-none-eabi-gcc
CP=/home/sergey/toolchain/gcc-arm-none-eabi-8-2019-q3-update/bin/arm-none-eabi-objcopy
#-------------------------------------------------------------------
source_dirs:= Code/FreeRTOS Code/drivers Code/system Code/user Code/GCC_only     
include_path:= Code/drivers Code/system/include Code/FreeRTOS/include Code/user Code/GCC_only   

LDSCRIPT=-TSTM32F030R8_FLASH.ld
#-------------------------------------------------------------------
VPATH := $(source_dirs) $(include_dirs) 

search_wildcards_c:= $(addsuffix /*.c, $(source_dirs))
search_wildcards_s:= $(addsuffix /*.s, $(source_dirs))
#
source_files_c:= $(wildcard $(search_wildcards_c))
source_files_s:= $(wildcard $(search_wildcards_s))
#
source_files += $(source_files_c)
source_files += $(source_files_s)
#
object_files =  $(source_files_c:.c=.o)
object_files += $(source_files_s:.s=.o)
#
include := $(addprefix -I,$(include_path))
#-------------------------------------------------------------------
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mlittle-endian
CFLAGS += -mthumb
CFLAGS += -fno-exceptions
CFLAGS += -fno-unwind-tables
CFLAGS += -fno-asynchronous-unwind-tables
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += $(include)
#program breaks!!!
#CFLAGS += -Os
#-------------------------------------------------------------------
LDFLAGS += $(LDSCRIPT)
LDFLAGS += -mcpu=cortex-m0
LDFLAGS += -mlittle-endian
LDFLAGS += -mthumb
LDFLAGS += --specs=nosys.specs
LDFLAGS += -Wl,-Map=Mapfile.map,--gc-sections
#-------------------------------------------------------------------
target: program
	@echo "***MAKE COMPLETE***"
	@echo "Clean temporally files..."
	rm $(notdir $(object_files)) demo.elf
	
program: demo.elf
	@echo "Create hex and binary file"
	$(CP) -O ihex demo.elf demo.hex
	$(CP) -O binary demo.elf demo.bin
demo.elf: $(object_files)
	@echo "LINKING"
	$(LD) $(LDFLAGS) $(notdir $(object_files)) -o demo.elf
.c.o:
	$(CC) $(CFLAGS) -c $<
.s.o:
	$(CC) $(CFLAGS) -c $<
#-------------------------------------------------------------------
