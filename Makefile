# https://stackoverflow.com/a/18258352/13830772
rwildcard = $(filter-out \ ,$(foreach pattern,$(2),$(wildcard $(1)/$(pattern)))$(foreach child,$(wildcard $(1)/*),$(call rwildcard,$(child),$(2))))

CONFIG ?= config/release.mk
$(info Config file is $(CONFIG))
include $(CONFIG) # user configuration for user platform

BUILD_NAME     ?= default
CMAKE_BUILD_DIR = build/$(BUILD_NAME)
CMAKE_LINT_DIR  = build/$(BUILD_NAME)-lint
FILE_LIST       = build/$(BUILD_NAME)-file-list

FOLDERS_WITH_SOURCES = cmd src test-utils tests
CONFIGS = CMakeLists.txt $(foreach x,$(FOLDERS_WITH_SOURCES),$(call rwildcard,$(x),CMakeLists.txt))
CODES   = $(foreach x,$(FOLDERS_WITH_SOURCES),$(call rwildcard,$(x),*.h *.cpp))
$(shell cmake -D OUT=$(FILE_LIST) -D FILES="$(CODES)" -P ./cmake/update-file-list.cmake)

CMAKE_CONFIG_LINT = cmake $(CMAKE_OPTIONS) -B $(CMAKE_LINT_DIR) -D LINT=ON -D CMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_CONFIG      = cmake $(CMAKE_OPTIONS) -B $(CMAKE_BUILD_DIR)
CMAKE_LINT        = cmake --build $(CMAKE_LINT_DIR)  $(CMAKE_BUILD_OPTIONS)
CMAKE_BUILD       = cmake --build $(CMAKE_BUILD_DIR) $(CMAKE_BUILD_OPTIONS)

TEST_BUILD = $(CMAKE_BUILD) -t tests
TEST_RUN   = $(CMAKE_BUILD_DIR)/bin/tests

.PHONY: all
all: app

.PHONY: app
app: $(CMAKE_BUILD_DIR)
	$(CMAKE_BUILD) -t FabGrub
	@echo OUT EXECUTABLE: $(CMAKE_BUILD_DIR)/bin/FabGrub

.PHONY: test
test: $(CMAKE_BUILD_DIR)
	$(TEST_BUILD)
	$(TEST_RUN) --order-by=rand --test-suite-exclude=it

.PHONY: it
it: $(CMAKE_BUILD_DIR)
	$(TEST_BUILD)
	$(TEST_RUN) --order-by=rand --test-suite=it

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

$(CMAKE_BUILD_DIR): $(FILE_LIST) $(CONFIGS)
	$(CMAKE_CONFIG)

.PHONY: clean
clean:
	cmake -D PATH:STRING=build -P ./cmake/rm.cmake
