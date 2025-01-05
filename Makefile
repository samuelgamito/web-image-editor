CLANG_FORMAT = clang-format
STYLE = Google
SRC_DIR = .

# Find all C++ source and header files
SRC_FILES = $(shell find $(SRC_DIR) -type f \( -name "*.cpp" -o -name "*.h" \))

# Default target
all:
	@mkdir -p build
	@cd build/; cmake ..
	@echo "Use 'make format' to format all files."

# Format all files in place
format:
	@echo "formating: $(SRC_FILES)"
	@echo "Formatting files with clang-format ($(STYLE) style)..."
	@$(foreach file, $(SRC_FILES), $(CLANG_FORMAT) -i --style=$(STYLE) $(file);)
	@echo "Formatting complete."

# Dry run clang-formating
format-dry-run:
	@echo "formating: $(SRC_FILES)"
	@echo "Previewing format changes with clang-format ($(STYLE) style)..."
	@$(foreach file, $(SRC_FILES), $(CLANG_FORMAT) --dry-run --style=$(STYLE) $(file);)
	@echo "Dry run complete."

## Build
build:
	@cmake --build build

## Build And Run
build-run:
	@cmake --build build
	@build/./WebImageEditor

# Clean build project
clean:
	@rm -rf ./build
	@echo "Nothing to clean for this project."