BUILD_VERBOSE ?= 1
DEBUG		  ?= 1
ROOT = ..
MODULE_ROOT = $(ROOT)/modules
MODULE_SDK = $(MODULE_ROOT)/ps2sdk
include ../micropython/py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h
QSTR_GLOBAL_DEPENDENCIES += mpconfigport.h

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE = ee-
include $(PS2SDK)/samples/Makefile.eeglobal

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(BUILD)/genhdr
INC += -I$(MODULE_SDK)/include

EE_BIN = $(BUILD)/firmware.elf
EE_LIB = $(LIBMICROPYTHON)
CFLAGS = $(INC) $(EE_INCS) -Wall -Werror -std=c99 $(EE_CFLAGS) $(EE_NO_CRT) $(COPT)
LDFLAGS = $(EE_LDFLAGS) -T$(PS2SDK)/ee/startup/linkfile --gc-sections

# Tune for Debugging or Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -D__DEBUG
else
CFLAGS += -fdata-sections -ffunction-sections
endif

LIBS = -lc -lkernel -lahx -laudsrv

SRC_C = \
	main.c \
	uart_core.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/mp-readline/readline.c

MODULE_C = \
	$(MODULE_SDK)/src/rpc/ahx.c \
	$(MODULE_SDK)/src/rpc/audsrv.c \
	$(MODULE_SDK)/src/rpc/rpc.c

OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(MODULE_C:.c=.o))

PY_BYTECODE = frozentest.mpy
PY_SCRIPT = frozentest.py

all: $(LIB)

remake: clean all

fullclean: clean
	rm -f *.mpy *.a *.elf
	rm -rf modules

test: install $(PY_BYTECODE)
	$(MAKE) -f Makefile.pyfreeze test

retest: fullclean test
	
install: lib
	cp -f $(EE_LIB) $(PS2SDK)/ports/lib
	mkdir -p $(PS2SDK)/ports/include/micropython
	mkdir -p $(PS2SDK)/ports/include/micropython/genhdr
	cp -f $(BUILD)/genhdr/qstrdefs.preprocessed.h $(PS2SDK)/ports/include/micropython/genhdr
	cp -f $(BUILD)/genhdr/qstrdefs.generated.h $(PS2SDK)/ports/include/micropython/genhdr
	mkdir -p $(PS2DEV)/share/micropython
	cp -f ../micropython/tools/mpy-tool.py $(PS2DEV)/share/micropython
	cp -f ../micropython/py/makeqstrdata.py $(PS2DEV)/share/micropython
	cp -f Makefile.pyfreeze $(PS2DEV)/share/micropython/Makefile
	cp -f ps2-upython $(PS2DEV)/bin
	mkdir -p $(PS2SDK)/ports/include/micropython/py
	cp -f ../micropython/py/*.h $(PS2SDK)/ports/include/micropython/py
	cp *.h $(PS2SDK)/ports/include/micropython

$(PY_BYTECODE): $(PY_SCRIPT)
	mpy-cross $<

#$(BUILD)/_frozen_mpy.c: $(PY_BYTECODE) $(BUILD)/genhdr/qstrdefs.generated.h
#	$(ECHO) "MISC freezing bytecode"
#	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

include $(TOP)/py/mkrules.mk
