
CONFIG ?= config/release.mk
$(info Config file is $(CONFIG))
include $(CONFIG) # user configuration for user platform

BUILD_DIR ?= build
FAST_BUILD_DIR ?= $(BUILD_DIR)-fast

FOLDERS_WITH_SOURCES = cmd src test-utils tests it

HEADERS = $(foreach x,$(FOLDERS_WITH_SOURCES),$(wildcard $(x)/**/*.h))
SOURCES = $(foreach x,$(FOLDERS_WITH_SOURCES),$(wildcard $(x)/**/*.cpp))
CODES   = $(HEADERS) $(SOURCES)

CMAKE_CONFIG_LINT = cmake $(CMAKE_OPTIONS) -B $(BUILD_DIR) -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_CONFIG      = cmake $(CMAKE_OPTIONS) -B $(FAST_BUILD_DIR) -D FAST=ON
CMAKE_LINT        = cmake --build $(BUILD_DIR)      $(CMAKE_BUILD_OPTIONS)
CMAKE_BUILD       = cmake --build $(FAST_BUILD_DIR) $(CMAKE_BUILD_OPTIONS)

.PHONY: all
all: app

.PHONY: app
app: cmake
	$(CMAKE_BUILD) -t FabGrub
	@echo OUT EXECUTABLE: $(FAST_BUILD_DIR)/bin/FabGrub

.PHONY: test
test: cmake
	$(CMAKE_BUILD) -t tests
	$(FAST_BUILD_DIR)/bin/tests --order-by=rand

.PHONY: it
it: cmake
	$(CMAKE_BUILD) -t it
	$(FAST_BUILD_DIR)/bin/it --order-by=rand

.PHONY: lint
lint: all-formatted
	$(CMAKE_CONFIG_LINT)
	$(CMAKE_LINT)

.PHONY: format
format:
	clang-format -i $(CODES)

.PHONY: all-formatted
all-formatted:
	clang-format --dry-run -Werror $(CODES)

.PHONY: cmake
cmake:
	$(CMAKE_CONFIG)

.PHONY: clean
clean:
	cmake -D PATH:STRING=$(BUILD_DIR) -P ./cmake/rm.cmake
	cmake -D PATH:STRING=$(FAST_BUILD_DIR) -P ./cmake/rm.cmake
