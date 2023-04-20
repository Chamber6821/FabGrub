
include configuration.mk # user configuration for user platform

# MODE = # from console
PERMISSIBLE_MODS=Debug Release
ifeq ($(filter $(MODE), $(PERMISSIBLE_MODS)),)
$(warning "Mode '$(MODE)' is not permissible!")
$(warning "Permissible mods: $(PERMISSIBLE_MODS)")
$(warning "Use 'make <target> MODE=Debug' or 'make <target> MODE=Release' to fix")
$(error "Mode '$(MODE)' is not permissible!")
endif

CMAKE_BUILD_TYPE = -DCMAKE_BUILD_TYPE$(MODE)
BUILD_DIR        = $(BUILD_DIR_PREFIX)$(MODE)

APP_TARGET   = FabGrub
TESTS_TARGET = tests

HEADERS = $(wildcard src/**/*.h)
SOURCES = $(wildcard src/**/*.cpp)
TESTS   = $(wildcard tests/**/*.cpp)
CONFIGS = $(wildcard **/CMakeLists.txt)
ALL     = $(HEADERS) $(SOURCES) $(TESTS) $(CONFIGS)

app: cmake tests
	cmake --build $(BUILD_DIR) -t $(APP_TARGET) $(CMAKE_BUILD_OPTIONS)

tests: cmake
	cmake --build $(BUILD_DIR) -t $(TESTS_TARGET) $(CMAKE_BUILD_OPTIONS)
	$(BUILD_DIR)/$(TESTS_TARGET)/$(TESTS_TARGET)

cmake: $(ALL)
	cmake -B $(BUILD_DIR) $(CMAKE_OPTIONS)

clean:
	cmake --build $(BUILD_DIR) -t clean
