
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
$(MODE) = ON

MAKEFLAGS += --no-print-directory

APP_TARGET   = hello
TESTS_TARGET = tests

BUILD_DIR = $(BUILD_DIR_PREFIX)$(MODE_LOWER)
CACHE_DIR = $(BUILD_DIR)/make-cache
TESTS_EXECUTABLE = "$(BUILD_DIR)/$(TESTS_TARGET)/$(TESTS_TARGET)"

HEADERS = $(wildcard src/**/*.h test-utils/**/*.h)
SOURCES = $(wildcard src/**/*.cpp test-utils/**/*.cpp tests/**/*.cpp)
CODES   = $(HEADERS) $(SOURCES)
CONFIGS = $(wildcard CMakeLists.txt **/CMakeLists.txt)

CLANG_FORMAT_CACHE_FOLDER = $(CACHE_DIR)/clang-format
CLANG_FORMAT_CACHE_FILES = $(foreach x,$(CODES),$(CLANG_FORMAT_CACHE_FOLDER)/$(x).label)

app: cmake clang-format tests
	cmake --build $(BUILD_DIR) -t $(APP_TARGET) $(CMAKE_BUILD_OPTIONS)

cmake: $(BUILD_DIR)
tests: $(CACHE_DIR)/tests
clang-format: $(CLANG_FORMAT_CACHE_FILES)

clean:
	cmake -D PATH:STRING=$(BUILD_DIR) -P ./cmake/rm.cmake

$(BUILD_DIR): $(CONFIGS)
	cmake -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(MODE) $(CMAKE_OPTIONS)

$(CACHE_DIR):
	cmake -D PATH:STRING=$(CACHE_DIR) -P ./cmake/mkdir-p.cmake

$(CACHE_DIR)/tests: $(BUILD_DIR) $(CODES) $(CACHE_DIR)
	cmake --build $(BUILD_DIR) -t $(TESTS_TARGET) $(CMAKE_BUILD_OPTIONS)
	$(TESTS_EXECUTABLE) --order-by=rand
	echo "" > $(CACHE_DIR)/tests

$(CLANG_FORMAT_CACHE_FILES): $(CLANG_FORMAT_CACHE_FOLDER)/%.label: % .clang-format
	cmake -D PATH:STRING=$(dir $@) -P ./cmake/mkdir-p.cmake
	clang-format -i $<
	echo "" > $@
