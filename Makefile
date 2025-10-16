CXX = g++
CXXFLAGS = -Wextra -Wall
CXXFLAGS_DEBUG = -Wextra -Wall -g -O0 -DDEBUG
CXXFLAGS_LINT = -Wall -Wextra -Wpedantic -Werror -Wshadow -Wnull-dereference -Wunused -Wdouble-promotion -Wformat=2 -Wstrict-aliasing=2 -Wstrict-overflow=5 -Wcast-align -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wuseless-cast
LIBS = -lraylib -llua

SRC_DIR = src
BUILD_DIR = build
FONTS_DIR = fonts

TARGET = $(BUILD_DIR)/rayed
DEBUG_TARGET = $(BUILD_DIR)/rayed_debug

SRC = $(SRC_DIR)/main.cpp
BACKTRACE_FILE = $(BUILD_DIR)/backtrace.txt

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEFINES) $< -o $@ $(LIBS)
	@echo "Build succeeded"

run: $(TARGET)
	./$(TARGET)

debug: $(DEBUG_TARGET)
	@echo "Starting GDB session for $(DEBUG_TARGET)..."
	@echo "set pagination off" > $(BUILD_DIR)/gdb_commands
	@echo "run" >> $(BUILD_DIR)/gdb_commands
	@echo "bt" >> $(BUILD_DIR)/gdb_commands
	@echo "quit" >> $(BUILD_DIR)/gdb_commands
	gdb -x $(BUILD_DIR)/gdb_commands $(DEBUG_TARGET) | tee $(BACKTRACE_FILE)
	@echo "Debug session completed. Backtrace saved to $(BACKTRACE_FILE)"
	@echo "Backtrace summary:"
	@grep -A 20 "Program received signal" $(BACKTRACE_FILE) || echo "No crash detected"

$(DEBUG_TARGET): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS_DEBUG) $(DEFINES) $< -o $@ $(LIBS)
	@echo "Debug build succeeded"

release: $(TARGET)
ifndef ARCH
	@echo "Architecture not specified; not creating release archive"
	@echo "Usage: make release ARCH=<architecture> OS=<operating_system> [DEFINES=...]"
else ifndef OS
	@echo "OS not specified; not creating release archive"
	@echo "Usage: make release ARCH=<architecture> OS=<operating_system> [DEFINES=...]"
else
	@echo "Creating release for $(ARCH) on $(OS) (that should match this architecture)"
	rm -f $(BUILD_DIR)/*.tar.gz
	tar -czvf $(BUILD_DIR)/$(ARCH)-$(OS).tar.gz $(BUILD_DIR)/* $(FONTS_DIR)/*
endif

clean:
	rm -rf $(BUILD_DIR)

lint: $(SRC)
	$(CXX) $(CXXFLAGS_LINT) -fsyntax-only $<
	clang-tidy $< --warnings-as-errors=* -- $(CXXFLAGS) $(DEFINES)
	@echo "Linting completed successfully"

ifneq ($(DEFINES),)
DEFINES := $(foreach def,$(DEFINES),-D$(def))
endif

.PHONY: all run release clean debug lint
