
CONFIG ?= config/release.mk
include $(CONFIG) # user configuration for user platform

BUILD_DIR ?= build
FAST_BUILD_DIR ?= $(BUILD_DIR)-fast

FOLDERS_WITH_SOURCES = cmd src test-utils tests it

HEADERS = $(foreach x,$(FOLDERS_WITH_SOURCES),$(wildcard $(x)/**/*.h))
SOURCES = $(foreach x,$(FOLDERS_WITH_SOURCES),$(wildcard $(x)/**/*.cpp))
CODES   = $(HEADERS) $(SOURCES)

all: format test it lint
.PHONY: all

app: TARGET = FabGrub
app: cmake-fast
	cmake --build $(FAST_BUILD_DIR) -t $(TARGET) $(CMAKE_BUILD_OPTIONS)
	@echo OUT EXECUTABLE: $(FAST_BUILD_DIR)/bin/$(TARGET)
.PHONY: app

test: TARGET = tests
test: cmake-fast
	cmake --build $(FAST_BUILD_DIR) -t $(TARGET) $(CMAKE_BUILD_OPTIONS)
	$(FAST_BUILD_DIR)/bin/$(TARGET) --order-by=rand
.PHONY: test

it: TARGET = it
it: cmake-fast
	cmake --build $(FAST_BUILD_DIR) -t $(TARGET) $(CMAKE_BUILD_OPTIONS)
	$(FAST_BUILD_DIR)/bin/$(TARGET) --order-by=rand
.PHONY: it

lint: all-formatted build-all
.PHONY: lint

format:
	clang-format -i $(CODES)
.PHONY: format

all-formatted:
	clang-format --dry-run -Werror $(CODES)
.PHONY: all-formatted

build-all: cmake
	cmake --build $(BUILD_DIR) $(CMAKE_BUILD_OPTIONS)
.PHONY: build-all

cmake:
	cmake -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON $(CMAKE_OPTIONS)
.PHONY: cmake

cmake-fast:
	cmake -B $(FAST_BUILD_DIR) -D FAST=ON $(CMAKE_OPTIONS)
.PHONY: cmake-fast

clean:
	cmake -D PATH:STRING=$(BUILD_DIR) -P ./cmake/rm.cmake
.PHONY: clean
