
include configuration.mk # user configuration for user platform

MODE ?= Debug
PERMISSIBLE_MODS=Debug Release
ifeq ($(filter $(MODE), $(PERMISSIBLE_MODS)),)
$(warning "Mode '$(MODE)' is not permissible!")
$(warning "Permissible mods: $(PERMISSIBLE_MODS)")
$(warning "Use 'make <target> MODE=Debug' or 'make <target> MODE=Release' to fix")
$(error "Mode '$(MODE)' is not permissible!")
endif
MODE_LOWER = $(subst Debug,debug,$(subst Release,release,$(MODE)))

MAKEFLAGS += --no-print-directory

APP_TARGET   = hello
TESTS_TARGET = tests

BUILD_DIR = $(BUILD_DIR_PREFIX)$(MODE_LOWER)
CACHE_DIR = $(BUILD_DIR)/make-cache
TESTS_EXECUTABLE = "$(BUILD_DIR)/$(TESTS_TARGET)/$(TESTS_TARGET)"

HEADERS = $(wildcard src/**/*.h test-utils/**/*.h)
SOURCES = $(wildcard src/**/*.cpp test-utils/**/*.cpp tests/**/*.cpp)
CONFIGS = $(wildcard **/CMakeLists.txt)
ALL     = $(HEADERS) $(SOURCES) $(CONFIGS)

app: cmake clang-format tests clang-tidy
	cmake --build $(BUILD_DIR) -t $(APP_TARGET) $(CMAKE_BUILD_OPTIONS)

cmake: $(BUILD_DIR)
tests: $(CACHE_DIR)/tests
clang-tidy: $(CACHE_DIR)/clang-tidy
clang-format: $(CACHE_DIR)/clang-format

clean:
	cmake -D PATH:STRING=$(BUILD_DIR) -P ./cmake/rm.cmake

$(BUILD_DIR): $(CONFIGS)
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(MODE) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_OPTIONS)

$(CACHE_DIR):
	mkdir -p "$(CACHE_DIR)"

$(CACHE_DIR)/tests: $(BUILD_DIR) $(ALL) $(CACHE_DIR)
	cmake --build $(BUILD_DIR) -t $(TESTS_TARGET) $(CMAKE_BUILD_OPTIONS)
	$(TESTS_EXECUTABLE) --order-by=rand
	echo "" > $(CACHE_DIR)/tests

$(CACHE_DIR)/clang-tidy: $(BUILD_DIR) $(CACHE_DIR) $(ALL) .clang-tidy
	clang-tidy -p $(BUILD_DIR) $(SOURCES)
	echo "" > $(CACHE_DIR)/clang-tidy

$(CACHE_DIR)/clang-format: $(CACHE_DIR) $(HEADERS) $(ALL) .clang-format
	clang-format -i $(HEADERS) $(SOURCES)
	echo "" > $(CACHE_DIR)/clang-format
