# final output file name
ARTIFACT = game

# target platform (QNX ARM 64)
PLATFORM ?= aarch64le
BUILD_PROFILE ?= debug

# config naming
CONFIG_NAME ?= $(PLATFORM)-$(BUILD_PROFILE)
OUTPUT_DIR = build/$(CONFIG_NAME)
TARGET = $(OUTPUT_DIR)/$(ARTIFACT)

# compiler
CC = qcc -Vgcc_nto$(PLATFORM)
LD = $(CC)

# includes and libs
INCLUDES += -Isrc
LIBS += -lm -lsocket

# flags
CCFLAGS_release += -O2
CCFLAGS_debug += -g -O0 -fno-builtin

CCFLAGS_all += -Wall -fmessage-length=0
CCFLAGS_all += $(CCFLAGS_$(BUILD_PROFILE))

LDFLAGS_all += $(LDFLAGS_$(BUILD_PROFILE))
LIBS_all += $(LIBS_$(BUILD_PROFILE))

# dependency generation
DEPS = -Wp,-MMD,$(@:%.o=%.d),-MT,$@

# source files
SRCS = src/main.c src/physics.c src/ai.c src/collision.c src/network.c

# object files
OBJS = $(patsubst src/%.c,$(OUTPUT_DIR)/%.o,$(SRCS))

# ✅ create output directory (FIX)
$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# compile rule (FIX: order-only dependency)
$(OUTPUT_DIR)/%.o: src/%.c | $(OUTPUT_DIR)
	$(CC) -c $(DEPS) -o $@ $(INCLUDES) $(CCFLAGS_all) $<

# link
$(TARGET): $(OBJS)
	$(LD) -o $(TARGET) $(LDFLAGS_all) $(OBJS) $(LIBS_all) $(LIBS)

# default
all: $(TARGET)

# clean
clean:
	rm -rf build

# rebuild
rebuild: clean all

# include dependencies
-include $(OBJS:.o=.d)

.PHONY: all clean rebuild