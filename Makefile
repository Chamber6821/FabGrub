
CONFIG ?= config/release.mk
$(info Config file is $(CONFIG))
include $(CONFIG) # user configuration for user platform

BUILD_DIR ?= build
LINT_DIR  ?= $(BUILD_DIR)-lint

FOLDERS_WITH_SOURCES = cmd src test-utils tests

HEADERS = $(foreach x,$(FOLDERS_WITH_SOURCES),$(wildcard $(x)/**/*.h))
SOURCES = $(foreach x,$(FOLDERS_WITH_SOURCES),$(wildcard $(x)/**/*.cpp))
CODES   = $(HEADERS) $(SOURCES)

CMAKE_CONFIG_LINT = cmake $(CMAKE_OPTIONS) -B $(LINT_DIR) -D LINT=ON -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_CONFIG      = cmake $(CMAKE_OPTIONS) -B $(BUILD_DIR)
CMAKE_LINT        = cmake --build $(LINT_DIR)  $(CMAKE_BUILD_OPTIONS)
CMAKE_BUILD       = cmake --build $(BUILD_DIR) $(CMAKE_BUILD_OPTIONS)

.PHONY: all
all: app

.PHONY: app
app: cmake
	$(CMAKE_BUILD) -t FabGrub
	@echo OUT EXECUTABLE: $(BUILD_DIR)/bin/FabGrub

.PHONY: test
test: cmake
	$(CMAKE_BUILD) -t tests
	$(BUILD_DIR)/bin/tests --order-by=rand --test-suite-exclude=it

.PHONY: it
it: cmake
	$(CMAKE_BUILD) -t tests
	$(BUILD_DIR)/bin/tests --order-by=rand --test-suite=it

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
	cmake -D PATH:STRING=$(LINT_DIR)  -P ./cmake/rm.cmake
