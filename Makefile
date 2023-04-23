
include configuration.mk # user configuration for user platform

MODE ?= Debug
PERMISSIBLE_MODS=Debug Release
ifeq ($(filter $(MODE), $(PERMISSIBLE_MODS)),)
$(warning "Mode '$(MODE)' is not permissible!")
$(warning "Permissible mods: $(PERMISSIBLE_MODS)")
$(warning "Use 'make <target> MODE=Debug' or 'make <target> MODE=Release' to fix")
$(error "Mode '$(MODE)' is not permissible!")
endif

BUILD_DIR = $(BUILD_DIR_PREFIX)$(MODE)

APP_TARGET   = hello
TESTS_TARGET = tests

HEADERS = $(wildcard src/**/*.h)
SOURCES = $(wildcard src/**/*.cpp)
TESTS   = $(wildcard tests/**/*Test.cpp)
CONFIGS = $(wildcard **/CMakeLists.txt)
ALL     = $(HEADERS) $(SOURCES) $(TESTS) $(CONFIGS)

app: cmake tests clang-tidy
	cmake --build $(BUILD_DIR) -t $(APP_TARGET) $(CMAKE_BUILD_OPTIONS)

tests: cmake
	cmake --build $(BUILD_DIR) -t $(TESTS_TARGET) $(CMAKE_BUILD_OPTIONS)
	$(BUILD_DIR)/$(TESTS_TARGET)/$(TESTS_TARGET)

clang-tidy: cmake
	clang-tidy -p $(BUILD_DIR) $(SOURCES) $(TESTS)

cmake: $(BUILD_DIR)

$(BUILD_DIR): $(ALL)
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(MODE) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_OPTIONS)

clean:
	cmake -D PATH:STRING=$(BUILD_DIR) -P ./cmake/rm.cmake
