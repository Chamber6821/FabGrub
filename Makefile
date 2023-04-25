
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

HEADERS = $(wildcard src/**/*.h test-utils/**/*.h)
SOURCES = $(wildcard src/**/*.cpp test-utils/**/*.cpp tests/**/*.cpp)
CONFIGS = $(wildcard **/CMakeLists.txt)
ALL     = $(HEADERS) $(SOURCES) $(CONFIGS)

app: cmake clang-format tests clang-tidy
	cmake --build $(BUILD_DIR) -t $(APP_TARGET) $(CMAKE_BUILD_OPTIONS)

tests: cmake $(ALL)
	cmake --build $(BUILD_DIR) -t $(TESTS_TARGET) $(CMAKE_BUILD_OPTIONS)
	$(BUILD_DIR)/$(TESTS_TARGET)/$(TESTS_TARGET)

clang-tidy: cmake
	clang-tidy -p $(BUILD_DIR) $(SOURCES)

clang-format: cmake
	clang-format -i $(HEADERS) $(SOURCES)

cmake: $(BUILD_DIR)

$(BUILD_DIR): $(CONFIGS)
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(MODE) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_OPTIONS)

clean:
	cmake -D PATH:STRING=$(BUILD_DIR) -P ./cmake/rm.cmake
